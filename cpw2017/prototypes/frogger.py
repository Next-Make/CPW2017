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
WINDOW_WIDTH = 550
WINDOW_HEIGHT = 400

# Game board color
#
# NOTE: The only color the board
# buttons can turn is red.
RED = 255, 0, 0
BLACK = 0, 0, 0
FILL_COLOR = RED

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
GAME_COUNTER = None
SCREEN_BUFFER = None

def main():
    global CHAR_X, CHAR_Y, GAME_COUNTER, KEYS, SCREEN_BUFFER, SURFACE, TIMEOUT

    pygame.init()

    title = "Frogger"
    SURFACE = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))

    pygame.display.set_caption(title)
    pygame.mouse.set_visible(False)

    # The display counter used for setting how often
    # we update the screen buffer.
    GAME_COUNTER = 0

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
        frogger()

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
            radius = 30
            pos = 50 + 75*x, 350 - 75*y

            if SCREEN_BUFFER[x][y] == 0:  # unfilled
                pygame.draw.circle(SURFACE, FILL_COLOR, pos, radius, 3)
            else:  # filled
                pygame.draw.circle(SURFACE, FILL_COLOR, pos, radius, 0)


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

    global GAME_COUNTER

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

    if GAME_COUNTER >= 20:
        draw_screen()
        pygame.display.flip()
        GAME_COUNTER = 0
    else:
        GAME_COUNTER += 1

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

    global FROG, TIMEOUT

    if TIMEOUT >= 100:
        clear_buffer()
        if KEYS[KEY_UP]:
            FROG[1] += 1
            TIMEOUT = 0
        if KEYS[KEY_LEFT]:
            FROG[0] -= 1
            TIMEOUT = 0
        if KEYS[KEY_DOWN]:
            FROG[1] -= 1
            TIMEOUT = 0
        if KEYS[KEY_RIGHT]:
            FROG[0] += 1
            TIMEOUT = 0
    else:
        TIMEOUT += 1

    # Check bounds on our new location.
    if FROG[0] < 0:
        FROG[0] = 0
    if FROG[0] > 6:
        FROG[0] = 6
    if FROG[1] < 0:
        FROG[1] = 0
    if FROG[1] > 4:
        FROG[1] = 4

    # Output character dot.
    SCREEN_BUFFER[FROG[0]][FROG[1]] = 2


################################
# WRITE YOUR OWN PROGRAM HERE! #
################################

# Note: your code will run every 1 ms, and no other timing mechanisms are
# allowed, so you cannot use time.sleep(). Instead, make a counter variable
# and count time in milliseconds

# Define game global variables here. 

# Frog position [column, row]
FROG = [3, 1]

# A blink counter (BLINK) and its reset point (BLINK_RATE)
BLINK = 0
BLINK_RATE = 70

