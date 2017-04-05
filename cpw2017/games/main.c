/*
 * Next_Make_CPW_2017.c
 */


#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
//#include <avr/pgmspace.h>

//PA0 BUT_R_R
//PA1 BUT_R_U
//PA2 BUT_R_D
//PA3 BUT_R_L
//PA4 
//PA5 
//PA6 
//PA7 

//PB0 BUT_L_U
//PB1 BUT_L_R
//PB2 BUT_L_D
//PB3 
//PB4 
//PB5 
//PB6 BUT_L_L
//PB7 

//If the LED matrix is in the right way
//char correctLED = 1;
//PC0 Extra2
//PC1 Row1-
//PC2 Row2-
//PC3 Row3-
//PC4 Row4-
//PC5 Row5-
//PC6 Reset*
//PC7 Extra3

//PD0 Extra1
//PD1 Col1+
//PD2 Col2+
//PD3 Col3+
//PD4 Col4+
//PD5 Col5+
//PD6 Col6+
//PD7 Col7+


//If the LED matrix is in backwards
char correctLED = 0;
//PC0 Extra2
//PC1 Row4-//
//PC2 Col4+//
//PC3 Col5+//
//PC4 Row1-//
//PC5 Col3+//
//PC6 Reset*
//PC7 Extra3

//PD0 Extra1
//PD1 Col7+//
//PD2 Col6+//
//PD3 Row5-//
//PD4 Row2-//
//PD5 Row3-//
//PD6 Col2+//
//PD7 Col1+//




//current LED output
//0 is off
//1 is on dim
//2 in on bright
unsigned char outputLed[7][5];
char buttons[8];

unsigned char menu_run = 0;



unsigned char row = 0;
unsigned char everyNth = 0;
unsigned char everyFourth = 0;
void writeLeds()
{
	if(correctLED)
	{
		//turn off all rows
		DDRC |= (1<<PC1);
		DDRC |= (1<<PC2);
		DDRC |= (1<<PC3);
		DDRC |= (1<<PC4);
		DDRC |= (1<<PC5);
		
		PORTC |= (1<<PC1);
		PORTC |= (1<<PC2);
		PORTC |= (1<<PC3);
		PORTC |= (1<<PC4);
		PORTC |= (1<<PC5);
		
		if(row==1)
		{
			//turn on correct row
			PORTC &= ~(1<<PC1);
		}
		else if(row==2)
		{
			//turn on correct row
			PORTC &= ~(1<<PC2);
		}
		else if(row==3)
		{
			//turn on correct row
			PORTC &= ~(1<<PC3);
		}
		else if(row==4)
		{
			//turn on correct row
			PORTC &= ~(1<<PC4);
		}
		else
		{
			//turn on correct row
			PORTC &= ~(1<<PC5);
		}
		
		//turn on Leds in that row
		for(unsigned char i=0; i<7; i++)
		{
			if(outputLed[i][row]==0)
			{
				DDRD |= (1<<(i+1));
				PORTD &= ~(1<<(i+1));
			}
			else if(outputLed[i][row]==1)
			{
				DDRD &= ~(1<<(i+1));
				PORTD |= (1<<(i+1));
			}
			else
			{
				DDRD |= (1<<(i+1));
				PORTD |= (1<<(i+1));
			}
		}
		
	}
	else
	{
		//turn off all rows
		DDRC |= (1<<PC4);
		DDRD |= (1<<PD4);
		DDRD |= (1<<PD5);
		DDRC |= (1<<PC1);
		DDRD |= (1<<PD3);
		
		PORTC |= (1<<PC4);
		PORTD |= (1<<PD4);
		PORTD |= (1<<PD5);
		PORTC |= (1<<PC1);
		PORTD |= (1<<PD3);
		
		//and cols
		DDRD |= (1<<PD7);
		PORTD &= ~(1<<PD7);
		DDRD |= (1<<PD6);
		PORTD &= ~(1<<PD6);
		DDRC |= (1<<PC5);
		PORTC &= ~(1<<PC5);
		DDRC |= (1<<PC2);
		PORTC &= ~(1<<PC2);
		DDRC |= (1<<PC3);
		PORTC &= ~(1<<PC3);
		DDRD |= (1<<PD2);
		PORTD &= ~(1<<PD2);
		DDRD |= (1<<PD1);
		PORTD &= ~(1<<PD1);
		
			
			
		if(row==0)
		{
			//turn on correct row
			PORTC &= ~(1<<PC4);
		}
		else if(row==1)
		{
			//turn on correct row
			PORTD &= ~(1<<PD4);
		}
		else if(row==2)
		{
			//turn on correct row
			PORTD &= ~(1<<PD5);
		}
		else if(row==3)
		{
			//turn on correct row
			PORTC &= ~(1<<PC1);
		}
		else
		{
			//turn on correct row
			PORTD &= ~(1<<PD3);
		}
		
		
		//turn on Leds in that row
		//col1
		if(outputLed[6][4-row]==0)
		{
			
		}
		else if(outputLed[6][4-row]==2)
		{
			PORTD |= (1<<PD7);
		}
		else if(everyFourth==1)
		{
			PORTD |= (1<<PD7);
		}
		//col2
		if(outputLed[5][4-row]==0)
		{
			
		}
		else if(outputLed[5][4-row]==2)
		{
			PORTD |= (1<<PD6);
		}
		else if(everyFourth==1)
		{
			PORTD |= (1<<PD6);
		}
		//col3
		if(outputLed[4][4-row]==0)
		{
			
		}
		else if(outputLed[4][4-row]==2)
		{
			PORTC |= (1<<PC5);
		}
		else if(everyFourth==1)
		{
			PORTC |= (1<<PC5);
		}
		//col4
		if(outputLed[3][4-row]==0)
		{
			
		}
		else if(outputLed[3][4-row]==2)
		{
			PORTC |= (1<<PC2);
		}
		else if(everyFourth==1)
		{
			PORTC |= (1<<PC2);
		}
		//col5
		if(outputLed[2][4-row]==0)
		{
			
		}
		else if(outputLed[2][4-row]==2)
		{
			PORTC |= (1<<PC3);
		}
		else if(everyFourth==1)
		{
			PORTC |= (1<<PC3);
		}
		//col6
		if(outputLed[1][4-row]==0)
		{
			
		}
		else if(outputLed[1][4-row]==2)
		{
			PORTD |= (1<<PD2);
		}
		else if(everyFourth==1)
		{
			PORTD |= (1<<PD2);
		}
		//col7
		if(outputLed[0][4-row]==0)
		{
			
		}
		else if(outputLed[0][4-row]==2)
		{
			PORTD |= (1<<PD1);
		}
		else if(everyFourth==1)
		{
			PORTD |= (1<<PD1);
		}
		
	}
	
	row += 1;
	if(row >= 5)
	{
		row = 0;
		everyNth += 1;
		if(everyNth == 4)
		{
			everyFourth = 1;
			everyNth = 0;
		}
		else
		{
			everyFourth = 0;
		}
	}
}

