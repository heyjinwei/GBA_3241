#include "sprites.h"
#define INPUT                      (KEY_MASK & (~REG_KEYS))
/*
#define SCREEN_WIDTH         240
#define SCREEN_HEIGHT        160
*/

int userX = 10;
int userY = 90;
int enemyX = 220;
int enemyY = 90;
int gameState = 0;

void checkbutton(void)
{
	// Gift function to show you how a function that can be called upon button interrupt 
    // to detect which button was pressed and run a specific function for each button could
    // look like. You would have to define each buttonA/buttonB/... function yourself.
    u16 buttons = INPUT;
    
    if ((buttons & KEY_A) == KEY_A)
    {
    }
    if ((buttons & KEY_B) == KEY_B)
    {
    }
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

void buttonS(void)
{
    if (gameState == 0)
    {
        gameState = 1;
        delMenu();
    }
}

void buttonR(void) //move right
{   
    if (gameState == 1 ){
        if (userX < 104) { //move right if not in middle
            userX += 1;
        }
    }
}

void buttonL(void) //move left
{
    if (gameState == 1 ){
        if (userX > 0) { //move left if not out of screen
            userX -= 1;
       }
    }
}

void buttonU(void) //move up
{
    if (gameState == 1 ){
        if (userY > 16) { //move up if below max height
            userY -= 1;      
        } 
    }
}

void buttonD(void) //move down
{
    if (gameState == 1 ){
        if (userY < 144) { //move down if above minimum height
            userY += 1;      
        } 
    }
}

void fillPalette(void)
{
    int     i;

	// Fill the palette in GBA memory
    for (i = 0; i < NCOLS; i++)
        spritePal[i] = palette[i];
}


void fillSprites(void)
{
    int     i;

	// Load all sprites in GBA memory
    for (i = 0; i < 128*16*16; i++)
        spriteData[i] = (sprites[i*2+1] << 8) + sprites[i*2];

	// draw all sprites on screen, but all of them outside of the screen (starting at position (240,160) the bottom right corner of the GBA screen)
    for(i = 0; i < 128; i++)
        drawSprite(0, i, 240,160);
}


void drawSprite(int numb, int N, int x, int y)
{
	// Same as CA2, make specific sprite (based on its name/numb) appear on screen, as slide number N (each sprite needs a different, arbitrary, N >= 0)
    *(unsigned short *)(0x7000000 + 8*N) = y | 0x2000;
    *(unsigned short *)(0x7000002 + 8*N) = x | 0x4000;
    *(unsigned short *)(0x7000004 + 8*N) = numb*8;
}


void drawMenu(void)
{
    if (gameState==0){
        drawSprite(25, 30, 100,120);
        drawSprite(26, 31, 116,120);
        drawSprite(27, 32, 132,120);
    }
        
}

void delMenu(void)
{
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