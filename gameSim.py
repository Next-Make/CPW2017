"""
Next Make CPW 2017 Game Simulator

This is where you will also write the code
to develop the game you so desire! :)
"""

import sys
import time
import pygame
from pygame.locals import *

# EDIT THIS SECTION OF CODE AT YOUR OWN RISK.
pygame.init()
screen = pygame.display.set_mode((550, 400))
pygame.display.set_caption('Pygame Keyboard Test')
pygame.mouse.set_visible(0)

WHITE = 255, 255, 255
GREEN = 0, 255, 0
BLACK = 0, 0, 0
BLUE = 0, 0, 255
RED = 255, 0, 0

# Array for keeping tracked of relevant
# game keys and whether they have been
# pressed or not.
#
# 0 = Key Not Pressed
# 1 = Key Pressed
KEYS = [0, 0, 0, 0, 0, 0, 0, 0]

KEY_W = 0
KEY_A = 1
KEY_S = 2
KEY_D = 3
KEY_UP = 4
KEY_DOWN = 5
KEY_LEFT = 6
KEY_RIGHT = 7

# The screen is a 7x5 array and is the only item
# that should be modified during your game.
SCREEN_BUFFER = [[0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0],
                 [0, 0, 0, 0, 0]]


def clear_buffer():
    """
    Clear the screen buffer.
    """
    global SCREEN_BUFFER

    for x in range(7):
        for y in range(5):
            SCREEN_BUFFER[x][y] = 0


DISP_COUNTER = 0


def update_simulator():
    """
    Update the simulator, including the display and keys pressed.
    """
    global DISP_COUNTER

    def draw_screen():
        """
        Redraw the screen using the updated screen buffer.
        """
        global SCREEN_BUFFER

        for x in range(7):
            for y in range(5):
                color = RED
                radius = 30
                pos = 50 + 75*x, 350 - 75*y

                if SCREEN_BUFFER[x][y] == 0:  # unfilled --> filled
                    pygame.draw.circle(screen, color, pos, radius, 3)
                else:  # filled --> unfilled
                    pygame.draw.circle(screen, color, pos, radius, 0)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == KEYDOWN and event.key == K_ESCAPE:
            pygame.display.quit()
            sys.exit(0)

    # Update key presses by checking
    # for pressed keys.
    keys = pygame.key.get_pressed()
    KEYS[KEY_UP] = keys[pygame.K_UP]
    KEYS[KEY_DOWN] = keys[pygame.K_DOWN]
    KEYS[KEY_RIGHT] = keys[pygame.K_RIGHT]
    KEYS[KEY_LEFT] = keys[pygame.K_LEFT]

    KEYS[KEY_W] = keys[pygame.K_w]
    KEYS[KEY_S] = keys[pygame.K_s]
    KEYS[KEY_D] = keys[pygame.K_d]
    KEYS[KEY_A] = keys[pygame.K_a]

    if DISP_COUNTER >= 20:
        draw_screen()
        pygame.display.flip()
        DISP_COUNTER = 0
    else:
        DISP_COUNTER += 1

    screen.fill(BLACK)
    time.sleep(0.001)


# Define test programs here.
def key_test():
    """
    Test program to make sure that we're responding to all relevant keys.
    """
    global SCREEN_BUFFER

    clear_buffer()

    SCREEN_BUFFER[5][2] = KEYS[KEY_UP]
    SCREEN_BUFFER[5][0] = KEYS[KEY_DOWN]
    SCREEN_BUFFER[6][1] = KEYS[KEY_RIGHT]
    SCREEN_BUFFER[4][1] = KEYS[KEY_LEFT]

    SCREEN_BUFFER[1][2] = KEYS[KEY_W]
    SCREEN_BUFFER[1][0] = KEYS[KEY_S]
    SCREEN_BUFFER[2][1] = KEYS[KEY_D]
    SCREEN_BUFFER[0][1] = KEYS[KEY_A]


charX = 0
charY = 0
timeout = 0


def move_character():
    """
    Move character on the game board.
    """

    global charX, charY, timeout

    clear_buffer()

    if timeout >= 100:
        if KEYS[KEY_W]:
            charY += 1
            timeout = 0
        if KEYS[KEY_A]:
            charX -= 1
            timeout = 0
        if KEYS[KEY_S]:
            charY -= 1
            timeout = 0
        if KEYS[KEY_D]:
            charX += 1
            timeout = 0
    else:
        timeout += 1

    # Check bounds on our new location.
    if charX < 0:
        charX = 0
    if charX > 6:
        charX = 6
    if charY < 0:
        charY = 0
    if charY > 4:
        charY = 4

    # Output character dot.
    SCREEN_BUFFER[charX][charY] = 1


################################
# WRITE YOUR OWN PROGRAM HERE! #
################################

# Note: your code will run every 1 ms, and no other timing mechanisms are
# allowed, so you cannot use time.sleep(). Instead, make a counter variable
# and count time in milliseconds

# Define game global variables here.
your_var = 0


# Define program here.
def your_program():
    global your_var

    # Modify the screen buffer to draw things on the screen.
    SCREEN_BUFFER[0][0] = your_var

    # Read from the keys pressed.
    if KEYS[KEY_W]:
        your_var = 1 - your_var


if __name__ == "__main__":
    while True:
        key_test()
        # move_character()
        # your_program()

        # Needed for simulation to run.
        update_simulator()