void clearLeds()
{
	for(unsigned char i=0; i<7; i++)
	{
		for(unsigned char j=0; j<5; j++)
		{
			outputLed[i][j] = 0;
		}
	}
}

void getButtons()
{
	//button 0
	if(PINB & (1<<PB0))
	{
		buttons[0] -= 1;
	}
	else
	{
		buttons[0] += 1;
	}
	//button 1
	if(PINA & (1<<PA1))
	{
		buttons[1] -= 1;
	}
	else
	{
		buttons[1] += 1;
	}
	//button 2
	if(PINB & (1<<PB6))
	{
		buttons[2] -= 1;
	}
	else
	{
		buttons[2] += 1;
	}
	//button 3
	if(PINB & (1<<PB1))
	{
		buttons[3] -= 1;
	}
	else
	{
		buttons[3] += 1;
	}
	//button 4
	if(PINA & (1<<PA3))
	{
		buttons[4] -= 1;
	}
	else
	{
		buttons[4] += 1;
	}
	//button 5
	if(PINA & (1<<PA0))
	{
		buttons[5] -= 1;
	}
	else
	{
		buttons[5] += 1;
	}
	//button 6
	if(PINB & (1<<PB2))
	{
		buttons[6] -= 1;
	}
	else
	{
		buttons[6] += 1;
	}
	//button 7
	if(PINA & (1<<PA2))
	{
		buttons[7] -= 1;
	}
	else
	{
		buttons[7] += 1;
	}
	
	for(unsigned char i=0; i<8; i++)
	{
		if(buttons[i]>20)
		{
			buttons[i] = 20;
		}
		else if(buttons[i]<0)
		{
			buttons[i] = 0;
		}
	}
	
}


unsigned char pong_posL = 2;
unsigned char pong_posR = 2;
unsigned int pong_delay = 0;

char pong_velY = 1;
char pong_velX = 1;
unsigned char pong_balX = 3;
unsigned char pong_balY = 2;

unsigned char pong_scoreL = 5;
unsigned char pong_scoreR = 5;
unsigned char pong_scoreTimer = 0;
unsigned char pong_score = 1;

