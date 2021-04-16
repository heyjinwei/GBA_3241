// -----------------------------------------------------------------------------
// C-Skeleton to be used with HAM Library from www.ngine.de
// -----------------------------------------------------------------------------
#include "mygbalib.h"
#include <mygba.h>

int i;

int level = 1;
bool isMovingUp = 0;
int userHealth = 1;
int enemyHealth = 7;

void moveEnemy(void) 
{
    if (level == 1) 
    {
        if (enemyY < 144 && !isMovingUp) { //if you try to move the man out of the screen this prevents it
            enemyY += 1;       
        }
        if (enemyY > 16 && isMovingUp) { //if you try to move the man out of the screen this prevents it
            enemyY -= 1;       
        }
        if (enemyY >= 144 || enemyY <= 16) {
            isMovingUp = !isMovingUp;
        }
    }
}

void menuHandler(void)
{
    if (gameState == 0){
        drawMenu();
        checkbutton();
    }
}

void showHealthBar(void)
{
    drawSprite(6, 6, 112, 0); // draw health bar separato
    for (i=1; i <= userHealth; i++)
    {
        drawSprite(4, 6 + i, 112 - (i * 16), 0);
    }
    for( ; i <= 7; i++) 
    {
        drawSprite(7, 6 + i, 112 - (i * 16), 0);
    }
    for (i=1; i <= enemyHealth; i++)
    {
        drawSprite(5, 13 + i, 112 + (i * 16), 0);
    }
    for( ; i <= 7; i++) 
    {
        drawSprite(7, 13 + i, 112 - (i * 16), 0);
    }
}

void gameHandler(void)
{
    moveEnemy();
    checkbutton();
    drawSprite(0, 2, userX, userY);
    drawSprite(1, 3, enemyX, enemyY);
    if(gameState==1){
        showHealthBar();
        drawSprite(2, 4, 120, 40); //draw friendly rocket
        drawSprite(3, 5, 120, 120); //draw enemy rocket
    }
        
}

void Handler(void)
{
    REG_IME = 0x0; // Stop all other interrupt handling, while we handle this current one

    if ((REG_IF & INT_TIMER2) == INT_TIMER2)
    {
        menuHandler();              
    }
    
    if ((REG_IF & INT_TIMER0) == INT_TIMER0) 
    {
        gameHandler();
    }
    if ((REG_IF & INT_TIMER1) == INT_TIMER1) 
    {
        
    }
	    
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
    REG_IE  = INT_TIMER0 | INT_TIMER1 | INT_TIMER2;
    REG_IME = 0x1;		// Enable interrupt handling
    
    // Set Timer Mode (fill that section and replace TMX with selected timer number)
    
    /*
	clock 1: 59.595 nanosec
	clock 2: 3.8 microsec
	clock 3: 15.256 microsec
	clock 4: 61.025 microsec
    */

    // Check start (115 times/second)
    // 3.8 microsecond per check (2304 step for clock2)
    REG_TM2D =  53261;      
    REG_TM2CNT = TIMER_FREQUENCY_256 | TIMER_ENABLE | TIMER_INTERRUPTS;

    // 60 FPS game
    // 16,666 microsecond per frame (273 step for clock4)
    REG_TM0D =	65261;		
    REG_TM0CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE| TIMER_INTERRUPTS;

	// Check button (115 times/second)
    // 3.8 microsecond per check (2304 step for clock2)
    REG_TM1D =	63261;
    REG_TM1CNT = TIMER_FREQUENCY_256 | TIMER_ENABLE| TIMER_INTERRUPTS;

	// Infinite loop
	for(;;);
	
	return 0;
}

