/*
 * Copyright (c) 2017, Next Make
 *
 * Main game file that will be flashed to the boards during CPW.
 * Do not add your game to this file until it works by itself!
 */

#include <avr/io.h>
#define F_CPU 8000000
#include <avr/pgmspace.h>
#include <util/delay.h>

// Threshold for button state
#define BUT 5

// If the LED matrix is in backwards
char correctLED = 0;

// Current LED output:
//
// 0 = off
// 1 = dim
// 2 = bright
unsigned char outputLED[7][5];
char buttons[8];

unsigned char menu_run = 0;

unsigned char row = 0;
unsigned char everyNth = 0;
unsigned char everyFourth = 0;

/**
 * Turn on/off the appropriate LEDs using the outputLED array.
 */
void write_LEDs() {
    if (correctLED) {
         // Turn off all rows
         DDRC |= (1 << PC1);
         DDRC |= (1 << PC2);
         DDRC |= (1 << PC3);
         DDRC |= (1 << PC4);
         DDRC |= (1 << PC5);

         PORTC |= (1 << PC1);
         PORTC |= (1 << PC2);
         PORTC |= (1 << PC3);
         PORTC |= (1 << PC4);
         PORTC |= (1 << PC5);

         // Turn on correct row
         if (row == 1) {
             PORTC &= ~(1 << PC1);
         } else if (row == 2) {
             PORTC &= ~(1 << PC2);
         } else if (row == 3) {
             PORTC &= ~(1 << PC3);
         } else if (row == 4) {
             PORTC &= ~(1 << PC4);
         } else {
             PORTC &= ~(1 << PC5);
         }

         // Turn on LEDs in that row
         for (unsigned char i = 0; i < 7; i++) {
             if (outputLED[i][row] == 0) {
                 DDRD |= (1 << (i + 1));
                 PORTD &= ~(1 << (i + 1));
             } else if (outputLED[i][row] == 1) {
                 DDRD &= ~(1 << (i + 1));
                 PORTD |= (1 << (i + 1));
             } else {
                 DDRD |= (1 << (i + 1));
                 PORTD |= (1 << (i + 1));
             }
         }
    } else {
        // Turn off all rows
        DDRC |= (1 << PC4);
        DDRD |= (1 << PD4);
        DDRD |= (1 << PD5);
        DDRC |= (1 << PC1);
        DDRD |= (1 << PD3);

        PORTC |= (1 << PC4);
        PORTD |= (1 << PD4);
        PORTD |= (1 << PD5);
        PORTC |= (1 << PC1);
        PORTD |= (1 << PD3);

        // Turn off all columns
        DDRD |= (1 << PD7);
        PORTD &= ~(1 << PD7);
        DDRD |= (1 << PD6);
        PORTD &= ~(1 << PD6);
        DDRC |= (1 << PC5);
        PORTC &= ~(1 << PC5);
        DDRC |= (1 << PC2);
        PORTC &= ~(1 << PC2);
        DDRC |= (1 << PC3);
        PORTC &= ~(1 << PC3);
        DDRD |= (1 << PD2);
        PORTD &= ~(1 << PD2);
        DDRD |= (1 << PD1);
        PORTD &= ~(1 << PD1);

        // Turn on correct row
        if (row == 0) {
            PORTC &= ~(1 << PC4);
        } else if (row == 1) {
            PORTD &= ~(1 << PD4);
        } else if (row == 2) {
            PORTD &= ~(1 << PD5);
        } else if (row == 3) {
            PORTC &= ~(1 << PC1);
        } else {
            PORTD &= ~(1 << PD3);
        }

        // Turn on LEDs in that row
        //
        // Column 1
        if (outputLED[6][4 - row] == 0) {
        } else if (outputLED[6][4 - row] == 2) {
            PORTD |= (1 << PD7);
        } else if (everyFourth == 1) {
            PORTD |= (1 << PD7);
        }

        // Column 2
        if (outputLED[5][4 - row] == 0) {
        } else if (outputLED[5][4 - row] == 2) {
            PORTD |= (1 << PD6);
        } else if (everyFourth == 1) {
            PORTD |= (1 << PD6);
        }

        // Column 3
        if (outputLED[4][4 - row] == 0) {
        } else if (outputLED[4][4 - row] == 2) {
            PORTC |= (1 << PC5);
        } else if (everyFourth == 1) {
            PORTC |= (1 << PC5);
        }

        // Column 4
        if (outputLED[3][4 - row] == 0) {
        } else if (outputLED[3][4 - row] == 2) {
            PORTC |= (1 << PC2);
        } else if (everyFourth == 1) {
            PORTC |= (1 << PC2);
        }

        // Column 5
        if (outputLED[2][4 - row] == 0) {
        } else if (outputLED[2][4 - row] == 2) {
            PORTC |= (1 << PC3);
        } else if (everyFourth == 1) {
            PORTC |= (1 << PC3);
        }

        // Column 6
        if (outputLED[1][4 - row] == 0) {
        } else if (outputLED[1][4 - row] == 2) {
            PORTD |= (1 << PD2);
        } else if (everyFourth == 1) {
            PORTD |= (1 << PD2);
        }

        // Column 7
        if (outputLED[0][4 - row] == 0) {
        } else if (outputLED[0][4 - row] == 2) {
            PORTD |= (1 << PD1);
        } else if (everyFourth == 1) {
            PORTD |= (1 << PD1);
        }
    }

    row += 1;

    if (row >= 5) {
        row = 0;
        everyNth += 1;

        if (everyNth == 4) {
            everyFourth = 1;
            everyNth = 0;
        } else {
            everyFourth = 0;
        }
    }
}