void pong()
{
	pong_delay += 1;
	if(pong_delay>1000)
	{
		pong_delay = 0;
		
		if( buttons[3] | buttons[4])
		{
			menu_run = 0;
			//reset variables
			pong_scoreL = 5;
			pong_scoreR = 5;
			pong_scoreTimer = 0;
			pong_score = 1;
			pong_velY = 1;
			pong_velX = 1;
			pong_balX = 3;
			pong_balY = 2;
			pong_posL = 2;
			pong_posR = 2;
			pong_delay = 0;
		}
		
		if(pong_score == 1)
		{
			
			
			
			if(pong_scoreTimer>1)
			{
				if((pong_scoreL == 5) | (pong_scoreR == 5))
				{
					pong_scoreL = 0;
					pong_scoreR = 0;
				}
				if( buttons[0] | buttons[1] | buttons[6] | buttons[7])
				{
					//wait until button pressed to continue
					pong_score = 0;
					pong_scoreTimer = 0;
					clearLeds();
				}
			}
			else
			{
				pong_scoreTimer += 1;
				clearLeds();
				//draw scores
				unsigned char l = 0;
				while(l<pong_scoreL)
				{
					outputLed[0][l] = 1;
					outputLed[1][l] = 1;
					outputLed[2][l] = 1;
					l += 1;
				}
				unsigned char r = 0;
				while(r<pong_scoreR)
				{
					outputLed[4][r] = 1;
					outputLed[5][r] = 1;
					outputLed[6][r] = 1;
					r += 1;
				}
			}
		}
		else
		{
		
		
			//left up
			if(buttons[0]>3)
			{
				pong_posL += 1;
				if(pong_posL>=3)
				{
					pong_posL = 3;
				}
			}
			//left down
			if(buttons[6]>3)
			{
				if(pong_posL==0)
				{
					pong_posL = 0;
				}
				else
				{
					pong_posL -= 1;
				}
			}
		
			
		
		
			//right up
			if(buttons[1]>3)
			{
				pong_posR += 1;
				if(pong_posR>=3)
				{
					pong_posR = 3;
				}
			}
			//right down
			if(buttons[7]>3)
			{
				if(pong_posR==0)
				{
					pong_posR = 0;
				}
				else
				{
					pong_posR -= 1;
				}
			}
		
			
		
		
			//update ball
			//erase old ball
			outputLed[pong_balX][pong_balY] = 0;
			//update velocities
			if(pong_balY == 4)
			{
				pong_velY = -pong_velY;
			}
			if(pong_balY == 0)
			{
				pong_velY = -pong_velY;
			}
			if(pong_balX == 6)
			{
				if(pong_balY == pong_posR+1)
				{
					if(pong_velY != 1)
					{
						pong_velY += 1;
					}
				
				}
				else if(pong_balY == pong_posR)
				{
					if(pong_velY != -1)
					{
						pong_velY -= 1;
					}
				
				}
				else
				{
					pong_score = 1;
					pong_scoreL += 1;
					pong_balX = 3;
					pong_balY = 2;
					pong_velX = -pong_velX;
					return;
				}
				pong_velX = -pong_velX;
			}
			if(pong_balX == 0)
			{
				if(pong_balY == pong_posL+1)
				{
					if(pong_velY != 1)
					{
						pong_velY += 1;
					}
				
				}
				else if(pong_balY == pong_posL)
				{
					if(pong_velY != -1)
					{
						pong_velY -= 1;
					}
				
				}
				else
				{
					pong_score = 1;
					pong_scoreR += 1;
					pong_balX = 3;
					pong_balY = 2;
					pong_velX = -pong_velX;
					return;
				}
				pong_velX = -pong_velX;
			}
			//get new ball coordinates
			pong_balX = pong_balX + pong_velX;
			pong_balY = pong_balY + pong_velY;
			if(pong_balY > 4)
			{
				pong_balY = 4;
			}
			else if(pong_balY == 0)
			{
				pong_balY = 0;
			}
			if(pong_balX > 6)
			{
				pong_balY = 6;
			}
			else if(pong_balY == 0)
			{
				pong_balY = 0;
			}
		
			
			//clear old paddle areas
			outputLed[0][0] = 0;
			outputLed[0][1] = 0;
			outputLed[0][2] = 0;
			outputLed[0][3] = 0;
			outputLed[0][4] = 0;

			outputLed[6][0] = 0;
			outputLed[6][1] = 0;
			outputLed[6][2] = 0;
			outputLed[6][3] = 0;
			outputLed[6][4] = 0;
			
			//draw paddles
			outputLed[6][pong_posR] = 1;
			outputLed[6][pong_posR+1] = 1;
			outputLed[0][pong_posL] = 1;
			outputLed[0][pong_posL+1] = 1;
			//draw new ball
			outputLed[pong_balX][pong_balY] = 2;
		}
		
	}
}

/**
 * Tetris game variables.
 */
// Counter used when moving a piece
unsigned int tetrisMoveTimeout = 0;

// Counter used when rotating a piece
unsigned int tetrisRotateTimeout = 0;

// Counter used when dropping a piece
unsigned int tetrisDropTimeout = 0;

// Initial interval between piece-dropping events
unsigned int tetrisDropInterval = 2500;

// Counter used when speeding up the drop interval
unsigned int tetrisDropSpeedupTimeout = 0;

// Pixels representing blocks that have been previously dropped
unsigned char tetrisPixels[7][5];

