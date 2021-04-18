#include "sprites.h"
#define INPUT                      (KEY_MASK & (~REG_KEYS))

int userX = 10;
int userY = 90;
int enemyX = 220;
int enemyY = 90;
int fromGameOver = 0;
// Game state is to track the state of the game
// gameState 0: Menu
// gameState 1: In-game
// gameState 2: User win
// gameState 3: Game over
int gameState = 0;

// To debounce button A in gameState 1
int bufferButtonA = 0;

// Should be a boolean
int isShielded = 0;

void checkbutton(void)
{
	// Gift function to show you how a function that can be called upon button interrupt 
    // to detect which button was pressed and run a specific function for each button could
    // look like. You would have to define each buttonA/buttonB/... function yourself.
    u16 buttons = INPUT;    
    checkButtonBThenA(buttons);
    if ((buttons & KEY_SELECT) == KEY_SELECT)
    {
    }
    if ((buttons & KEY_START) == KEY_START)
    {
        buttonS();
    }
    if ((buttons & KEY_RIGHT) == KEY_RIGHT)
    {
        buttonR();
    }
    if ((buttons & KEY_LEFT) == KEY_LEFT)
    {
        buttonL();
    }
    if ((buttons & KEY_UP) == KEY_UP)
    {
        buttonU();
    }
    if ((buttons & KEY_DOWN) == KEY_DOWN)
    {
        buttonD();
    }
}

void checkButtonBThenA(u16 buttons) // Check if button B is pressed then checks A. Both cannot be on at the same time.
{
    if ((buttons & KEY_B) == KEY_B)
    {
        isShielded = 1; // while button is pressed, shield is on
        return;
    } else if ((buttons & KEY_A) == KEY_A)
    {
        buttonA();
    }
    isShielded = 0;
}


void buttonA(void) // When button A is pressed
{
    if (gameState == 1) // Double confirm the gamestate
    {
        // Only do at the moment the button is pressed
        // holding the button does not trigger multiple times
        if(bufferButtonA==0) 
        {
            bufferButtonA = 1;
            userHandler();    
        }
    }
}

void buttonS(void)
{
    if (gameState == 0)
    {
        delMenu();
        // Initialize game parameter
        gameInit();
        // From menu to start game
        gameState = 1;
    }
    else if (gameState == 3)
    {
        delGameOver();
        gameInit();
        gameState = 1;
    }
}


void buttonR(void) //move right
{   
    if (gameState == 1 )
    {
        if (userX < 104) //move right if not in middle
        { 
            userX += 1;
        }
    }
}

void buttonL(void) //move left
{
    if (gameState == 1 )
    {
        if (userX > 0) //move left if not out of screen
        { 
            userX -= 1;
       }
    }
}

void buttonU(void) //move up
{
    if (gameState == 1 )
    {
        if (userY > 16) //move up if below max height
        { 
            userY -= 1;      
        } 
    }
}


void buttonD(void) //move down
{
    if (gameState == 1 )
    {
        if (userY < 144) //move down if above minimum height
        { 
            userY += 1;      
        } 
    }
}


void fillPalette(void)
{
    int     i;
	// Fill the palette in GBA memory
    for (i = 0; i < NCOLS; i++)
    {
        spritePal[i] = palette[i];
    }
}


void fillSprites(void)
{
    int     i;
	// Load all sprites in GBA memory
    for (i = 0; i < 128*16*16; i++)
    {
        spriteData[i] = (sprites[i*2+1] << 8) + sprites[i*2];
    }

	// draw all sprites on screen, but all of them outside of the screen (starting at position (240,160) the bottom right corner of the GBA screen)
    for(i = 0; i < 128; i++)
    {
        drawSprite(0, i, 240,160);
    }
}

// draw all sprites on screen, but all of them outside of the screen (starting at position (240,160) the bottom right corner of the GBA screen)
void resetSprites(void)
{
    int i = 0;
    for(i = 0; i < 128; i++)
    {
        drawSprite(0, i, 240,160);
    }
}


void drawSprite(int numb, int N, int x, int y)
{
	// Same as CA2, make specific sprite (based on its name/numb) appear on screen, as slide number N (each sprite needs a different, arbitrary, N >= 0)
    *(unsigned short *)(0x7000000 + 8*N) = y | 0x2000;
    *(unsigned short *)(0x7000002 + 8*N) = x | 0x4000;
    *(unsigned short *)(0x7000004 + 8*N) = numb*8;
}

void drawGameOver(void)
{
    // GAME OVER
    drawSprite(28, 33, 84, 50);
    drawSprite(29, 34, 100, 50);
    drawSprite(30, 35, 124, 50);
    drawSprite(31, 36, 140, 50);
    // START AGAIN? 
    drawSprite(25, 37, 76,120);
    drawSprite(26, 38, 92,120);
    drawSprite(27, 39, 108,120);
    drawSprite(32, 40, 124,120);
    drawSprite(33, 41, 140,120);
    drawSprite(34, 42, 156,120);
}

void delGameOver(void)
{
    // GAME OVER
    drawSprite(28, 33, 240,160);
    drawSprite(29, 34, 240,160);
    drawSprite(30, 35, 240,160);
    drawSprite(31, 36, 240,160);
    // START AGAIN? 
    drawSprite(25, 37, 240,160);
    drawSprite(26, 38, 240,160);
    drawSprite(27, 39, 240,160);
    drawSprite(32, 40, 240,160);
    drawSprite(33, 41, 240,160);
    drawSprite(34, 42, 240,160);
}

void drawMenu(void)
{
    if (gameState==0)
    {
        // SHOOT!
        drawSprite(35, 43, 69,50);
        drawSprite(36, 44, 90,50);
        drawSprite(37, 45, 111,50);
        drawSprite(37, 46, 132,50);
        drawSprite(38, 47, 153,50);
        // START
        drawSprite(25, 30, 100,120);
        drawSprite(26, 31, 116,120);
        drawSprite(27, 32, 132,120);
    }
        
}


void delMenu(void)
{
    drawSprite(35, 43, 240,160);
    drawSprite(36, 44, 240,160);
    drawSprite(37, 45, 240,160);
    drawSprite(37, 46, 240,160);
    drawSprite(38, 47, 240,160);
    drawSprite(25, 30, 240,160);
    drawSprite(26, 31, 240,160);
    drawSprite(27, 32, 240,160);
}

/*
void drawLaser(void)
{
	// Gift function showing you how to draw an example sprite defined in sprite.h on screen, using drawSprite()
	// Note that this code uses largeer sprites with a palette, so the main code needs to be initialized in graphical mode 2, using:
    //		*(unsigned short *) 0x4000000 = 0x40 | 0x2 | 0x1000;
	// at the beginning of main() in main.c

    switch(lPlat) {
        case 16:
        {
            drawSprite(LASER, NPLATS*3 + 5 + NROCK + NMET, LaserX, LaserY);
            break;
        }
        case 9:
        {
            drawSprite(LASER, NPLATS*2 + 5 + NROCK + NMET, LaserX, LaserY);
            break;
        }
        default:
            break;
    }
}
*/