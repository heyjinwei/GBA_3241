// -----------------------------------------------------------------------------
// C-Skeleton to be used with HAM Library from www.ngine.de
// -----------------------------------------------------------------------------
#include "mygbalib.h"
#include <mygba.h>
#include <time.h>
#include <stdlib.h>

#define TELEPORT_TIME 300

int i;
int j;

//enemy special variables
bool isMovingUp = 0;
int teleportTimer = 0;
int teleportCount = 0;
int randomIntegers[50] = {223, 119, 182, 164, 214, 34, 60, 249, 254, 251, 114, 218, 132, 73, 17, 13, 120, 126, 7, 82, 175, 65, 163, 193, 127, 42, 151, 151, 168, 53, 132, 135, 172, 58, 43, 130, 93, 104, 124, 91, 99, 238, 54, 231, 55, 71, 244, 176, 198, 251};

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
	userX = 10;
	userY = 90;
	enemyX = 220;
	enemyY = 90;	
	if(gameState != 2)
	{
		level = 1;
	}
	userHealth = 7;
	enemyHealth = 7;
	teleportTimer = 0;
    teleportCount = 0;
	
	enemyRocketInd  = 0;
	userRocketInd  = 0;

	rocketInit();
}

void rocketInit(void)
{
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


int generateRandomInt(int high, int low)
{
	int randomInt = (rand() % (high - low + 1)) + low;
	return randomInt;
}

void teleportAndShoot(void)
{
    enemyX = generateRandomInt(224, 128);
    enemyY = generateRandomInt(144, 16);
    enemyHandler();
}

void moveEnemy(void) 
{
    if (level == 1) 
    {
        if (enemyY < 144 && !isMovingUp) 
        { //if you try to move the man out of the screen this prevents it
            enemyY += 1;       
        } else if (enemyY > 16 && isMovingUp) 
        { //if you try to move the man out of the screen this prevents it
            enemyY -= 1;       
        }
        if (enemyY >= 144 || enemyY <= 16) 
        {
            isMovingUp = !isMovingUp;
        }
    } else if (level == 2)
    {
    	teleportTimer++;
    	if (teleportTimer >= (TELEPORT_TIME - 100) && teleportCount < 1)
    	{
            teleportAndShoot();
    		teleportCount++;
    	}
        else if (teleportTimer == (TELEPORT_TIME - 50) && teleportCount < 2)
        {
            teleportAndShoot();
            teleportCount++;
        }
        else if (teleportTimer >= TELEPORT_TIME)
        {
            teleportAndShoot();
            teleportTimer = 0;
            teleportCount = 0;
        }
    }
}

void drawEndgame(void)
{
	delMidDivider();
	showHealthBar();
	if (gameState == 3)
	{
		drawSprite(10, 6, 112, 0); 				// draw health bar separator for lose
		drawEnemy();
		//drawSprite(44, 2, userX, userY);		// draw user ashes
	} else
	{
		drawSprite(11, 6, 112, 0); 				// draw health bar separator for win
		drawUser();
		//drawSprite(44, 3, enemyX, enemyY);		// draw enemy ashes
	}
	// drawMidDivider();
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

void drawUser(void)
{
	if (isShielded)
	{
		drawSprite(8, 2, userX, userY);
	} else
	{
		drawSprite(0, 2, userX, userY);
	}
}

void drawEnemy(void)
{
	if (level == 1)
	{
		drawSprite(1, 3, enemyX, enemyY);
	} else if (level == 2)
	{
		if ((teleportTimer > (TELEPORT_TIME - 20)) || teleportTimer < 10)
		{
			drawSprite(14, 3, enemyX, enemyY);
		} else
		{
			drawSprite(12, 3, enemyX, enemyY);
		}
		
	}
	
}

void drawMidDivider(void)
{
	int y = 16;   
	int count = 0;
	while (y <= 160)
	{
		drawSprite(9, 120 + count, 112, y);
		++count;
		y += 16;
	}	
}

void delMidDivider(void)
{
	int y = 16;   
	int count = 0;
	while (y <= 160)
	{
		drawSprite(9, 120 + count, 240, 160);
		++count;
		y += 16;
	}	
}

void gameHandler(void) 
{
    
    checkbutton();
	moveEnemy(); 
    
    if (gameState==0)
    {
    	drawMenu();     
    }
    else if(gameState==1)
    {	
    	drawUser();
    	drawEnemy();
    	drawMidDivider();
        showHealthBar();
        drawRockets();
    }
    else if(gameState==2)
    { 
    	drawCongrats();
    	drawEndgame(); 
    }
    else if(gameState==3)
    {
    	drawGameOver();
    	drawEndgame();
    }
}

void userDamaged(void)
{
	if (isShielded)
	{
		userHealth--;
	} 
	else
	{
		userHealth -= 2;
	}

	if(userHealth < 0)
	{
		resetSprites();
		gameState = 3; //Game over
	}
}

void enemyDamaged(void)
{
	enemyHealth--; 
	if(enemyHealth < 0)
	{
		resetSprites();
		gameState = 2; //User win		
	}
}

void moveEnemyRockets(int j)
{	
	if (level == 1)
	{
		enemyRocketX[j] -= 3; // Travel speed
	} 
	else if (level == 2)
	{
		enemyRocketX[j] -= 2; // Travel speed
		if (teleportTimer % 2)
		{
			if (enemyRocketY[j] > userY) 
			{
				enemyRocketY[j]--;
			} 
			else if (enemyRocketY[j] < userY)
			{
				enemyRocketY[j]++;
			}
		}
	}
}

void drawEnemyRocket(int j)
{
	if (level == 1)
	{
		drawSprite(2, 80+j, enemyRocketX[j], enemyRocketY[j]);
	} 
	else if (level == 2)
	{
		drawSprite(13, 80+j, enemyRocketX[j], enemyRocketY[j]);
	}
}

void drawRockets(void)
{	
	for(j = 0; j<10; j++) // Loop through all 10 available rockets
	{
		if(enemyRocketState[j] == 1) // If rocket is travelling
		{
			moveEnemyRockets(j);
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
			drawEnemyRocket(j);
			
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
    
    if ((REG_IF & INT_TIMER0) == INT_TIMER0) 
    {
        gameHandler();
    }
    if ((REG_IF & INT_TIMER1) == INT_TIMER1) 
    {
    	// Reset button availability every 0.5 second
    	bufferButtonA = 0;
    	bufferButtonS = 0;

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
	resetSprites();
	
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

    // Set 60 FPS game and check for user control
    // 16,666 microsecond per frame (273 step for clock4)
    REG_TM0D =	65261;		
    REG_TM0CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE| TIMER_INTERRUPTS;

	// Game logic for enemy (0.5 time/second)
    // 32772 step for clock4
    REG_TM1D =	32763;
    REG_TM1CNT = TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;

	// Infinite loop
	for(;;);
	
	return 0;
}