// The pieces cycled through when spawning new pieces
unsigned char tetrisPieces[7] = {0, 1, 0, 2, 0, 0, 2};

// Info about the various shapes
unsigned char tetrisShapes[3][4][2] = {{{0, 2}, {1, 2}, {0, 3}}, {{0, 2}, {0, 3}, {0, 1}}, {{0, 2}, {0, 1}, {1, 2}, {1, 1}}};
unsigned char tetrisSizes[3] = {3, 3, 4};

// The current index into pieces
unsigned char tetrisIndex = 0;

// The indices for parts of the current piece
char tetrisCurrentPiece[4][2] = {{0, 2}, {1, 2}, {0, 3}}; // We start out with the first L piece
unsigned char tetrisCurrentPieceSize = 3;
unsigned char tetrisCurrentPieceType = 0; // 0 = L, 1 = I, 2 = S
unsigned char tetrisCurrentOrientation = 0; // Can range from 0-3, depending on the type

/**
 * Rotate current piece counterclockwise.
 */
void rotateCCW() {
    if (tetrisCurrentPieceType == 0) { // L
        if (tetrisCurrentOrientation == 0) {
            // 2
            // 0 1
            tetrisCurrentOrientation = 3;
            tetrisCurrentPiece[0][0] += 1;
            tetrisCurrentPiece[1][1] += 1;
            tetrisCurrentPiece[2][1] -= 1;
        } else if (tetrisCurrentOrientation == 1) {
            // 0 2
            // 1
            tetrisCurrentOrientation = 0;
            tetrisCurrentPiece[0][1] -= 1;
            tetrisCurrentPiece[1][0] += 1;
            tetrisCurrentPiece[2][0] -= 1;
        } else if (tetrisCurrentOrientation == 2) {
            // 1 0
            // 2
            tetrisCurrentOrientation = 1;
            tetrisCurrentPiece[0][0] -= 1;
            tetrisCurrentPiece[1][1] -= 1;
            tetrisCurrentPiece[2][1] += 1;
        } else if (tetrisCurrentOrientation == 3) {
            //   1
            // 2 0
            tetrisCurrentOrientation = 2;
            tetrisCurrentPiece[0][1] += 1;
            tetrisCurrentPiece[1][0] -= 1;
            tetrisCurrentPiece[2][0] += 1;
        }
    } else if (tetrisCurrentPieceType == 1) { // I
        if (tetrisCurrentOrientation == 0) {
            // 2
            // 0
            // 1
            tetrisCurrentOrientation = 1;
            tetrisCurrentPiece[1][0] -= 1;
            tetrisCurrentPiece[1][1] -= 1;
            tetrisCurrentPiece[2][0] += 1;
            tetrisCurrentPiece[2][1] += 1;
        } else if (tetrisCurrentOrientation == 1) {
            // 2 0 1
            tetrisCurrentOrientation = 0;
            tetrisCurrentPiece[1][0] += 1;
            tetrisCurrentPiece[1][1] += 1;
            tetrisCurrentPiece[2][0] -= 1;
            tetrisCurrentPiece[2][1] -= 1;
        }
    }
}

/**
 * Check for collisions between the current piece and the edges of the game board.
 */
unsigned char collideWithEdges() {
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        if (tetrisCurrentPiece[i][0] < 0 || tetrisCurrentPiece[i][0] >= 7) {
            return 1;
        }
        if (tetrisCurrentPiece[i][1] < 0 || tetrisCurrentPiece[i][1] >= 5) {
            return 1;
        }
    }
    return 0;
}

/**
 * Check for collisions between the current piece and the bottom of the game board.
 */
unsigned char collideWithBottom() {
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        if (tetrisCurrentPiece[i][0] >= 7) {
            // This piece has hit the bottom of the screen
            return 1;
        }
    }
    return 0;
}

/**
 * Check for collisions between the current piece and other pieces on the game board.
 */
unsigned char collideWithPixels() {
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        if (tetrisPixels[(unsigned char)(tetrisCurrentPiece[i][0])][(unsigned char)(tetrisCurrentPiece[i][1])]) {
            // This piece has hit another piece
            return 1;
        }
    }
    return 0;
}

/**
 * Checks if a particular row on the game board is completed.
 */
unsigned char completedRow(unsigned char rowToClear) {
    for (unsigned char i = 0; i < 5; i++) {
        if (!(tetrisPixels[rowToClear][i])) {
            return 0;
        }
    }
    return 1;
}

/**
 * Freeze a piece, get a new piece, and clear completed rows. To be called upon a collision.
 */
