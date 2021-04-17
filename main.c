// -----------------------------------------------------------------------------
// C-Skeleton to be used with HAM Library from www.ngine.de
// -----------------------------------------------------------------------------
#include "mygbalib.h"
#include <mygba.h>

int i;
int j;
int level = 1;
bool isMovingUp = 0;

int userHealth;
int enemyHealth;

int enemyRocketInd;
int userRocketInd;

int enemyRocketState[10];
int enemyRocketX[10];
int enemyRocketY[10];

int userRocketState[10];
int userRocketX[10];
int userRocketY[10];


// Function to initialize game parameter
// Future dev: Add level, where each level has
// different initialized parameter
void gameInit(void)
{
	userHealth = 7;
	enemyHealth = 7;
	
	enemyRocketInd  = 0;
	userRocketInd  = 0;

	for(i = 0; i < 10; i++) // Initialize rocket parameters
	{
		// To track the rocket's state
		// State 0: Ready to be fired
		// State 1: Travelling
		enemyRocketState[i] = 0;
		// Rocket's position 
		enemyRocketX[i] = 240; 
		enemyRocketY[i] = 160;

		userRocketState[i] = 0;
		userRocketX[i] = 0;
		userRocketY[i] = 160;
	}
}


void moveEnemy(void) 
{
    if (level == 1) 
    {
        if (enemyY < 144 && !isMovingUp) 
        { //if you try to move the man out of the screen this prevents it
            enemyY += 1;       
        }
        if (enemyY > 16 && isMovingUp) 
        { //if you try to move the man out of the screen this prevents it
            enemyY -= 1;       
        }
        if (enemyY >= 144 || enemyY <= 16) 
        {
            isMovingUp = !isMovingUp;
        }
    }
}

void gameOver(void)
{
	gameState = 3;
	drawGameOver();
}

void menuHandler(void)
{
    if (gameState == 0)
    {
        drawMenu();
        checkbutton();
    }
}

void showHealthBar(void)
{
    drawSprite(6, 6, 112, 0); 				// draw health bar separator
    for (i=1; i <= userHealth; i++) {		// draw user health
        drawSprite(4, 6 + i, 112 - (i * 16), 0);
    }
    for( ; i <= 7; i++) { 					// draw depleted health for user
        drawSprite(7, 6 + i, 112 - (i * 16), 0);
    }
    for (i=1; i <= enemyHealth; i++) {		//draw enemy health
        drawSprite(5, 13 + i, 112 + (i * 16), 0);
    }
    for( ; i <= 7; i++) { 					// draw depleted health for enemy
        drawSprite(7, 13 + i, 112 + (i * 16), 0);
    }				
}


void gameHandler(void)
{
    moveEnemy();
    checkbutton();
    drawSprite(0, 2, userX, userY);
    drawSprite(1, 3, enemyX, enemyY);
    if(gameState==1)
    {
        showHealthBar();
        drawRockets();
    }
    else if(gameState==2)
    {
    	gameState = 0;
    	// <Insert User wins details>
    	// Print win and go next lvl
    	// Next level can just change gameState==1 and change parameters (i.e. rocketspeed)
    }
    else if(gameState==3)
    {
    	gameOver();
    	// <Insert Game Over details>
    	// Print game over
    	// Go back to menu (gameState = 0)
    }
}


void userDamaged(void)
{
	userHealth--;
	if(userHealth < 0)
	{
		gameState = 3; //Game over
	}
}


void enemyDamaged(void)
{
	enemyHealth--;
	if(enemyHealth < 0)
	{
		gameState = 2; //User win
	}
}


void drawRockets(void)
{	
	for(j = 0; j<10; j++) // Loop through all 10 available rockets
	{
		if(enemyRocketState[j] == 1) // If rocket is travelling
		{
			enemyRocketX[j] -= 3; // Travel speed
			// Check if rocket collides with user
			if(enemyRocketX[j]<=userX+5 && enemyRocketX[j]>=userX-5 && enemyRocketY[j]<=userY+10 && enemyRocketY[j]>=userY-5)
			{
				userDamaged(); // If collide, damage
				enemyRocketState[j] = 0; // Rocket collided, back to state 0
			}
			// Check if rocket is going out of bound
			if(enemyRocketX[j]<=3 || enemyRocketState[j]==0)
			{
				// If out of bound, reset and back to state 0
				enemyRocketX[j] = 240;
				enemyRocketY[j] = 160;
				enemyRocketState[j] = 0;
			}
			drawSprite(2, 80+j, enemyRocketX[j], enemyRocketY[j]);
		}
		if(userRocketState[j] == 1) // Same thing but from user perspective
		{
			userRocketX[j] += 3;
			if(userRocketX[j]<=enemyX+5 && userRocketX[j]>=enemyX-5 && userRocketY[j]<=enemyY+10 && userRocketY[j]>=enemyY-5)
			{
				enemyDamaged();
				userRocketState[j] = 0;
			}
			if(userRocketX[j]>=237 || userRocketState[j]==0 )
			{
				userRocketX[j] = 0;
				userRocketY[j] = 160;
				userRocketState[j] = 0;
			}
			drawSprite(3, 100+j, userRocketX[j], userRocketY[j]);
		}		
	}
}


void enemyHandler(void)
{
	if(gameState == 1) // If game is ongoing
	{
		if (enemyRocketInd==10) // Check if the rocket index is going out of bound
		{
			// If looped through all 10 rockets, go back to the first one
			// as it is most probably ready to be used again
			enemyRocketInd = 0;
		}
		// Check if the rocket is ready to be used
		if(enemyRocketState[enemyRocketInd]==0)
		{
			// If it is, fire it
			enemyRocketState[enemyRocketInd] = 1; // State changes from idle to travelling
			enemyRocketX[enemyRocketInd] = enemyX-10; // Fire from the agent's location
			enemyRocketY[enemyRocketInd] = enemyY-5;
		}		
		enemyRocketInd++;
	}
}


void userHandler(void)
{
	if(gameState==1)
	{
		if (userRocketInd==10)
		{
			userRocketInd = 0;
		}
		if(userRocketState[userRocketInd]==0)
		{
			userRocketState[userRocketInd] = 1;
			userRocketX[userRocketInd] = userX+10;
			userRocketY[userRocketInd] = userY-5;
		}		
		userRocketInd++;
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
    	bufferButtonA = 0;
        enemyHandler();
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

	// Set background mode
	ham_SetBgMode(2);

	fillPalette();
	fillSprites();
	
	// Set Handler Function for interrupts and enable selected interrupts
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

    // Set 60 FPS game and check for user control
    // 16,666 microsecond per frame (273 step for clock4)
    REG_TM0D =	65261;		
    REG_TM0CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE| TIMER_INTERRUPTS;

	// Game logic for enemy (0.5 time/second)
    // 32772 step for clock4
    REG_TM1D =	32763;
    REG_TM1CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;

    // Game logic for user (0.5 time/second)
    // 32772 step for clock4
    REG_TM1D =	32763;
    REG_TM1CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;

	// Infinite loop
	for(;;);
	
	return 0;
}

