/*
 * Copyright (c) 2017, Next Make
 *
 * Move and rotate falling blocks to form lines and clear the board!
 * Game over if the screen fills up.
 *
 * The game speeds up over time, see how long you can last!
 */

#include <avr/io.h>
#define F_CPU 8000000
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

unsigned char row = 0;
unsigned char everyNth = 0;
unsigned char everyFourth = 0;

/**
 * Turn on/off the appropriate LEDs using the outputLED array.
 */
void writeLeds() {
    if (correctLED) {
        // turn off all rows
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
void clearLeds() {
    for (unsigned char i = 0; i < 7; i++) {
        for (unsigned char j = 0; j < 5; j++) {
            outputLED[i][j] = 0;
        }
    }
}

/**
 * Get the buttons pressed on the board.
 */
void getButtons() {
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

/**
 * Tetris game variables.
 */
unsigned char BOARD_WIDTH = 7;
unsigned char BOARD_HEIGHT = 5;

// Counter used when moving a piece
unsigned int tetrisMoveTimeout = 0;

// Counter used when rotating a piece
unsigned int tetrisRotateTimeout = 0;

// Counter used when dropping a piece
unsigned int tetrisDropTimeout = 0;

// Initial interval between piece-dropping events
unsigned int tetrisDropInterval = 500;

// Counter used when speeding up the drop interval
unsigned int tetrisDropSpeedupTimeout = 0;

// Pixels representing blocks that have been previously dropped
unsigned char tetrisPixels[7][5];

// The pieces cycled through when spawning new pieces
unsigned char tetrisPieces[7] = {0, 1, 0, 2, 0, 0, 2};

// The current index into pieces
unsigned char tetrisIndex = 0;

// The indices for parts of the current piece
char tetrisCurrentPiece[4][2] = {
    {0, 2}, {1, 2}, {0, 3}};  // We start out with the first L piece
unsigned char tetrisCurrentPieceSize = 3;
unsigned char tetrisCurrentPieceType = 0;  // 0 = L, 1 = I, 2 = S
unsigned char tetrisCurrentOrientation =
    0;  // Can range from 0-3, depending on the type

// The number of rows cleared
unsigned char tetrisScore = 0;

// Counter used when displaying end-game info
unsigned int tetrisEndGameCounter = 0;

// Whether the game is over
unsigned char tetrisGameOver = 0;

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
 * Rotate current piece clockwise.
 */
void rotateCW() {
    if (tetrisCurrentPieceType == 0) {  // L
        if (tetrisCurrentOrientation == 0) {
            // 2
            // 0 1
            tetrisCurrentOrientation = 1;
            tetrisCurrentPiece[0][1] += 1;
            tetrisCurrentPiece[1][0] -= 1;
            tetrisCurrentPiece[2][0] += 1;
        } else if (tetrisCurrentOrientation == 1) {
            // 0 2
            // 1
            tetrisCurrentOrientation = 2;
            tetrisCurrentPiece[0][0] += 1;
            tetrisCurrentPiece[1][1] += 1;
            tetrisCurrentPiece[2][1] -= 1;
        } else if (tetrisCurrentOrientation == 2) {
            // 1 0
            // 2
            tetrisCurrentOrientation = 3;
            tetrisCurrentPiece[0][1] -= 1;
            tetrisCurrentPiece[1][0] += 1;
            tetrisCurrentPiece[2][0] -= 1;
        } else if (tetrisCurrentOrientation == 3) {
            //   1
            // 2 0
            tetrisCurrentOrientation = 0;
            tetrisCurrentPiece[0][0] -= 1;
            tetrisCurrentPiece[1][1] += 1;
            tetrisCurrentPiece[2][0] -= 1;
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
 * Freeze the current piece on the board.
 */
void convertToPixels() {
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        // Move the current piece up and save it as pixels
        tetrisPixels[(unsigned char)(tetrisCurrentPiece[i][0]) - 1]
                    [(unsigned char)(tetrisCurrentPiece[i][1])] = 2;
    }
}

/**
 * Check for collisions between the current piece and the edges of the game
 * board.
 */
unsigned char collideWithEdges() {
    for (unsigned char i = 0; i < tetrisCurrentPieceSize; i++) {
        if (tetrisCurrentPiece[i][0] < 0 ||
            tetrisCurrentPiece[i][0] >= BOARD_WIDTH) {
            return 1;
        }
        if (tetrisCurrentPiece[i][1] < 0 ||
            tetrisCurrentPiece[i][1] >= BOARD_HEIGHT) {
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
        if (tetrisCurrentPiece[i][0] >= BOARD_WIDTH) {
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
 * Checks if the game has ended.
 */
void tetrisCheckGameOver() {
    if (collideWithPixels()) {
        tetrisGameOver = 1;
    }
}

/**
 * Generate a new Tetris piece.
 */
void getNewPiece() {
    tetrisIndex = (tetrisIndex + 1) % 7;  // 7 available pieces to cycle through
    tetrisCurrentPieceType = tetrisPieces[tetrisIndex];
    // Initialize the new piece's coordinates
    if (tetrisCurrentPieceType == 0) {  // L
        tetrisCurrentPiece[0][0] = 0;
        tetrisCurrentPiece[0][1] = 2;
        tetrisCurrentPiece[1][0] = 1;
        tetrisCurrentPiece[1][1] = 2;
        tetrisCurrentPiece[2][0] = 0;
        tetrisCurrentPiece[2][1] = 3;
        tetrisCurrentPieceSize = 3;
    } else if (tetrisCurrentPieceType == 1) {  // I
        tetrisCurrentPiece[0][0] = 0;
        tetrisCurrentPiece[0][1] = 2;
        tetrisCurrentPiece[1][0] = 0;
        tetrisCurrentPiece[1][1] = 3;
        tetrisCurrentPiece[2][0] = 0;
        tetrisCurrentPiece[2][1] = 1;
        tetrisCurrentPieceSize = 3;
    } else if (tetrisCurrentPieceType == 2) {  // S
        tetrisCurrentPiece[0][0] = 0;
        tetrisCurrentPiece[0][1] = 2;
        tetrisCurrentPiece[1][0] = 0;
        tetrisCurrentPiece[1][1] = 1;
        tetrisCurrentPiece[2][0] = 1;
        tetrisCurrentPiece[2][1] = 2;
        tetrisCurrentPiece[3][0] = 1;
        tetrisCurrentPiece[3][1] = 1;
        tetrisCurrentPieceSize = 4;
    }
    tetrisCurrentOrientation = 0;

    // If the newly spawned piece collides with an existing pixel, game over
    tetrisCheckGameOver();
}

/**
 * Checks if a particular row on the game board is completed.
 */
unsigned char completedRow(unsigned char rowToClear) {
    for (unsigned char i = 0; i < BOARD_HEIGHT; i++) {
        if (!(tetrisPixels[rowToClear][i])) {
            return 0;
        }
    }
    return 1;
}

/**
 * Clear all completed rows on the game board.
 */
void clearRows() {
    unsigned char currentRow = BOARD_WIDTH - 1;

    while (1) {
        if (completedRow(currentRow)) {
            // Clear the row and move everything down
            for (unsigned char r = currentRow; r > 0; r--) {
                for (unsigned char c = 0; c < BOARD_HEIGHT; c++) {
                    tetrisPixels[r][c] = tetrisPixels[r - 1][c];
                }
            }
            tetrisScore += 1;
            // Reset currentRow so we'll check all rows again
            currentRow = BOARD_WIDTH;
        }

        if (currentRow == 0) {
            return;
        }
        currentRow -= 1;
    }
}

/**
 * Freeze a piece, get a new piece, and clear completed rows.
 * To be called upon a collision.
 */
void handleCollision() {
    convertToPixels();
    getNewPiece();
    clearRows();
}

/**
 * Move and rotate the current piece on the game board.
 */
void movePiece() {
    tetrisMoveTimeout += 1;
    tetrisRotateTimeout += 1;

    // Button layout
    //   0     1
    // 2   3 4   5
    //   6     7

    if (tetrisMoveTimeout >= 100) {
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
                if (tetrisCurrentPiece[i][1] >= BOARD_HEIGHT ||
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
    if (tetrisRotateTimeout >= 125) {
        tetrisRotateTimeout = 0;
        // Right left (rotate ccw)
        if (buttons[4] > 3) {
            rotateCCW();
            if (collideWithEdges() || collideWithPixels()) {
                // If we collided with something, don't allow the rotation
                rotateCW();
            }
        }
        // Right right (rotate cw)
        if (buttons[5] > 3) {
            rotateCW();
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
}

/**
 * Drop the current piece down on the game board over time.
 */
void dropPieces() {
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
    for (unsigned char i = 0; i < BOARD_WIDTH; i++) {
        for (unsigned char j = 0; j < BOARD_HEIGHT; j++) {
            if (outputLED[i][j] == 0) {
                outputLED[i][j] = tetrisPixels[i][j];
            }
        }
    }

    // Speed up the game over time
    tetrisDropSpeedupTimeout += 1;
    if (tetrisDropSpeedupTimeout >= 8000) {
        tetrisDropInterval -= 25;
        if (tetrisDropInterval < 50) {
            tetrisDropInterval = 50;
        }
        tetrisDropSpeedupTimeout = 0;
    }
}

// Locations of LEDs to turn on for the digits 0-9.
// Each digit is 3 LEDs wide and 5 LEDs tall.
unsigned char digits[10][5][3] = {
    {{2, 2, 2}, {2, 0, 2}, {2, 0, 2}, {2, 0, 2}, {2, 2, 2}},
    {{0, 2, 0}, {0, 2, 0}, {0, 2, 0}, {0, 2, 0}, {0, 2, 0}},
    {{2, 2, 2}, {0, 0, 2}, {2, 2, 2}, {2, 0, 0}, {2, 2, 2}},
    {{2, 2, 2}, {0, 0, 2}, {2, 2, 2}, {0, 0, 2}, {2, 2, 2}},
    {{2, 0, 2}, {2, 0, 2}, {2, 2, 2}, {0, 0, 2}, {0, 0, 2}},
    {{2, 2, 2}, {2, 0, 0}, {2, 2, 2}, {0, 0, 2}, {2, 2, 2}},
    {{2, 2, 2}, {2, 0, 0}, {2, 2, 2}, {2, 0, 2}, {2, 2, 2}},
    {{2, 2, 2}, {0, 0, 2}, {0, 0, 2}, {0, 0, 2}, {0, 0, 2}},
    {{2, 2, 2}, {2, 0, 2}, {2, 2, 2}, {2, 0, 2}, {2, 2, 2}},
    {{2, 2, 2}, {2, 0, 2}, {2, 2, 2}, {0, 0, 2}, {0, 0, 2}}};

/**
 * Writes a digit, offset by a specified amount.
 * Each digit is 3 LEDs wide.
 */
void writeDigit(unsigned char digit, unsigned char offset) {
    outputLED[0 + offset][4] = digits[digit][0][0];
    outputLED[1 + offset][4] = digits[digit][0][1];
    outputLED[2 + offset][4] = digits[digit][0][2];
    outputLED[0 + offset][3] = digits[digit][1][0];
    outputLED[1 + offset][3] = digits[digit][1][1];
    outputLED[2 + offset][3] = digits[digit][1][2];
    outputLED[0 + offset][2] = digits[digit][2][0];
    outputLED[1 + offset][2] = digits[digit][2][1];
    outputLED[2 + offset][2] = digits[digit][2][2];
    outputLED[0 + offset][1] = digits[digit][3][0];
    outputLED[1 + offset][1] = digits[digit][3][1];
    outputLED[2 + offset][1] = digits[digit][3][2];
    outputLED[0 + offset][0] = digits[digit][4][0];
    outputLED[1 + offset][0] = digits[digit][4][1];
    outputLED[2 + offset][0] = digits[digit][4][2];
}

/**
 * Shows the final score on the screen.
 */
void showScore(unsigned char score) {
    if (score > 99) {
        // We can only show 2-digit scores, so show a max of 99
        writeDigit(9, 0);
        writeDigit(9, 4);
    } else {
        unsigned char tens = score / 10;
        unsigned char ones = score % 10;
        if (tens != 0) {
            writeDigit(tens, 0);
        }
        writeDigit(ones, 4);
    }
}

/**
 * Shows the game over screen and returns to the menu.
 */
void tetrisEndGame() {
    tetrisEndGameCounter += 1;

    if (tetrisEndGameCounter >= 1000) {
        // Return to menu
        menu_run = 0;

        // Reset variables
        tetrisMoveTimeout = 0;
        tetrisRotateTimeout = 0;
        tetrisDropTimeout = 0;
        tetrisDropInterval = 500;
        tetrisDropSpeedupTimeout = 0;
        for (unsigned char i = 0; i < BOARD_WIDTH; i++) {
            for (unsigned char j = 0; j < BOARD_HEIGHT; j++) {
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
        tetrisScore = 0;
        tetrisEndGameCounter = 0;
        tetrisGameOver = 0;
    } else {
        // Report score
        showScore(tetrisScore);
    }
}

/**
 * The main Tetris game!
 */
void tetris() {
    clearLeds();

    if (!tetrisGameOver) {
        movePiece();
        dropPieces();
    } else {
        tetrisEndGame();
    }
}

int main(void) {
    DDRC = 0b10111111;
    DDRD = 0b11111111;
    PORTC = 0b00000000;
    PORTD = 0b00000000;

    // Set up things for button inputs
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
        tetris();
        writeLeds();
        getButtons();

        _delay_ms(1);
    }
}