void handleCollision() {
    // Freeze the current piece on the board
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        // Move the current piece up and save it as pixels
        tetrisPixels[(unsigned char)(tetrisCurrentPiece[i][0]) - 1][(unsigned char)(tetrisCurrentPiece[i][1])] = 2;
    }

    // Generate a new Tetris piece
    tetrisIndex = (tetrisIndex + 1) % 7; // 7 available pieces to cycle through
    tetrisCurrentPieceType = tetrisPieces[tetrisIndex];
    // Initialize the new piece's coordinates
    tetrisCurrentPieceSize = tetrisSizes[tetrisCurrentPieceType];
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
    	for (unsigned char j = 0; j < 2; j++) {
    		tetrisCurrentPiece[i][j] = tetrisShapes[tetrisCurrentPieceType][i][j];
    	}
    }
    tetrisCurrentOrientation = 0;

    // If the newly spawned piece collides with an existing pixel, game over
    if (collideWithPixels()) {
        // Game over, return to menu
		menu_run = 0;

		// Reset variables
		tetrisMoveTimeout = 0;
		tetrisRotateTimeout = 0;
		tetrisDropTimeout = 0;
		tetrisDropInterval = 2500;
		tetrisDropSpeedupTimeout = 0;
		for (unsigned char i = 0; i < 7; i++) {
			for (unsigned char j = 0; j < 5; j++) {
				tetrisPixels[i][j] = 0;
			}
		}
		tetrisIndex = 0;
		tetrisCurrentPiece[0][0] = 0;
		tetrisCurrentPiece[0][1] = 2;
		tetrisCurrentPiece[1][0] = 1;
		tetrisCurrentPiece[1][1] = 2;
		tetrisCurrentPiece[2][0] = 0;
		tetrisCurrentPiece[2][1] = 3;
		tetrisCurrentPieceSize = 3;
		tetrisCurrentPieceType = 0;
		tetrisCurrentOrientation = 0;
    }

    // Clear all completed rows on the game board
    unsigned char currentRow = 6;
    while(1) {
        if (completedRow(currentRow)) {
            // Clear the row and move everything down
            for (unsigned char r = currentRow; r > 0; r--) {
                for (unsigned char c = 0; c < 5; c++) {
                    tetrisPixels[r][c] = tetrisPixels[r-1][c];
                }
            }
            // Reset currentRow so we'll check all rows again
            currentRow = 7;
        }

        if (currentRow == 0) {
            return;
        }
        currentRow -= 1;
    }
}

/**
 * The main Tetris game!
 */
void tetris()
{
    clearLeds();
    
    // Move and rotate the current piece on the game board
    tetrisMoveTimeout += 1;
    tetrisRotateTimeout += 1;
    if (tetrisMoveTimeout >= 500) {
        tetrisMoveTimeout = 0;
        // Right down (move left)
        if (buttons[7] > 3) {
            // Move the current piece to the left
            for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
                tetrisCurrentPiece[i][1] -= 1;
            }
            for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
                if (tetrisCurrentPiece[i][1] < 0 || tetrisPixels[(unsigned char)(tetrisCurrentPiece[i][0])][(unsigned char)(tetrisCurrentPiece[i][1])]) {
                    // Collision on the left, scoot everything to the right
                    for (unsigned char j = 0; j < tetrisCurrentPieceSize; j++) {
                        tetrisCurrentPiece[j][1] += 1;
                    }
                    break;
                }
            }
        }
        // Right up (move right)
        if (buttons[1] > 3) {
            // Move the current piece to the right
            for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
                tetrisCurrentPiece[i][1] += 1;
            }
            for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
                if (tetrisCurrentPiece[i][1] >= 5 || tetrisPixels[(unsigned char)(tetrisCurrentPiece[i][0])][(unsigned char)(tetrisCurrentPiece[i][1])]) {
                    // Collision on the right, scoot everything to the left
                    for (unsigned char j = 0; j < tetrisCurrentPieceSize; j++) {
                        tetrisCurrentPiece[j][1] -= 1;
                    }
                    break;
                }
            }
        }
    }
    if (tetrisRotateTimeout >= 625) {
    	tetrisRotateTimeout = 0;
        // Right left (rotate ccw)
        if (buttons[4] > 3) {
            rotateCCW();
            if (collideWithEdges() || collideWithPixels()) {
                // If we collided with something, don't allow the rotation
                rotateCCW();
                rotateCCW();
                rotateCCW();
            }
        }
        // Right right (rotate cw)
        if (buttons[5] > 3) {
            rotateCCW();
            rotateCCW();
            rotateCCW();
            if (collideWithEdges() || collideWithPixels()) {
                // If we collided with something, don't allow the rotation
                rotateCCW();
            }
        }
        // Output current piece in display
        for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
            outputLed[(unsigned char)(tetrisCurrentPiece[i][0])][(unsigned char)(tetrisCurrentPiece[i][1])] = 2;
        }
    }

    // Drop the current piece down on the game board over time
	tetrisDropTimeout += 1;
    if (tetrisDropTimeout >= tetrisDropInterval) {
    	tetrisDropTimeout = 0;
        for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
            tetrisCurrentPiece[i][0] += 1;
        }
        // Try colliding the current piece with the bottom of the screen and with other pieces
        if (collideWithBottom()) {
            handleCollision();
        } else if (collideWithPixels()) {
            handleCollision();
        }
    }

    // Draw current piece
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        outputLed[(unsigned char)(tetrisCurrentPiece[i][0])][(unsigned char)(tetrisCurrentPiece[i][1])] = 2;
    }

    // Draw pixels
    for (unsigned char i = 0; i < 7; i++) {
        for (unsigned char j = 0; j < 5; j++) {
            if (outputLed[i][j] == 0) {
                outputLed[i][j] = tetrisPixels[i][j];
            }
        }
    }

	// Speed up the game over time
    tetrisDropSpeedupTimeout += 1;
    if (tetrisDropSpeedupTimeout >= 40000) {
        tetrisDropInterval -= 125;
        if (tetrisDropInterval < 250) {
            tetrisDropInterval = 250;
        }
        tetrisDropSpeedupTimeout = 0;
    }
}

