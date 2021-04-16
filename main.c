// -----------------------------------------------------------------------------
// C-Skeleton to be used with HAM Library from www.ngine.de
// -----------------------------------------------------------------------------
#include "mygbalib.h"
#include <mygba.h>


void Handler(void)
{
    REG_IME = 0x0; // Stop all other interrupt handling, while we handle this current one

    if ((REG_IF & INT_TIMER0) == INT_TIMER0) // TODO: replace XXX with the specific interrupt you are handling
    {
        // TODO: Handle timer interrupt here
        // ham_DrawText(1, 1, "Shootout Showdown");
        drawSprite(1, 2, x_pos, y_pos);
        x_pos+=10;
    }
    if ((REG_IF & INT_TIMER1) == INT_TIMER1) 
    {
    	checkbutton();
    }
    /*
    if ((REG_IF & XXX) == XXX)
    {
        
                
    }
	*/

    
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x1;  // Re-enable interrupt handling
}

// -----------------------------------------------------------------------------
// Project Entry Point
// -----------------------------------------------------------------------------
int main(void)
{
	// Initialize HAMlib
	ham_Init();
	//int i;
	// Set background mode
	ham_SetBgMode(2);

	//ham_LoadBGPal((void*)bg_Palette, 256);

	// Initialize built-in Text-System
	// ham_InitText(0);

	// Draw some text
	/*
	ham_DrawText(4, 1, "SNAKEE");
	for (i = 0; i < 10; i++)
		ham_DrawText(4, i, "SNAKEE");
	*/
	fillPalette();
	fillSprites();
	//drawSprite(0, 1, 40, 40);
    //drawSprite(1, 2, 160, 40);
	// Set Handler Function for interrupts and enable selected interrupts
	// REG_IME = 0x0;
    REG_INT = (int)&Handler;
    REG_IE  = INT_TIMER0 | INT_TIMER1;
    REG_IME = 0x1;		// Enable interrupt handling
    
    // Set Timer Mode (fill that section and replace TMX with selected timer number)
    
    /*
	clock 1: 59.595 nanosec
	clock 2: 3.8 microsec
	clock 3: 15.256 microsec
	clock 4: 61.025 microsec
    */

    // 60 FPS game
    // 16,666 microsecond per frame (273 step for clock4)
    REG_TM0D =	25261;		
    REG_TM0CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;

	// Check button (60 times/second)
    // 16,666 microsecond per frame (273 step for clock4)
    REG_TM1D =	55261;		
    REG_TM1CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;


	// Infinite loop
	for(;;);
	
	return 0;
}


/*
int main(void)
{
	int i;
	
    // Set Mode 2
    *(unsigned short *) 0x4000000 = 0x40 | 0x2 | 0x1000;

    // Fill SpritePal
    *(unsigned short *) 0x5000200 = 0;
    *(unsigned short *) 0x5000202 = RGB(31,31,31);

    // Fill SpriteData
    for (i = 0; i < 10*8*8/2; i++)
        spriteData[i] = (numbers[i*2+1] << 8) + numbers[i*2];
    for (i = 0; i < 129; i++)
    	drawSprite(0, i, 240, 160);
	// Set Handler Function for interrupts and enable selected interrupts
    REG_INT = (int)&Handler;
    REG_IE |= REG_TM0CNT; 				// TODO: complete this line to choose which timer interrupts to enable
    REG_IME = 0x1;		// Enable interrupt handling

    // Set Timer Mode (fill that section and replace TMX with selected timer number)
    REG_TM0D =	30000;		// TODO: complete this line to set timer initial value
    REG_TM0CNT |= TIMER_FREQUENCY_256 | TIMER_ENABLE;;		// TODO: complete this line to set timer frequency and enable timer

    while(1);

	return 0;
}
*/