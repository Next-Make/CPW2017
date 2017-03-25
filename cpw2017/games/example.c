/*
 * Copyright (c) 2017, Next Make
 *
 * Example game that you can use as a template for your own games!
 *
 * This code contains a lot of useful functions that you can use
 * for coming up with ideas for your own games, such as how you
 * move characters around.
 *
 * Some important information regarding implementation:
 *
 * 1) The only method you should be modifying is buttonTest().
 *    The code is written specifically to work with the board as is.
 *    Attempts to mess with any of the other functions could throw off
 *    the hardware and lead to unexpected results.
 *
 * 2) Alternatively, you can implement your method (with a different
 *    name instead of buttonTest). However, make sure that you do not
 *    call any methods like "write_LEDs", "getButtons", or "_delay_ms."
 *
 *    If you need to implement some kind of delay in your method, you
 *    can create variables that act as counters in which you exit immediately
 *    out of your function if your counter doesn't reach a certain number:
 *
 *    int COUNTER = 0;
 *
 *    void your_function() {
 *        COUNTER++;
 *
 *        if (COUNTER < 1000) {
 *            // Or some other cutoff value
 *            return;
 *        }
 *
 *        COUNTER = 0;  // reset and continue...
 *        <your code here continues>
 *    }
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
        switch (row) {
            case 1:
                PORTC &= ~(1 << PC1);
                break;
            case 2:
                PORTC &= ~(1 << PC2);
                break;
            case 3:
                PORTC &= ~(1 << PC3);
                break;
            case 4:
                PORTC &= ~(1 << PC4);
                break;
            default:  // row == 5
                PORTC &= ~(1 << PC5);
                break;
        }

        // Turn on LEDs in that row
        for (unsigned char i = 0; i < 7; i++) {
            switch (outputLED[i][row]) {
                case 0:
                    DDRD |= (1 << (i + 1));
                    PORTD &= ~(1 << (i + 1));
                    break;
                case 1:
                    DDRD &= ~(1 << (i + 1));
                    PORTD |= (1 << (i + 1));
                    break;
                default:  // Brightness is 2
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

        // Turn off columns
        DDRD |= (1 << PD7);
        DDRD |= (1 << PD6);
        DDRC |= (1 << PC5);
        DDRC |= (1 << PC2);
        DDRC |= (1 << PC3);
        DDRD |= (1 << PD2);
        DDRD |= (1 << PD1);

        PORTD &= ~(1 << PD7);
        PORTD &= ~(1 << PD6);
        PORTC &= ~(1 << PC5);
        PORTC &= ~(1 << PC2);
        PORTC &= ~(1 << PC3);
        PORTD &= ~(1 << PD2);
        PORTD &= ~(1 << PD1);

        switch (row) {
            case 0:
                PORTC &= ~(1 << PC4);
                break;
            case 1:
                PORTD &= ~(1 << PD4);
                break;
            case 2:
                PORTD &= ~(1 << PD5);
                break;
            case 3:
                PORTC &= ~(1 << PC1);
                break;
            default:
                PORTD &= ~(1 << PD3);
                break;
        }

        // Turn on LEDs in that row
        //
        // Column 1
        if (outputLED[6][4 - row] == 1) {
            DDRD &= ~(1 << PD7);
            PORTD |= (1 << PD7);
        } else if (outputLED[6][4 - row] != 0) {
            DDRD |= (1 << PD7);
            PORTD |= (1 << PD7);
        }

        // Column 2
        if (outputLED[5][4 - row] == 1) {
            DDRD &= ~(1 << PD6);
            PORTD |= (1 << PD6);
        } else if (outputLED[5][4 - row] != 0) {
            DDRD |= (1 << PD6);
            PORTD |= (1 << PD6);
        }

        // Column 3
        if (outputLED[4][4 - row] == 1) {
            DDRC &= ~(1 << PC5);
            PORTC |= (1 << PC5);
        } else if (outputLED[4][4 - row] != 0) {
            DDRC |= (1 << PC5);
            PORTC |= (1 << PC5);
        }

        // Column 4
        if (outputLED[3][4 - row] == 1) {
            DDRC &= ~(1 << PC2);
            PORTC |= (1 << PC2);
        } else if (outputLED[3][4 - row] != 0) {
            DDRC |= (1 << PC2);
            PORTC |= (1 << PC2);
        }

        // Column 5
        if (outputLED[2][4 - row] == 1) {
            DDRC &= ~(1 << PC3);
            PORTC |= (1 << PC3);
        } else if (outputLED[2][4 - row] != 0) {
            DDRC |= (1 << PC3);
            PORTC |= (1 << PC3);
        }

        // Column 6
        if (outputLED[1][4 - row] == 1) {
            DDRD &= ~(1 << PD2);
            PORTD |= (1 << PD2);
        } else if (outputLED[1][4 - row] != 0) {
            DDRD |= (1 << PD2);
            PORTD |= (1 << PD2);
        }

        // Column 7
        if (outputLED[0][4 - row] == 1) {
            DDRD &= ~(1 << PD1);
            PORTD |= (1 << PD1);
        } else if (outputLED[0][4 - row] != 0) {
            DDRD |= (1 << PD1);
            PORTD |= (1 << PD1);
        }
    }

    row += 1;

    if (row >= 5) {
        row = 0;
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
 * Test program to make sure that we're responding to all relevant keys.
 */
void buttonTest() {
    clear_LEDs();

    if (buttons[0] > BUT) {
        outputLED[1][3] = 2;
    } else {
        outputLED[1][3] = 0;
    }

    if (buttons[1] > BUT) {
        outputLED[5][3] = 2;
    } else {
        outputLED[5][3] = 0;
    }

    if (buttons[2] > BUT) {
        outputLED[0][2] = 2;
    } else {
        outputLED[0][2] = 0;
    }

    if (buttons[3] > BUT) {
        outputLED[2][2] = 2;
    } else {
        outputLED[2][2] = 0;
    }

    if (buttons[4] > BUT) {
        outputLED[4][2] = 2;
    } else {
        outputLED[4][2] = 0;
    }

    if (buttons[5] > BUT) {
        outputLED[6][2] = 2;
    } else {
        outputLED[6][2] = 0;
    }

    if (buttons[6] > BUT) {
        outputLED[1][1] = 2;
    } else {
        outputLED[1][1] = 0;
    }

    if (buttons[7] > BUT) {
        outputLED[5][1] = 2;
    } else {
        outputLED[5][1] = 0;
    }
}

unsigned char posx = 0;
unsigned char posy = 0;
unsigned int delay = 0;

/**
 * Move a character on the game board.
 */
void animation() {
    delay += 1;

    if (delay > 100) {
        delay = 0;

        outputLED[posx][posy] = 1;
        posx += 1;

        if (posx >= 7) {
            posx = 0;
            posy += 1;

            if (posy >= 5) {
                posy = 0;
                clear_LEDs();
            }
        }

        outputLED[posx][posy] = 2;
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
        buttonTest();
        write_LEDs();
        getButtons();

        _delay_ms(1);
    }
}