unsigned char flappy_birdY = 4;
unsigned int flappy_delay = 0;
unsigned char flappy_top[36] =    {0,0,0,0,1,0,1,0,0,0,2,0,1,0,3,0,1,0,1,0,2,0,1,0,0,0,1,0,0,0,1,0,3,0,2,0};
unsigned char flappy_bottom[36] = {0,0,0,0,2,0,2,0,1,0,2,0,1,0,0,0,1,0,2,0,1,0,2,0,1,0,1,0,2,0,1,0,0,0,1,0};
unsigned int flappy_shift = 0;
void flappy()
{
	clearLeds();
	flappy_delay += 1;
	

	
	if(flappy_delay>1000)
	{
		flappy_delay = 0;
		
		flappy_shift +=1;
		
		if((flappy_birdY<=(flappy_bottom[flappy_shift]-1)) |  (flappy_birdY>=6-flappy_top[flappy_shift])  )
		{
			flappy_shift = 0; // Reset shift
			menu_run = 0;
		}
		
		if(buttons[5]>3)
		{
			if(flappy_birdY<4) 
			{
				flappy_birdY += 1;
			}	
		}
		else if(flappy_birdY>0) 
		{
			flappy_birdY -= 1;
		}
		
	}
	
	outputLed[0][flappy_birdY] = 2;
	
	for(unsigned char i=0; i<6; i++)
	{
		for(unsigned char j=0; j<=4; j++)
		{
			if((i+flappy_shift)<=36)
			{
				if( (j+1) <= flappy_bottom[i+flappy_shift])
				{
					outputLed[i][j] = 1;
				}
				if( j >= 5-flappy_top[i+flappy_shift])
				{
					outputLed[i][j] = 1;
				}
			}
			
			//row_beginning += 1
		}
		//col_beginning += 1
	}
	
	
	/*
	outputLed[2][3] = 2;
	outputLed[4][3] = 2;
	outputLed[1][2] = 1;
	outputLed[2][1] = 1;
	outputLed[3][1] = 1;
	outputLed[4][1] = 1;
	outputLed[5][2] = 1;
	*/
}

unsigned int memory_delay = 0;
void memory()
{
    memory_delay += 1;
    if(memory_delay>1000)
    {
        clearLeds();

        memory_delay = 0;

        if( buttons[1] | buttons[4] | buttons[7])
        {
            menu_run = 0;
            //reset variables
            memory_delay = 0;
        }
    }
}

unsigned int frogger_delay = 0;
void frogger()
{
    frogger_delay += 1;
    if(frogger_delay>1000)
    {
        clearLeds();

        frogger_delay = 0;

        if( buttons[1] | buttons[4] | buttons[7])
        {
            menu_run = 0;
            //reset variables
            frogger_delay = 0;
        }
    }
}

unsigned int snake_delay = 0;
void snake()
{
    snake_delay += 1;
    if(snake_delay>1000)
    {
        clearLeds();

        snake_delay = 0;

        if( buttons[1] | buttons[4] | buttons[7])
        {
            menu_run = 0;
            //reset variables
            snake_delay = 0;
        }
    }
}

unsigned int ghero_delay = 0;
void ghero()
{
    ghero_delay += 1;
    if(ghero_delay>1000)
    {
        clearLeds();

        ghero_delay = 0;

        if( buttons[1] | buttons[4] | buttons[7])
        {
            menu_run = 0;
            //reset variables
            ghero_delay = 0;
        }
    }
}

