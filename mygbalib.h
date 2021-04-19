#include "sprites.h"
#define INPUT                      (KEY_MASK & (~REG_KEYS))

int userX = 10;
int userY = 90;
int enemyX = 220;
int enemyY = 90;
int fromGameOver = 0;
int level;

// Game state is to track the state of the game
// gameState 0: Menu
// gameState 1: In-game
// gameState 2: User win
// gameState 3: Game over
int gameState = 0;

// To debounce button A in gameState 1
int bufferButtonA = 0;
int bufferButtonS = 0;

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
        if(bufferButtonS == 0)
        {
            resetSprites();
            level = 1;
            // Initialize game parameter
            gameInit();
            // From menu to start game
            gameState = 1;
        }
    }
    else if (gameState == 2)
    {
        resetSprites();
        if (++level <= 2)
        {
            gameInit();
            gameState = 1;
        } 
        else
        {
            gameState = 0;
            bufferButtonS = 1;
            // gameOver();
        }
    }
    else if (gameState == 3)
    {
        resetSprites();
        gameInit();
        gameState = 1;
    }
}


void buttonR(void) //move right
{   
    if (gameState == 1 )
    {
        if (userX < 96) //move right if not in middle
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


void drawCongrats(void)
{
    delMidDivider();
    // CONGRATULATIONS!!
    drawSprite(29, 48, 56, 50);
    drawSprite(30, 49, 72, 50);
    drawSprite(31, 50, 88, 50);
    drawSprite(32, 51, 104, 50);
    drawSprite(33, 52, 120, 50);
    drawSprite(34, 53, 136, 50);
    drawSprite(35, 54, 152, 50);
    drawSprite(36, 55, 168, 50);

    if(level < 2)
    {  
        // NEXT LVL
        drawSprite(37, 56, 84, 120);
        drawSprite(38, 57, 100, 120);
        drawSprite(39, 58, 124, 120);
        drawSprite(40, 59, 140, 120);
        drawSprite(41, 60, 156, 120);
    }
    else
    {
        // MENU
        drawSprite(42, 62, 104, 120);
        drawSprite(43, 63, 120, 120);
    }
}


void drawGameOver(void)
{
    delMidDivider();
    // GAME OVER
    drawSprite(18, 33, 84, 50);
    drawSprite(19, 34, 100, 50);
    drawSprite(20, 35, 124, 50);
    drawSprite(21, 36, 140, 50);
    // START AGAIN? 
    drawSprite(15, 37, 76,120);
    drawSprite(16, 38, 92,120);
    drawSprite(17, 39, 108,120);
    drawSprite(22, 40, 124,120);
    drawSprite(23, 41, 140,120);
    drawSprite(24, 42, 156,120);
}


void drawMenu(void)
{
    resetSprites();
    // SHOOT!
    drawSprite(25, 43, 69,50);
    drawSprite(26, 44, 90,50);
    drawSprite(27, 45, 111,50);
    drawSprite(27, 46, 132,50);
    drawSprite(28, 47, 153,50);
    // START
    drawSprite(15, 30, 100,120);
    drawSprite(16, 31, 116,120);
    drawSprite(17, 32, 132,120);
}

