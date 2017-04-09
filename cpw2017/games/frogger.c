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
                if (index < -froggerLevel[currentRow][1]) {
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
    clearLeds();

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
        frogger();
        writeLeds();
        getButtons();

        _delay_ms(1);
    }
}