void buttonTest()
{
	clearLeds();
	
	#define BUT 5
	
	if(buttons[0]>BUT)
	{
		outputLed[1][3] = 2;
	}
	else
	{
		outputLed[1][3] = 0;
	}
	if(buttons[1]>BUT)
	{
		outputLed[5][3] = 2;
	}
	else
	{
		outputLed[5][3] = 0;
	}
	if(buttons[2]>BUT)
	{
		outputLed[0][2] = 2;
	}
	else
	{
		outputLed[0][2] = 0;
	}
	if(buttons[3]>BUT)
	{
		outputLed[2][2] = 2;
	}
	else
	{
		outputLed[2][2] = 0;
	}
	if(buttons[4]>BUT)
	{
		outputLed[4][2] = 2;
	}
	else
	{
		outputLed[4][2] = 0;
	}
	if(buttons[5]>BUT)
	{
		outputLed[6][2] = 2;
	}
	else
	{
		outputLed[6][2] = 0;
	}
	if(buttons[6]>BUT)
	{
		outputLed[1][1] = 2;
	}
	else
	{
		outputLed[1][1] = 0;
	}
	if(buttons[7]>BUT)
	{
		outputLed[5][1] = 2;
	}
	else
	{
		outputLed[5][1] = 0;
	}

}

//unsigned char posL = 0;
//unsigned char posR = 0;
unsigned char posx = 0;
unsigned char posy = 0;
unsigned int delay = 0;
void animation()
{

	delay += 1;
	if(delay>100)
	{
		delay = 0;
		/*
		//left up
		if(buttons[0]>5)
		{
			posL += 1;
			if(posL>=5)
			{
				posL = 4;
			}
		}
		//left down
		if(buttons[2]>5)
		{
			if(posL==0)
			{
				posL = 0;
			}
			else
			{
				posL -= 1;
			}
		}
		*/
		outputLed[posx][posy] = 1;
		posx += 1;
		if(posx >= 7)
		{
			posx = 0;
			posy += 1;
			if(posy >= 5)
			{
				posy = 0;
				clearLeds();
			}
		}
		outputLed[posx][posy] = 2;
	}
	
	
	
	//clearLeds();
		
	//left side
	//outputLed[0][posL] = 2;
	//outputLed[0][posL+1] = 2;
	//right side
	//outputLed[7][posR] = 2;
	//outputLed[7][posR+1] = 2;
}

unsigned char menu_selection = 0;
unsigned int menu_delay = 0;
unsigned char menu_shift = 0;
const unsigned char text_pong[15] = { 0b00001111, 0b00000101, 0b00000111, 0b00000000, 0b00001111, 0b00001001, 0b00001111, 0b00000000, 0b00001111, 0b00000001, 0b00001111, 0b00000000, 0b00001111, 0b00001101, 0b00001101 };
const unsigned char text_tetris[24] = { 0b00000001, 0b00001111, 0b00000001, 0b00000000, 0b00001111, 0b00001011, 0b00001011, 0b00000000, 0b00000001, 0b00001111, 0b00000001, 0b00000000, 0b00001111, 0b00000111, 0b00001011, 0b00000000, 0b00001001, 0b00001111, 0b00001001, 0b00000000, 0b00001011, 0b00001011, 0b00001111 };
const unsigned char text_flappy[40] = { 0b00001111, 0b00000011, 0b00000001, 0b00000000, 0b00001111, 0b00001000, 0b00001000, 0b00000000, 0b00001111, 0b00000011, 0b00001111, 0b00000000, 0b00001111, 0b00000101, 0b00000111, 0b00000000, 0b00001111, 0b00000101, 0b00000111, 0b00000000, 0b00000011, 0b00001110, 0b00000011, 0b00000000, 0b00000000, 0b00001111, 0b00001010, 0b00001110, 0b00000000, 0b00001001, 0b00001111, 0b00001001, 0b00000000, 0b00001111, 0b00000111, 0b00001011, 0b00000000, 0b00001111, 0b00001001, 0b00000110 };
const unsigned char text_memory[23] = { 0b00001111, 0b00000010, 0b00001111, 0b00000000, 0b00001111, 0b00001011, 0b00001011, 0b00000000, 0b00001111, 0b00000010, 0b00001111, 0b00000000, 0b00001111, 0b00001001, 0b00001111, 0b00000000, 0b00001111, 0b00000111, 0b00001011, 0b00000000, 0b00000011, 0b00001110, 0b00000011 };
const unsigned char text_frogger[27] = { 0b00001111, 0b00000011, 0b00000001, 0b00000000, 0b00001111, 0b00000111, 0b00001011, 0b00000000, 0b00001111, 0b00001001, 0b00001111, 0b00000000, 0b00001111, 0b00001101, 0b00001101, 0b00000000, 0b00001111, 0b00001101, 0b00001101, 0b00000000, 0b00001111, 0b00001011, 0b00001011, 0b00000000, 0b00001111, 0b00000111, 0b00001011 };
const unsigned char text_snake[19] = { 0b00001011, 0b00001011, 0b00001111, 0b00000000, 0b00001111, 0b00000001, 0b00001111, 0b00000000, 0b00001111, 0b00000011, 0b00001111, 0b00000000, 0b00001111, 0b00000110, 0b00001001, 0b00000000, 0b00001111, 0b00001011, 0b00001011 };
const unsigned char text_ghero[40] = { 0b00001111, 0b00001101, 0b00001101, 0b00000000, 0b00001111, 0b00001000, 0b00001111, 0b00000000, 0b00001001, 0b00001111, 0b00001001, 0b00000000, 0b00000001, 0b00001111, 0b00000001, 0b00000000, 0b00001111, 0b00000011, 0b00001111, 0b00000000, 0b00001111, 0b00000111, 0b00001011, 0b00000000, 0b00000000, 0b00001111, 0b00000010, 0b00001111, 0b00000000, 0b00001111, 0b00001011, 0b00001011, 0b00000000, 0b00001111, 0b00000111, 0b00001011, 0b00000000, 0b00001111, 0b00001001, 0b00001111 };