/**
 * Clear the outputLED array.
 */
void clear_LEDs() {
    for (unsigned char i = 0; i < 7; i++) {
        for (unsigned char j = 0; j < 5; j++) {
            outputLED[i][j] = 0;
        }
    }
}

/**
 * Get the buttons pressed on the board.
 */
void get_buttons() {
    // Button 0
    if (PINB & (1 << PB0)) {
        buttons[0] -= 1;
    } else {
        buttons[0] += 1;
    }

    // Button 1
    if (PINA & (1 << PA1)) {
        buttons[1] -= 1;
    } else {
        buttons[1] += 1;
    }

    // Button 2
    if (PINB & (1 << PB6)) {
        buttons[2] -= 1;
    } else {
        buttons[2] += 1;
    }

    // Button 3
    if (PINB & (1 << PB1)) {
        buttons[3] -= 1;
    } else {
        buttons[3] += 1;
    }

    // Button 4
    if (PINA & (1 << PA3)) {
        buttons[4] -= 1;
    } else {
        buttons[4] += 1;
    }

    // Button 5
    if (PINA & (1 << PA0)) {
        buttons[5] -= 1;
    } else {
        buttons[5] += 1;
    }

    // Button 6
    if (PINB & (1 << PB2)) {
        buttons[6] -= 1;
    } else {
        buttons[6] += 1;
    }

    // Button 7
    if (PINA & (1 << PA2)) {
        buttons[7] -= 1;
    } else {
        buttons[7] += 1;
    }

    for (unsigned char i = 0; i < 8; i++) {
        if (buttons[i] > 20) {
            buttons[i] = 20;
        } else if (buttons[i] < 0) {
            buttons[i] = 0;
        }
    }
}

unsigned char pong_posL = 2;
unsigned char pong_posR = 2;
unsigned int shared_delay = 0;

char pong_velY = 1;
char pong_velX = 1;
unsigned char pong_balX = 3;
unsigned char pong_balY = 2;

unsigned char pong_scoreL = 5;
unsigned char pong_scoreR = 5;
unsigned char pong_scoreTimer = 0;
unsigned char pong_score = 1;

/**
 * Main pong game!
 */
