/*
 * Copyright (c) 2017, Next Make
 *
 * You win if you can use your paddle to hit
 * the ball into your opponent's goal five times
 * before they are able to!
 */

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

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

/**
 * The main pong game!
 */
void pong() {
    pong_delay += 1;

    if (pong_delay > 100) {
        pong_delay = 0;

        if (pong_score == 1) {
            if (pong_scoreTimer > 5) {
                if ((pong_scoreL == 5) | (pong_scoreR == 5)) {
                    pong_scoreL = 0;
                    pong_scoreR = 0;
                }

                if (buttons[0] | buttons[1] | buttons[2] | buttons[3] |
                    buttons[4] | buttons[5] | buttons[6] | buttons[7]) {
                    pong_score = 0;
                    pong_scoreTimer = 0;
                    clear_LEDs();
                }
            } else {
                pong_scoreTimer += 1;
                clear_LEDs();

                // Draw the scores
                unsigned char l = 0;

                while (l < pong_scoreL) {
                    outputLED[0][l] = 2;
                    outputLED[1][l] = 2;
                    outputLED[2][l] = 2;
                    l += 1;
                }

                unsigned char r = 0;
                while (r < pong_scoreR) {
                    outputLED[4][r] = 2;
                    outputLED[5][r] = 2;
                    outputLED[6][r] = 2;
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

            // Draw paddles
            outputLED[0][0] = 0;
            outputLED[0][1] = 0;
            outputLED[0][2] = 0;
            outputLED[0][3] = 0;
            outputLED[0][4] = 0;

            outputLED[0][pong_posL] = 2;
            outputLED[0][pong_posL + 1] = 2;

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

            // Draw paddles
            outputLED[6][0] = 0;
            outputLED[6][1] = 0;
            outputLED[6][2] = 0;
            outputLED[6][3] = 0;
            outputLED[6][4] = 0;

            outputLED[6][pong_posR] = 2;
            outputLED[6][pong_posR + 1] = 2;

            // Update ball state and position

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

            // Draw new ball
            outputLED[pong_balX][pong_balY] = 2;
        }
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
        pong();
        write_LEDs();
        getButtons();

        _delay_ms(1);
    }
}