void draw_text(const unsigned char text[], unsigned char size, unsigned char shift) {
	for(unsigned char i=0; i<7; i++)
	{
		for(unsigned char j=0; j<4; j++)
		{
			char index = (menu_shift+i-7);
			if(index<size && index>=0)
			{
				outputLed[i][j] = (text[(unsigned char)index]&(1<<(3-j)))!=0;//[j];
			}
			else
			{
				outputLed[i][j] = 0;
			}
		}
	}
	if(menu_shift < shift)
	{
		menu_shift += 1;
	}
	else
	{
		menu_shift = 0;
	}
}

void menu()
{
	if(menu_run)
	{
		if(menu_selection==0)
		{
			return pong();
		}
		else if(menu_selection==1)
		{
			return tetris();
		}
		else if(menu_selection==2)
		{
			return flappy();
		}
		else if(menu_selection==3)
		{
			return memory();
		}
		else if(menu_selection==4)
		{
			return frogger();
		}
		else if(menu_selection==5)
		{
			return snake();
		}
		else if(menu_selection==6)
		{
			return ghero();
		}
	}

	menu_delay += 1;
	if(menu_delay%600 == 0)
	{
		clearLeds();
		outputLed[menu_selection][4] = 2;
		
		//draw text
		//0 is pong
		if(menu_selection == 0)
		{
			draw_text(text_pong, 15, 30);
		}
		//1 is tetris
		else if(menu_selection == 1)
		{
			draw_text(text_tetris, 24, 40);
		}
		//2 is flappy
		else if(menu_selection == 2)
		{
			draw_text(text_flappy, 40, 50);
		}
		//3 is memory
		else if(menu_selection == 3)
		{
			draw_text(text_memory, 23, 33);
		}
		//4 is frogger
		else if(menu_selection == 4)
		{
			draw_text(text_frogger, 27, 42);
		}
		//5 is snake
		else if(menu_selection == 5)
		{
			draw_text(text_snake, 19, 35);
		}
		//6 is ghero
		else if(menu_selection == 6)
		{
			draw_text(text_ghero, 40, 50);
		}
	}
	if(menu_delay>=1000)
	{
		menu_delay = 0;
		//right button
		if(buttons[3]>3)
		{
			if(menu_selection<6)
			{
				menu_selection += 1;
				menu_shift = 0;
			}
		}
		//left button
		if(buttons[2]>3)
		{
			if(menu_selection>0)
			{
				menu_selection -= 1;
				menu_shift = 0;
			}
		}
		if(buttons[5]>3)
		{
			menu_run = 1;
		}

	}	
	
}

int main(void)
{

	DDRC = 0b10111111;
	DDRD = 0b11111111;
	PORTC = 0b00000000;
	PORTD = 0b00000000;
	
	//set up things for button inputs
	DDRA &= ~(1<<PA0);//BUT_R_R
	PORTA |= (1<<PA0);
	DDRA &= ~(1<<PA1);//BUT_R_U
	PORTA |= (1<<PA1);
	DDRA &= ~(1<<PA2);//BUT_R_D
	PORTA |= (1<<PA2);
	DDRA &= ~(1<<PA3);//BUT_R_L
	PORTA |= (1<<PA3);
	
	DDRB &= ~(1<<PB0);//PB0 BUT_L_U
	PORTB |= (1<<PB0);
	DDRB &= ~(1<<PB1);//PB1 BUT_L_R
	PORTB |= (1<<PB1);
	DDRB &= ~(1<<PB2);//PB2 BUT_L_D
	PORTB |= (1<<PB2);
	DDRB &= ~(1<<PB6);//PB6 BUT_L_L
	PORTB |= (1<<PB6);
	
	
    while(1)
    {
    	menu();
    	//animation();
    	//buttonTest();
    	//pong();
    	//clearLeds();
		
		//left side
		//outputLed[0][0] = buttons[0];
		//outputLed[0][1] = buttons[2];
    
    
		writeLeds();
		getButtons();
		
		_delay_us(100);
    }
}