void pong() {
    shared_delay += 1;
    if (shared_delay > 1000) {
        shared_delay = 0;

        if (buttons[3] | buttons[4]) {
            menu_run = 0;

            // Reset variables
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
            shared_delay = 0;
        }

        if (pong_score == 1) {
            if (pong_scoreTimer > 1) {
                if ((pong_scoreL == 5) | (pong_scoreR == 5)) {
                    pong_scoreL = 0;
                    pong_scoreR = 0;
                }

                if (buttons[0] | buttons[1] | buttons[6] | buttons[7]) {
                    // Wait until button pressed to continue
                    pong_score = 0;
                    pong_scoreTimer = 0;
                    clear_LEDs();
                }
            } else {
                pong_scoreTimer += 1;
                clear_LEDs();

                // Draw scores
                unsigned char l = 0;
                while (l < pong_scoreL) {
                    outputLED[0][l] = 1;
                    outputLED[1][l] = 1;
                    outputLED[2][l] = 1;
                    l += 1;
                }
                unsigned char r = 0;
                while (r < pong_scoreR) {
                    outputLED[4][r] = 1;
                    outputLED[5][r] = 1;
                    outputLED[6][r] = 1;
                    r += 1;
                }
            }
        } else {
            // Left up
            if (buttons[0] > 3) {
                pong_posL += 1;
                if (pong_posL >= 3) {
                    pong_posL = 3;
                }
            }

            // Left down
            if (buttons[6] > 3) {
                if (pong_posL == 0) {
                    pong_posL = 0;
                } else {
                    pong_posL -= 1;
                }
            }

            // Right up
            if (buttons[1] > 3) {
                pong_posR += 1;
                if (pong_posR >= 3) {
                    pong_posR = 3;
                }
            }

            // Right down
            if (buttons[7] > 3) {
                if (pong_posR == 0) {
                    pong_posR = 0;
                } else {
                    pong_posR -= 1;
                }
            }

            // Update ball
            // Erase old ball
            outputLED[pong_balX][pong_balY] = 0;

            // Update velocities
            if (pong_balY == 4) {
                pong_velY = -pong_velY;
            }

            if (pong_balY == 0) {
                pong_velY = -pong_velY;
            }

            if (pong_balX == 6) {
                if (pong_balY == pong_posR + 1) {
                    if (pong_velY != 1) {
                        pong_velY += 1;
                    }
                } else if (pong_balY == pong_posR) {
                    if (pong_velY != -1) {
                        pong_velY -= 1;
                    }
                } else {
                    pong_score = 1;
                    pong_scoreL += 1;
                    pong_balX = 3;
                    pong_balY = 2;
                    pong_velX = -pong_velX;
                    return;
                }

                pong_velX = -pong_velX;
            }

            if (pong_balX == 0) {
                if (pong_balY == pong_posL + 1) {
                    if (pong_velY != 1) {
                        pong_velY += 1;
                    }
                } else if (pong_balY == pong_posL) {
                    if (pong_velY != -1) {
                        pong_velY -= 1;
                    }
                } else {
                    pong_score = 1;
                    pong_scoreR += 1;
                    pong_balX = 3;
                    pong_balY = 2;
                    pong_velX = -pong_velX;
                    return;
                }

                pong_velX = -pong_velX;
            }

            // Get new ball coordinates
            pong_balX = pong_balX + pong_velX;
            pong_balY = pong_balY + pong_velY;

            if (pong_balY > 4) {
                pong_balY = 4;
            } else if (pong_balY == 0) {
                pong_balY = 0;
            }

            if (pong_balX > 6) {
                pong_balY = 6;
            } else if (pong_balY == 0) {
                pong_balY = 0;
            }

            // Clear old paddle areas
            outputLED[0][0] = 0;
            outputLED[0][1] = 0;
            outputLED[0][2] = 0;
            outputLED[0][3] = 0;
            outputLED[0][4] = 0;

            outputLED[6][0] = 0;
            outputLED[6][1] = 0;
            outputLED[6][2] = 0;
            outputLED[6][3] = 0;
            outputLED[6][4] = 0;

            // Draw paddles
            outputLED[6][pong_posR] = 1;
            outputLED[6][pong_posR + 1] = 1;
            outputLED[0][pong_posL] = 1;
            outputLED[0][pong_posL + 1] = 1;

            // Draw new ball
            outputLED[pong_balX][pong_balY] = 2;
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
unsigned char tetrisShapes[3][4][2] = {{{0, 2}, {1, 2}, {0, 3}},
                                       {{0, 2}, {0, 3}, {0, 1}},
                                       {{0, 2}, {0, 1}, {1, 2}, {1, 1}}};
unsigned char tetrisSizes[3] = {3, 3, 4};

// The current index into pieces
unsigned char tetrisIndex = 0;

// The indices for parts of the current piece
char tetrisCurrentPiece[4][2] = {
    {0, 2}, {1, 2}, {0, 3}};  // We start out with the first L piece
unsigned char tetrisCurrentPieceSize = 3;
unsigned char tetrisCurrentPieceType = 0;  // 0 = L, 1 = I, 2 = S
unsigned char tetrisCurrentOrientation =
    0;  // Can range from 0-3, depending on the type

/**
 * Rotate current piece counterclockwise.
 */
void rotateCCW() {
    if (tetrisCurrentPieceType == 0) {  // L
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
    } else if (tetrisCurrentPieceType == 1) {  // I
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
 * Check for collisions between the current piece and the edges of the game
 * board.
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
 * Check for collisions between the current piece and the bottom of the game
 * board.
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
 * Check for collisions between the current piece and other pieces on the game
 * board.
 */
unsigned char collideWithPixels() {
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        if (tetrisPixels[(unsigned char)(tetrisCurrentPiece[i][0])]
                        [(unsigned char)(tetrisCurrentPiece[i][1])]) {
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
 * Freeze a piece, get a new piece, and clear completed rows. To be called upon
 * a collision.
 */
void handleCollision() {
    // Freeze the current piece on the board
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        // Move the current piece up and save it as pixels
        tetrisPixels[(unsigned char)(tetrisCurrentPiece[i][0]) - 1]
                    [(unsigned char)(tetrisCurrentPiece[i][1])] = 2;
    }

    // Generate a new Tetris piece
    tetrisIndex = (tetrisIndex + 1) % 7;  // 7 available pieces to cycle through
    tetrisCurrentPieceType = tetrisPieces[tetrisIndex];
    // Initialize the new piece's coordinates
    tetrisCurrentPieceSize = tetrisSizes[tetrisCurrentPieceType];
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        for (unsigned char j = 0; j < 2; j++) {
            tetrisCurrentPiece[i][j] =
                tetrisShapes[tetrisCurrentPieceType][i][j];
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
    while (1) {
        if (completedRow(currentRow)) {
            // Clear the row and move everything down
            for (unsigned char r = currentRow; r > 0; r--) {
                for (unsigned char c = 0; c < 5; c++) {
                    tetrisPixels[r][c] = tetrisPixels[r - 1][c];
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
void tetris() {
    clear_LEDs();

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
                if (tetrisCurrentPiece[i][1] < 0 ||
                    tetrisPixels[(unsigned char)(tetrisCurrentPiece[i][0])]
                                [(unsigned char)(tetrisCurrentPiece[i][1])]) {
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
                if (tetrisCurrentPiece[i][1] >= 5 ||
                    tetrisPixels[(unsigned char)(tetrisCurrentPiece[i][0])]
                                [(unsigned char)(tetrisCurrentPiece[i][1])]) {
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
            outputLED[(unsigned char)(tetrisCurrentPiece[i][0])]
                     [(unsigned char)(tetrisCurrentPiece[i][1])] = 2;
        }
    }

    // Drop the current piece down on the game board over time
    tetrisDropTimeout += 1;
    if (tetrisDropTimeout >= tetrisDropInterval) {
        tetrisDropTimeout = 0;
        for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
            tetrisCurrentPiece[i][0] += 1;
        }

        // Try colliding the current piece with the bottom of the screen and
        // with other pieces
        if (collideWithBottom()) {
            handleCollision();
        } else if (collideWithPixels()) {
            handleCollision();
        }
    }

    // Draw current piece
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        outputLED[(unsigned char)(tetrisCurrentPiece[i][0])]
                 [(unsigned char)(tetrisCurrentPiece[i][1])] = 2;
    }

    // Draw pixels
    for (unsigned char i = 0; i < 7; i++) {
        for (unsigned char j = 0; j < 5; j++) {
            if (outputLED[i][j] == 0) {
                outputLED[i][j] = tetrisPixels[i][j];
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
// unsigned int shared_delay = 0;
const unsigned char flappy_top[36] PROGMEM = {
    0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 2, 0, 1, 0, 3, 0, 1, 0,
    1, 0, 2, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 3, 0, 2, 0};
const unsigned char flappy_bottom[36] PROGMEM = {
    0, 0, 0, 0, 2, 0, 2, 0, 1, 0, 2, 0, 1, 0, 0, 0, 1, 0,
    2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 2, 0, 1, 0, 0, 0, 1, 0};
unsigned int flappy_shift = 0;

/**
 * Main flappy birds game!
 */
unsigned char flappy_holdon = 1;
void flappy() {
    clear_LEDs();
    shared_delay += 1;

    if (shared_delay > 1000) {
        shared_delay = 0;

        flappy_shift += 1;

        if (buttons[5] > 3) {
            if (flappy_birdY < 4) {
                flappy_birdY += 1;
            }
        } else if (flappy_birdY > 0) {
            flappy_birdY -= 1;
        }

        if ((flappy_birdY <=
             (pgm_read_byte(&(flappy_bottom[flappy_shift])) - 1)) |
            (flappy_birdY >=
             5 - (pgm_read_byte(&(flappy_top[flappy_shift]))))) {
            if (flappy_holdon) {
                flappy_holdon = 0;
            } else {
                flappy_shift = 0;  // Reset shift
                menu_run = 0;
                flappy_holdon = 1;
                return;
            }
        }
    }

    outputLED[0][flappy_birdY] = 2;
    if (flappy_holdon != 0) {
        for (unsigned char i = 0; i < 6; i++) {
            for (unsigned char j = 0; j <= 4; j++) {
                if ((i + flappy_shift) <= 36) {
                    if ((j + 1) <=
                        (pgm_read_byte(&(flappy_bottom[i + flappy_shift])))) {
                        outputLED[i][j] = 1;
                    }
                    if (j >=
                        5 - (pgm_read_byte(&(flappy_top[i + flappy_shift])))) {
                        outputLED[i][j] = 1;
                    }
                }
            }
        }
    } else {
        clear_LEDs();
    }
}

// Game state variables
//
// 0 = configuration (handedness)
// 1 = displaying (new sequence)
// 2 = awaiting (recreate sequence)
int GAME_STATE = 1;

// "Random" sequence of integers for rectangles in honor of Class of 2021!
//
// 0 = Top Left
// 1 = Top Right
// 2 = Bottom Left
// 3 = Bottom Right
const int SEQUENCE[21] = {0, 2, 1, 3, 3, 1, 0,
                          3, 0, 1, 2, 2, 0, 3,
                          2, 1, 1, 0, 3, 0, 0};
int INDEX = 0;

// Variables for keeping track of the
// game configuration.
const int TOP_LEFT = 0;
const int TOP_RIGHT = 1;
const int BOTTOM_LEFT = 6;
const int BOTTOM_RIGHT = 7;

const int SOFT_RESET = 2;  // Reset game only
const int HARD_RESET = 5;  // Reset game and handedness

// Variables for keeping track of where
// we are in the displaying of the sequence.
int DISPLAYING = 0;
int ITER_INDEX = 0;
int DISPLAY_DELAY = 0;

// Variables for keeping track of where
// we are in when replaying the sequence.
int REPLAY_INDEX = 0;
int REPLAY_DELAY = 0;
int DISPLAY_REPLAY = 0;

// Counter for keeping track of how to
// display that the user has replayed
// the sequence correctly.
int RIGHT_DELAY = 0;

// Counter for keeping track of how to
// display that the user has replayed
// the sequence incorrectly.
int WRONG_DELAY = 0;


/**
 * Fill the appropriate squares for the boarder in outputLED array.
 */
void fill_boarder() {
    clear_LEDs();

    outputLED[0][2] = 2;
    outputLED[1][2] = 2;
    outputLED[2][2] = 2;
    outputLED[3][0] = 2;
    outputLED[3][1] = 2;
    outputLED[3][2] = 2;
    outputLED[3][3] = 2;
    outputLED[3][4] = 2;
    outputLED[4][2] = 2;
    outputLED[5][2] = 2;
    outputLED[6][2] = 2;
}

/**
 * Draw rectangle corresponding to the sequence index.
 *
 * @param rect_index: The index corresponding to a particular rectangle
                      that will be displayed from the sequence.
 */
void draw_rectangle(int rect_index) {
    switch (rect_index) {
        case 0:
            outputLED[0][3] = 2;
            outputLED[0][4] = 2;
            outputLED[1][3] = 2;
            outputLED[1][4] = 2;
            outputLED[2][3] = 2;
            outputLED[2][4] = 2;
            break;
        case 1:
            outputLED[4][3] = 2;
            outputLED[4][4] = 2;
            outputLED[5][3] = 2;
            outputLED[5][4] = 2;
            outputLED[6][3] = 2;
            outputLED[6][4] = 2;
            break;
        case 2:
            outputLED[0][0] = 2;
            outputLED[0][1] = 2;
            outputLED[1][0] = 2;
            outputLED[1][1] = 2;
            outputLED[2][0] = 2;
            outputLED[2][1] = 2;
            break;
        case 3:
            outputLED[4][0] = 2;
            outputLED[4][1] = 2;
            outputLED[5][0] = 2;
            outputLED[5][1] = 2;
            outputLED[6][0] = 2;
            outputLED[6][1] = 2;
            break;
        default:
            // Do nothing here.
            break;
    }
}

/**
 * Wait for the next element in the sequence.
 */
void await_sequence() {
    int seq_index;
    int next_button;
    int button_pressed = -1;

    REPLAY_DELAY++;

    if (DISPLAY_REPLAY && REPLAY_DELAY < 2500) {
        return;
    } else {
        fill_boarder();
        REPLAY_DELAY = 0;
        DISPLAY_REPLAY = 0;
    }

    // We will have already incremented INDEX by the
    // time we get to here, so the inequality is strict.
    if (REPLAY_INDEX < INDEX) {
        seq_index = REPLAY_INDEX % 21;
        next_button = SEQUENCE[seq_index];

        if (buttons[SOFT_RESET] > BUT) {
            INDEX = 0;
            GAME_STATE = 1;
            REPLAY_INDEX = 0;
            REPLAY_DELAY = 0;
            DISPLAY_REPLAY = 0;

            return;
        } else if (buttons[HARD_RESET] > BUT) {
            INDEX = 0;
            GAME_STATE = 1;
            REPLAY_INDEX = 0;
            REPLAY_DELAY = 0;
            DISPLAY_REPLAY = 0;

            menu_run = 0;
            return;
        }

        if (buttons[TOP_LEFT] > BUT) {
            button_pressed = 0;
        } else if (buttons[TOP_RIGHT] > BUT) {
            button_pressed = 1;
        } else if (buttons[BOTTOM_LEFT] > BUT) {
            button_pressed = 2;
        } else if (buttons[BOTTOM_RIGHT] > BUT) {
            button_pressed = 3;
        }

        if (button_pressed > -1) {
            draw_rectangle(button_pressed);

            if (button_pressed == next_button) {
                REPLAY_INDEX++;
                DISPLAY_REPLAY = 1;
            } else {
                draw_rectangle(next_button);
                INDEX = 0;
                GAME_STATE = 4;
                REPLAY_INDEX = 0;
                REPLAY_DELAY = 0;
                DISPLAY_REPLAY = 0;
            }
        }
    } else {
        GAME_STATE = 3;
        REPLAY_DELAY = 0;
        REPLAY_INDEX = 0;
        DISPLAY_REPLAY = 0;
    }
}

/**
 * Generate the memory sequence and display.
 */
void generate_sequence() {
    int seq_index;

    if (ITER_INDEX <= INDEX) {
        if (DISPLAY_DELAY > 5000) {
            DISPLAY_DELAY = 0;
            fill_boarder();

            if (DISPLAYING) {
                ITER_INDEX++;
            } else {
                seq_index = ITER_INDEX % 21;
                draw_rectangle(SEQUENCE[seq_index]);
            }

            DISPLAYING = ~DISPLAYING;
        } else {
            DISPLAY_DELAY++;
        }
    } else {
        INDEX++;
        ITER_INDEX = 0;

        DISPLAYING = 0;
        DISPLAY_DELAY = 0;

        GAME_STATE = 2;
    }
}

/**
 * Indicate that the user has replayed the sequence correctly.
 */
void display_correct() {
    if (RIGHT_DELAY > 5400) {
        GAME_STATE = 1;
        RIGHT_DELAY = 0;
    } else {
        if (RIGHT_DELAY > 4500) {
            fill_boarder();
        } else if (RIGHT_DELAY > 1500) {
            clear_LEDs();

            outputLED[1][1] = 2;
            outputLED[2][0] = 2;
            outputLED[3][1] = 2;
            outputLED[4][2] = 2;
            outputLED[5][3] = 2;
            outputLED[6][4] = 2;
        }

        RIGHT_DELAY++;
    }
}

/**
 * Indicate that the user has replayed the sequence incorrectly.
 */
void display_wrong() {
    if (WRONG_DELAY > 5900) {
        GAME_STATE = 1;
        WRONG_DELAY = 0;
    } else {
        if (WRONG_DELAY > 5000) {
            fill_boarder();
        } else if (WRONG_DELAY > 3000) {
            clear_LEDs();

            outputLED[1][0] = 2;
            outputLED[1][4] = 2;
            outputLED[2][1] = 2;
            outputLED[2][3] = 2;
            outputLED[3][2] = 2;
            outputLED[4][1] = 2;
            outputLED[4][3] = 2;
            outputLED[5][0] = 2;
            outputLED[5][4] = 2;
        } else if (WRONG_DELAY > 1800) {
            // We include this delay because
            // we won't have turned off the
            // rectangle pressed previously
            // in await_sequence().
            fill_boarder();
        }

        WRONG_DELAY++;
    }
}

/**
 * Main memory game!
 */
void memory() {
    switch (GAME_STATE) {
        case 1:
            generate_sequence();
            break;
        case 2:
            await_sequence();
            break;
        case 3:
            display_correct();
            break;
        case 4:
            display_wrong();
            break;
        default:
            // Do nothing here.
            break;
    }
}

/**
 * Frogger game variables.
 */
// Current frog position
unsigned char frogX = 3;
// frogY is always 1, we scroll the screen instead of moving the frog

// Counter used for blinking the frog
unsigned char blink = 0;

// Counter used for moving the frog and trucks
unsigned char timeout = 0;

// The trucks in the level are in froggerLevel:
// Within each row, each truck has the same speed and same length.
// The speed of a row is equal to the number of spaces between trucks.
// Within each sublist, the first element is the length of the trucks in that
// row (2-4).
// The second element is the velocity (-2, -1, 1, or 2) with positive velocity
// meaning moving to the right.
// The third element is the truck index, which indicates the portion of the
// first truck in a row that is still off the screen.
// A sublist with all 0s represents an empty row.
char froggerLevel[37][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {2, -2, 0}, {3, 1, 0}, {2, -1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {1, 2, 0}, {4, -1, 0}, {1, 2, 0},{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {2, -2, 0}, {3, 1, 0}, {2, -1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {1, 2, 0}, {1, -2, 0}, {1, 2, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {2, -2, 0}, {3, 1, 0}, {2, -1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {1, 2, 0}, {4, -2, 0}, {1, 2, 0}, {0, 0, 0}};

// Counter used for moving trucks
unsigned int truckCounter = 0;

// Variable to make speed 1 trucks move on every other cycle
unsigned char truckPhase = 0;

// The beginning of the window of rows currently displayed on the screen
unsigned char rowIndex = 0;

/**
 * Resets variables and returns to the menu.
 */
void froggerEndGame() {
    // Return to the menu
    menu_run = 0;
    rowIndex = 0;
    frogX = 3;
}

/**
 * Check if the frog has collided with a truck.
 */
void froggerCheckCollision() {
    unsigned char currentRow = rowIndex + 1;
    if (currentRow < 37) {
        unsigned char index = froggerLevel[currentRow][2];
        if (froggerLevel[currentRow][1] > 0) {
            // Moving to the right
            for (unsigned char col = 0; col < 7; col++) {
                if (index >= froggerLevel[currentRow][1]) {
                    // A truck exists here
                    if (col == frogX) {
                        // Collision!
                        froggerEndGame();
                    }
                }
                index = (index + 1) % (froggerLevel[currentRow][0] + froggerLevel[currentRow][1]);
            }
        } else if (froggerLevel[currentRow][1] < 0) {
            // Moving to the left
            for (unsigned char col = 0; col < 7; col++) {
                if (index < froggerLevel[currentRow][1]) {
                    // A truck exists here
                    if (col == frogX) {
                        // Collision!
                        froggerEndGame();
                    }
                }
                index = (index - 1) % (froggerLevel[currentRow][0] - froggerLevel[currentRow][1]);
            }
        }
    }
}

/**
 * Move the frog on the screen.
 * The screen scrolls up and down with the frog to show more of the froggerLevel.
 */
void updateFrog() {
    // Up
    if (buttons[1] > 3) {
        rowIndex += 1;
    }
    // Down
    if (buttons[7] > 3 && rowIndex > 0) {
        rowIndex -= 1;
    }
    // Left
    if (buttons[4] > 3 && frogX > 0) {
        frogX -= 1;
    }
    // Right
    if (buttons[5] > 3 && frogX < 6) {
        frogX += 1;
    }
    timeout = 0;

    froggerCheckCollision();
    if (rowIndex == 37) {
        // Victory!
        froggerEndGame();
    }
}

/**
 * Draw the frog on the screen.
 */
void drawFrog() {
    if (blink <= 70) {
        outputLED[frogX][1] = 2;
    } else if (blink <= 140) {
        outputLED[frogX][1] = 0;
    } else {
        blink = 0;
    }
}

/**
 * Update the position of the trucks on the screen.
 */
void updateTrucks() {
    truckCounter += 1;

    if (truckCounter >= 400) {
        truckCounter = 0;
        truckPhase = (truckPhase + 1) % 2;

        for (unsigned char row = 0; row < 37; row++) {
            // Update truck indices
            if (froggerLevel[row][1] == 2 || ((froggerLevel[row][1] == 1) && truckPhase)) {
                froggerLevel[row][2] = (froggerLevel[row][2] - 1) % (froggerLevel[row][0] + froggerLevel[row][1]);
            } else if (froggerLevel[row][1] == -2 || ((froggerLevel[row][1] == -1) && truckPhase)) {
                froggerLevel[row][2] = (froggerLevel[row][2] - 1) % (froggerLevel[row][0] - froggerLevel[row][1]);
            }
        }

        froggerCheckCollision();
    }
}

/**
 * Draw the trucks on the screen.
 */
void drawTrucks() {
    unsigned char maxRow = rowIndex + 5;
    if (maxRow > 37) {
        maxRow = 37;
    }
    // Draw the current trucks
    for (unsigned char row = rowIndex; row < maxRow; row++) {
        unsigned char index = froggerLevel[row][2];
        if (froggerLevel[row][1] > 0) {
            // Trucks moving to the right
            for (unsigned char col = 0; col < 7; col++) {
                if (index >= froggerLevel[row][1]) {
                    // Drawing a truck
                    outputLED[col][row - rowIndex] = 2;
                }
                index = (index + 1) % (froggerLevel[row][0] + froggerLevel[row][1]);
            }
        } else if (froggerLevel[row][1] < 0) {
            // Trucks moving to the left
            for (unsigned char col = 0; col < 7; col++) {
                if (index < -froggerLevel[row][1]) {
                    // Drawing a truck
                    outputLED[col][row - rowIndex] = 2;
                }
                index = (index - 1) % (froggerLevel[row][0] - froggerLevel[row][1]);
            }
        }
    }
}

/**
 * The main Frogger game!
 */
void frogger() {
    clear_LEDs();

    drawFrog();
    drawTrucks();
    if (timeout >= 75) {
        updateFrog();
        updateTrucks();
    } else {
        timeout += 1;
        blink += 1;
        truckCounter += 1;
    }
}

/**
 * Main guitar hero game!
 */
void ghero() {
    shared_delay += 1;
    if (shared_delay > 1000) {
        clear_LEDs();

        shared_delay = 0;

        if (buttons[1] | buttons[4] | buttons[7]) {
            menu_run = 0;
            // reset variables
            shared_delay = 0;
        }
    }
}

unsigned char menu_selection = 0;
unsigned int menu_delay = 0;
unsigned char menu_shift = 0;

const unsigned char text_pong[15] PROGMEM = {
    0b00001111, 0b00000101, 0b00000111, 0b00000000, 0b00001111,
    0b00001001, 0b00001111, 0b00000000, 0b00001111, 0b00000001,
    0b00001111, 0b00000000, 0b00001111, 0b00001101, 0b00001101};
const unsigned char text_tetris[24] PROGMEM = {
    0b00000001, 0b00001111, 0b00000001, 0b00000000, 0b00001111, 0b00001011,
    0b00001011, 0b00000000, 0b00000001, 0b00001111, 0b00000001, 0b00000000,
    0b00001111, 0b00000111, 0b00001011, 0b00000000, 0b00001001, 0b00001111,
    0b00001001, 0b00000000, 0b00001011, 0b00001011, 0b00001111};
const unsigned char text_flappy[40] PROGMEM = {
    0b00001111, 0b00000011, 0b00000001, 0b00000000, 0b00001111, 0b00001000,
    0b00001000, 0b00000000, 0b00001111, 0b00000011, 0b00001111, 0b00000000,
    0b00001111, 0b00000101, 0b00000111, 0b00000000, 0b00001111, 0b00000101,
    0b00000111, 0b00000000, 0b00000011, 0b00001110, 0b00000011, 0b00000000,
    0b00000000, 0b00001111, 0b00001010, 0b00001110, 0b00000000, 0b00001001,
    0b00001111, 0b00001001, 0b00000000, 0b00001111, 0b00000111, 0b00001011,
    0b00000000, 0b00001111, 0b00001001, 0b00000110};
const unsigned char text_memory[23] PROGMEM = {
    0b00001111, 0b00000010, 0b00001111, 0b00000000, 0b00001111, 0b00001011,
    0b00001011, 0b00000000, 0b00001111, 0b00000010, 0b00001111, 0b00000000,
    0b00001111, 0b00001001, 0b00001111, 0b00000000, 0b00001111, 0b00000111,
    0b00001011, 0b00000000, 0b00000011, 0b00001110, 0b00000011};
const unsigned char text_frogger[27] PROGMEM = {
    0b00001111, 0b00000011, 0b00000001, 0b00000000, 0b00001111, 0b00000111,
    0b00001011, 0b00000000, 0b00001111, 0b00001001, 0b00001111, 0b00000000,
    0b00001111, 0b00001101, 0b00001101, 0b00000000, 0b00001111, 0b00001101,
    0b00001101, 0b00000000, 0b00001111, 0b00001011, 0b00001011, 0b00000000,
    0b00001111, 0b00000111, 0b00001011};
const unsigned char text_ghero[40] PROGMEM = {
    0b00001111, 0b00001101, 0b00001101, 0b00000000, 0b00001111, 0b00001000,
    0b00001111, 0b00000000, 0b00001001, 0b00001111, 0b00001001, 0b00000000,
    0b00000001, 0b00001111, 0b00000001, 0b00000000, 0b00001111, 0b00000011,
    0b00001111, 0b00000000, 0b00001111, 0b00000111, 0b00001011, 0b00000000,
    0b00000000, 0b00001111, 0b00000010, 0b00001111, 0b00000000, 0b00001111,
    0b00001011, 0b00001011, 0b00000000, 0b00001111, 0b00000111, 0b00001011,
    0b00000000, 0b00001111, 0b00001001, 0b00001111};

/**
 * Draw text displaying which game is selected.
 *
 * @param text : Text to display
 * @param size : The length of the text array
 * @param shift : The amount at which the text should be shifted
 */
void draw_text(const unsigned char text[], unsigned char size,
               unsigned char shift) {
    for (unsigned char i = 0; i < 7; i++) {
        for (unsigned char j = 0; j < 4; j++) {
            char index = (menu_shift + i - 7);

            if (index < size && index >= 0) {
                unsigned char text_index = (unsigned char) index;

                outputLED[i][j] =
                    (pgm_read_byte(&(text[text_index])) &
                     (1 << (3 - j))) != 0;  // [j];
            } else {
                outputLED[i][j] = 0;
            }
        }
    }

    if (menu_shift < shift) {
        menu_shift += 1;
    } else {
        menu_shift = 0;
    }
}

/**
 * Load the game menu for the user.
 */
void menu() {
    if (menu_run) {
        if (menu_selection == 0) {
            return pong();
        } else if (menu_selection == 1) {
            return tetris();
        } else if (menu_selection == 2) {
            return flappy();
        } else if (menu_selection == 3) {
            return memory();
        } else if (menu_selection == 4) {
            return frogger();
        } else if (menu_selection == 5) {
            return ghero();
        }
    }

    menu_delay += 1;
    if (menu_delay % 600 == 0) {
        clear_LEDs();
        outputLED[menu_selection][4] = 2;

        // Draw text
        if (menu_selection == 0) {
            // 0 is Pong
            draw_text(text_pong, 15, 30);
        } else if (menu_selection == 1) {
            // 1 is Tetris
            draw_text(text_tetris, 24, 40);
        } else if (menu_selection == 2) {
            // 2 is Flappy
            draw_text(text_flappy, 40, 50);
        } else if (menu_selection == 3) {
            // 3 is memory
            draw_text(text_memory, 23, 33);
        } else if (menu_selection == 4) {
            // 4 is Frogger
            draw_text(text_frogger, 27, 42);
        } else if (menu_selection == 5) {
            // 5 is Guitar Hero
            draw_text(text_ghero, 40, 50);
        }
    }

    if (menu_delay >= 1000) {
        menu_delay = 0;

        // Right button
        if (buttons[3] > 3) {
            menu_shift = 0;
            menu_selection += 1;
            menu_selection %= 6;
        }

        // Left button
        if (buttons[2] > 3) {
            menu_shift = 0;

            if (menu_selection == 0) {
                menu_selection = 5;
            } else {
                menu_selection -= 1;
            }
        }

        if (buttons[5] > 3) {
            menu_run = 1;
        }
    }
}

int main(void) {
    DDRC = 0b10111111;
    DDRD = 0b11111111;
    PORTC = 0b00000000;
    PORTD = 0b00000000;

    // set up things for button inputs
    DDRA &= ~(1 << PA0);  // BUT_R_R
    PORTA |= (1 << PA0);
    DDRA &= ~(1 << PA1);  // BUT_R_U
    PORTA |= (1 << PA1);
    DDRA &= ~(1 << PA2);  // BUT_R_D
    PORTA |= (1 << PA2);
    DDRA &= ~(1 << PA3);  // BUT_R_L
    PORTA |= (1 << PA3);

    DDRB &= ~(1 << PB0);  // PB0 BUT_L_U
    PORTB |= (1 << PB0);
    DDRB &= ~(1 << PB1);  // PB1 BUT_L_R
    PORTB |= (1 << PB1);
    DDRB &= ~(1 << PB2);  // PB2 BUT_L_D
    PORTB |= (1 << PB2);
    DDRB &= ~(1 << PB6);  // PB6 BUT_L_L
    PORTB |= (1 << PB6);

    while (1) {
        menu();
        write_LEDs();
        get_buttons();

        _delay_us(100);
    }
}
