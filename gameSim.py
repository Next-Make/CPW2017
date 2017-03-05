"""
Example game that you can use as a template for your own games!

This code contains a lot of useful functions that you can use
for coming up with ideas for your own games, such as how you
move characters around.
"""

import sys
import time
import pygame
from pygame.locals import *

# Window dimensions
WINDOWWIDTH = 550
WINDOWHEIGHT = 400

# Game board colors
WHITE = 255, 255, 255
GREEN = 0, 255, 0
BLUE = 0, 0, 255
BLACK = 0, 0, 0
RED = 255, 0, 0

# Key syntactic sugars
KEY_W = 0
KEY_A = 1
KEY_S = 2
KEY_D = 3

KEY_UP = 4
KEY_DOWN = 5
KEY_LEFT = 6
KEY_RIGHT = 7

# Game global variables
KEYS = None
CHAR_X = None
CHAR_Y = None
SURFACE = None
TIMEOUT = None
DISP_COUNTER = None
SCREEN_BUFFER = None


def main():
    global CHAR_X, CHAR_Y, DISP_COUNTER, KEYS, SCREEN_BUFFER, SURFACE, TIMEOUT

    pygame.init()

    title = "Pygame Keyboard Test"
    SURFACE = pygame.display.set_mode((WINDOWWIDTH, WINDOWHEIGHT))

    pygame.display.set_caption(title)
    pygame.mouse.set_visible(False)

    # The display counter used for setting how often
    # we update the screen buffer.
    DISP_COUNTER = 0

    # The screen is a 7x5 array and is the only item
    # that should be modified during your game.
    SCREEN_BUFFER = [[0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0]]

    # Location for a character pixel
    CHAR_X = 0
    CHAR_Y = 0

    # Counter used when moving a character
    TIMEOUT = 0

    # Array for keeping tracked of relevant
    # game keys and whether they have been
    # pressed or not.
    #
    # 0 = Key Not Pressed
    # 1 = Key Pressed
    KEYS = [0, 0, 0, 0, 0, 0, 0, 0]

    while True:
        key_test()
        # move_character()
        # your_program()

        # Needed for simulation to run.
        update_simulator()


def clear_buffer():
    """
    Clear the screen buffer.
    """

    for x in range(7):
        for y in range(5):
            SCREEN_BUFFER[x][y] = 0


def draw_screen():
    """
    Redraw the screen using the updated screen buffer.
    """

    for x in range(7):
        for y in range(5):
            color = RED
            radius = 30
            pos = 50 + 75*x, 350 - 75*y

            if SCREEN_BUFFER[x][y] == 0:  # unfilled
                pygame.draw.circle(SURFACE, color, pos, radius, 3)
            else:  # filled
                pygame.draw.circle(SURFACE, color, pos, radius, 0)


def update_keys(new_keys):
    """
    Update the key mappings based on the new keys provided.

    :param new_keys: The mappings provided from pygame for keys pressed.
    """

    KEYS[KEY_UP] = new_keys[pygame.K_UP]
    KEYS[KEY_DOWN] = new_keys[pygame.K_DOWN]
    KEYS[KEY_RIGHT] = new_keys[pygame.K_RIGHT]
    KEYS[KEY_LEFT] = new_keys[pygame.K_LEFT]

    KEYS[KEY_W] = new_keys[pygame.K_w]
    KEYS[KEY_S] = new_keys[pygame.K_s]
    KEYS[KEY_D] = new_keys[pygame.K_d]
    KEYS[KEY_A] = new_keys[pygame.K_a]


def update_simulator():
    """
    Update the simulator, including the display and keys pressed.
    """

    global DISP_COUNTER

    for event in pygame.event.get():
        if event.type == QUIT:
            sys.exit()
        elif event.type == KEYDOWN and event.key == K_ESCAPE:
            pygame.display.quit()
            sys.exit(0)

    # Update key presses by checking
    # for pressed keys.
    keys = pygame.key.get_pressed()
    update_keys(keys)

    if DISP_COUNTER >= 20:
        draw_screen()
        pygame.display.flip()
        DISP_COUNTER = 0
    else:
        DISP_COUNTER += 1

    SURFACE.fill(BLACK)
    time.sleep(0.001)


# Define test programs here.
def key_test():
    """
    Test program to make sure that we're responding to all relevant keys.
    """

    clear_buffer()

    SCREEN_BUFFER[5][2] = KEYS[KEY_UP]
    SCREEN_BUFFER[5][0] = KEYS[KEY_DOWN]
    SCREEN_BUFFER[6][1] = KEYS[KEY_RIGHT]
    SCREEN_BUFFER[4][1] = KEYS[KEY_LEFT]

    SCREEN_BUFFER[1][2] = KEYS[KEY_W]
    SCREEN_BUFFER[1][0] = KEYS[KEY_S]
    SCREEN_BUFFER[2][1] = KEYS[KEY_D]
    SCREEN_BUFFER[0][1] = KEYS[KEY_A]


def move_character():
    """
    Move a character on the game board.
    """

    global CHAR_X, CHAR_Y, TIMEOUT

    clear_buffer()

    if TIMEOUT >= 100:
        if KEYS[KEY_W]:
            CHAR_Y += 1
            TIMEOUT = 0
        if KEYS[KEY_A]:
            CHAR_X -= 1
            TIMEOUT = 0
        if KEYS[KEY_S]:
            CHAR_Y -= 1
            TIMEOUT = 0
        if KEYS[KEY_D]:
            CHAR_X += 1
            TIMEOUT = 0
    else:
        TIMEOUT += 1

    # Check bounds on our new location.
    if CHAR_X < 0:
        CHAR_X = 0
    if CHAR_X > 6:
        CHAR_X = 6
    if CHAR_Y < 0:
        CHAR_Y = 0
    if CHAR_Y > 4:
        CHAR_Y = 4

    # Output character dot.
    SCREEN_BUFFER[CHAR_X][CHAR_Y] = 1


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
    main()