# Within each row, each truck has the same speed and the same length.
# The level list is a list of lists where each sublist represents a row.
# Within each sublist, the first element denotes the length of the trucks in that row (2 to 4).
# The second element denotes the velocity (-2, -1, 1, or 2) with positive being to the right.
# The third element is the truck index, which indicates the portion of the first truck in a row that is still off the screen.
# A sublist with all 0s represents an empty row.
LEVEL = [[0, 0, 0], [0, 0, 0], [0, 0, 0], [2, -2, 0], [3, 1, 0], [2, -1, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [1, 2, 0], [4, -1, 0], [1, 2, 0],[0, 0, 0], [0, 0, 0], [0, 0, 0], [2, -2, 0], [3, 1, 0], [2, -1, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [1, 2, 0], [1, -2, 0], [1, 2, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [2, -2, 0], [3, 1, 0], [2, -1, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0], [1, 2, 0], [4, -2, 0], [1, 2, 0], [0, 0, 0]] # [length, velocity, truck_index]

# Truck movement counter (TRUCK_COUNTER) and its reset point (TRUCK_RATE)
TRUCK_COUNTER = 0
TRUCK_RATE = 400
# Variable to make speed 1 trucks move on every other cycle
TRUCK_HALF = 0

# The beginning of the window of rows we can see
ROW_INDEX = 0

# Define program here.
def frogger():
    global TIMEOUT, FROG, BLINK, BLINK_RATE, TRUCK_COUNTER, CURRENT_LEVEL, PRIOR_LEVEL, NEXT_LEVEL

    clear_buffer()
    draw_frog()
    draw_trucks()
    if TIMEOUT >= 75:
        update_frog()
        update_trucks()
        TIMEOUT = 0
    else:
        TIMEOUT += 1
        BLINK += 1
        TRUCK_COUNTER += 1

def update_frog():
    global FROG, BLINK, BLINK_RATE, SCREEN_BUFFER, TIMEOUT, ROW_INDEX

    if KEYS[KEY_UP]:
        ROW_INDEX += 1
        TIMEOUT = 0
    if KEYS[KEY_LEFT] and not(FROG[0] <= 0):
        FROG[0] -= 1
        TIMEOUT = 0
    if KEYS[KEY_DOWN] and not ROW_INDEX <= 0:
        ROW_INDEX -= 1
        TIMEOUT = 0
    if KEYS[KEY_RIGHT] and not(FROG[0] >= 6):
        FROG[0] += 1
        TIMEOUT = 0

    check_collision()

    if ROW_INDEX == len(LEVEL):
        print 'You won! :)'
        exit(0)

def draw_frog():
    global BLINK

    if BLINK <= BLINK_RATE:
        SCREEN_BUFFER[FROG[0]][FROG[1]] = 2
    elif BLINK <= 2 * BLINK_RATE:
        SCREEN_BUFFER[FROG[0]][FROG[1]] = 0
    elif BLINK >= 2 * BLINK_RATE:
        BLINK = 0

def draw_trucks():
    """
    Draw trucks
    """
    for i in xrange(ROW_INDEX, min(ROW_INDEX + 5, len(LEVEL))):
        row = LEVEL[i]
        # Draw the current trucks
        if row[1] > 0:
            # Moving to the right
            speed = row[1]
            length = row[0]
            truck_index = row[2]

            index = truck_index
            for col in xrange(7):
                if index >= speed: #  Drawing truck
                    SCREEN_BUFFER[col][i - ROW_INDEX] = 2
                index = (index + 1) % (speed + length)
        elif row[1] < 0:
            # Moving to the left
            speed = abs(row[1])
            length = row[0]
            truck_index = row[2]

            index = truck_index
            for col in xrange(7):
                if index < speed: #  Drawing truck
                    SCREEN_BUFFER[col][i - ROW_INDEX] = 2
                index = (index - 1) % (speed + length)

def update_trucks():
    """
    Update position of trucks
    """
    global TRUCK_COUNTER, TRUCK_HALF

    TRUCK_COUNTER += 1

    if TRUCK_COUNTER >= TRUCK_RATE:
        # If trucks were updated, reset the truck counter
        TRUCK_COUNTER = 0
        TRUCK_HALF = (TRUCK_HALF + 1) % 2

        for row in LEVEL:
            # Update truck indices
            if abs(row[1]) == 2 or (abs(row[1]) == 1 and TRUCK_HALF == 1):
                row[2] = (row[2] - 1) % (abs(row[1]) + row[0])

    check_collision()

def check_collision():
    """
    Checks if the frog has collided with a truck. Ends the game if it has.
    """
    current_row = ROW_INDEX + 1
    if current_row < len(LEVEL):
        row = LEVEL[current_row]
        # Calculate truck locations in current row
        if row[1] > 0:
            # Moving to the right
            speed = row[1]
            length = row[0]
            truck_index = row[2]

            index = truck_index
            for col in xrange(7):
                if index >= speed: #  A truck exists here
                    # Check if this is at the frog's location
                    if col == FROG[0]:
                        print 'You lost! :('
                        exit(0)
                index = (index + 1) % (speed + length)
        elif row[1] < 0:
            # Moving to the left
            speed = abs(row[1])
            length = row[0]
            truck_index = row[2]

            index = truck_index
            for col in xrange(7):
                if index < speed: #  A truck exists here
                    # Check if this is at the frog's location
                    if col == FROG[0]:
                        print 'You lost! :('
                        exit(0)
                index = (index - 1) % (speed + length)

if __name__ == "__main__":
    main()
