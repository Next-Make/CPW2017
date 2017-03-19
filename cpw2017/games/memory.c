/*
 * Copyright (c) 2017, Next Make
 *
 * How well can you remember a sequence containing only four unique rectangles?
 */

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

// Threshold for button state
#define BUT 3

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

// Game state variables
//
// 0 = configuration (handedness)
// 1 = displaying (new sequence)
// 2 = awaiting (recreate sequence)
int GAME_STATE = 0;

// "Random" sequence of integers for rectangles in honor of Class of 2021!
//
// 0 = Top Left
// 1 = Top Right
// 2 = Bottom Left
// 3 = Bottom Right
int SEQUENCE[21] = {0, 2, 1, 3, 3, 1, 0,
                    3, 0, 1, 2, 2, 0, 3,
                    2, 1, 1, 0, 3, 0, 0};
int INDEX = 0;

// Variables for keeping track of the
// game configuration.
//
// 0 = Left-hand
// 1 = Right-hand
int AWAIT_DELAY = 0;
int HANDEDNESS = -1;

int TOP_LEFT = -1;
int TOP_RIGHT = -1;
int BOTTOM_LEFT = -1;
int BOTTOM_RIGHT = -1;

int SOFT_RESET = -1;  // Reset game only
int HARD_RESET = -1;  // Reset game and handedness

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
 * Wait for the user to configure which hand
 * they want to use for the game.
 */
void await_configuration() {
    fill_boarder();

    if (HANDEDNESS > -1) {
        if (AWAIT_DELAY > 200) {
            AWAIT_DELAY = 0;
            GAME_STATE = 1;
        } else {
            AWAIT_DELAY++;
        }

        return;
    }

    if (buttons[0] > BUT || buttons[2] > BUT ||
        buttons[3] > BUT || buttons[6] > BUT) {
        HANDEDNESS = 0;

        TOP_LEFT = 0;
        TOP_RIGHT = 3;
        BOTTOM_LEFT = 2;
        BOTTOM_RIGHT = 6;

        SOFT_RESET = 1;
        HARD_RESET = 7;
    } else if (buttons[1] > BUT || buttons[4] > BUT ||
               buttons[5] > BUT || buttons[7] > BUT) {
        HANDEDNESS = 1;

        TOP_LEFT = 4;
        TOP_RIGHT = 1;
        BOTTOM_LEFT = 7;
        BOTTOM_RIGHT = 5;

        SOFT_RESET = 0;
        HARD_RESET = 6;
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

    if ((HANDEDNESS == -1 || DISPLAY_REPLAY) && REPLAY_DELAY < 1000) {
        return;
    } else {
        fill_boarder();
        REPLAY_DELAY = 0;
        DISPLAY_REPLAY = 0;

        if (HANDEDNESS == -1) {
            GAME_STATE = 0;
            return;
        }
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
            HANDEDNESS = -1;
            REPLAY_INDEX = 0;
            REPLAY_DELAY = 0;
            DISPLAY_REPLAY = 0;

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
        if (DISPLAY_DELAY > 1000) {
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
    if (RIGHT_DELAY > 1800) {
        GAME_STATE = 1;
        RIGHT_DELAY = 0;
    } else {
        if (RIGHT_DELAY > 1500) {
            fill_boarder();
        } else if (RIGHT_DELAY > 500) {
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
    if (WRONG_DELAY > 2300) {
        GAME_STATE = 1;
        WRONG_DELAY = 0;
    } else {
        if (WRONG_DELAY > 2000) {
            fill_boarder();
        } else if (WRONG_DELAY > 1200) {
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
        } else if (WRONG_DELAY > 600) {
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
 * Main memory program
 */
void memory() {
    switch (GAME_STATE) {
        case 0:
            await_configuration();
            break;
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
        memory();

        write_LEDs();
        get_buttons();

        _delay_ms(1);
    }
}
