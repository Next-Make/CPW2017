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
unsigned char pixels[7][5];
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
 * Guitar Hero game variables.
 */
// The current state of the game; 0 = wait, 1 = play
unsigned char gameState = 0;

// The scores of the players
unsigned char winsLeft = 0;
unsigned char winsRight = 0;

// Mappings from keys to the notes onscreen
unsigned char keys[8] = {3, 3, 2, 2, 2, 2, 1, 1};

// Whether a change (a note move or a player keypress) has occurred
unsigned char transitionOccurred = 0;

// Counter used for moving song notes
unsigned int transitionDelay = 0;

// Initial value of threshold before moving notes
unsigned int delayThreshold = 10000;

// Song notes; 1 is bottom, 2 is mid, 3 is top
unsigned char song[20] = {1, 3, 2, 3, 1, 3, 1, 2, 2, 3, 1, 3, 2, 1, 1, 3, 2, 3, 2, 2};

// Progress through the song; wraps around
unsigned char songIndex = 0;

// Counter used for creating double notes
unsigned char doubleNote = 0;

/**
 * Wait for the user to decide which hand to use before starting.
 */
void paused() {
    // Draw the scores on the screen
    unsigned char row, col;
    for (row = 0; row < winsLeft; row++) {
        for (col = 0; col < 3; col++) {
            outputLED[col][row] = 2;
        }
    }
    for (row = 0; row < winsRight; row++) {
        for (col = 4; col < 7; col++) {
            outputLED[col][row] = 2;
        }
    }

    for (unsigned char i = 0; i < 8; i++) {
        if (buttons[i] > 3) {
            gameState = 1;
            clearLeds();
            transitionOccurred = 0;
            if (winsLeft >= 5 || winsRight >= 5) {
                winsLeft = 0;
                winsRight = 0;
            }
        }
    }
}

/**
 * Create and draw a new note in the song.
 */
void generateNew() {
    outputLED[3][song[songIndex]] = 2;
    pixels[3][song[songIndex]] = 2;
    songIndex = (songIndex + 1) % 20;

    doubleNote = (doubleNote + 1) % 7;
    if (doubleNote == 0) {
        // Create a double note
        outputLED[3][song[songIndex]] = 2;
        pixels[3][song[songIndex]] = 2;
        songIndex = (songIndex + 1) % 20;
    }
}

/**
 * Reset the game state for a fresh game.
 */
void resetGame() {
    gameState = 0;
    for (unsigned int i = 0; i < 10000; i++) {}
    clearLeds();
}

/**
 * Play the Guitar Hero game.
 */
void playGame() {
    unsigned char leftPress, rightPress;

    if (buttons[0] > 3)
        leftPress = 3;
    if (buttons[1] > 3)
        rightPress = 3;
    if (buttons[2] > 3 || buttons[3] > 3)
        leftPress = 2;
    if (buttons[4] > 3 || buttons[5] > 3)
        rightPress = 2;
    if (buttons[6] > 3)
        leftPress = 1;
    if (buttons[7] > 3)
        rightPress = 1;

    // Move dots
    if (transitionDelay > 10000) {
        transitionDelay = 0;
        
        // Move notes
        for (unsigned char col = 0; col < 7; col++) {
            for (unsigned char row = 1; row < 4; row++) {
                if ((pixels[col][row] % 2) == 1) {
                    outputLED[col][row] = 0;
                    pixels[col][row] = 0;
                    if (col == 3) {  // Propagate both directions
                        outputLED[col + 1][row] = 2;
                        outputLED[col - 1][row] = 2;
                        pixels[col + 1][row] = 2;
                        pixels[col - 1][row] = 2;
                    } else if (col > 3 && col < 6) {
                        outputLED[col + 1][row] = 2;
                        pixels[col + 1][row] = 2;
                    } else if (col < 3 && col > 0) {
                        outputLED[col - 1][row] = 2;
                        pixels[col - 1][row] = 2;
                    } else if (col == 0) {
                        winsRight += 1;
                        resetGame();
                    } else {
                        winsLeft += 1;
                        resetGame();
                    }
                }
            }
        }
        // for (col = 0; col < 7; col++) {
        //     for (row = 1; row < 4; row++) {
        //         if (outputLED[col][row] > 0) {
        //             outputLED[col][row] = 2;
        //         }
        //     }
        // }

        // Create and draw a new note in the song.
        outputLED[3][song[songIndex]] = 2;
        pixels[3][song[songIndex]] = 2;
        songIndex = (songIndex + 1) % 20;

        doubleNote = (doubleNote + 1) % 7;
        if (doubleNote == 0) {
            // Create a double note
            outputLED[3][song[songIndex]] = 2;
            pixels[3][song[songIndex]] = 2;
            songIndex = (songIndex + 1) % 20;
        }
        transitionOccurred = 1;
        delayThreshold -= 500;
        if (delayThreshold < 5000) {
            delayThreshold = 5000;
        }
    } else {
        transitionDelay += 1;
    }

    // Did left press the correct button at the correct time?
    if (leftPress) {
        if (pixels[0][leftPress] == 2) {
            outputLED[0][leftPress] = 0;
            pixels[0][leftPress] = 0;
        } else if (transitionOccurred) {  // Mispress
            winsRight += 1;
            resetGame();
        }
    }

    // Did right press the correct button at the correct time?
    if (rightPress) {
        if (pixels[6][rightPress] == 2) {
            outputLED[6][rightPress] = 0;
            pixels[6][rightPress] = 0;
        } else if (transitionOccurred) {  // Mispress
            winsLeft += 1;
            resetGame();
        }
    }
}

/**
 * The main Guitar Hero game!
 */
void ghero() {
    // clearLeds();

    if (gameState == 0) {
        clearLeds();
        paused();
    } else {
        playGame();
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
        ghero();
        writeLeds();
        getButtons();

        _delay_ms(1);
    }
}
