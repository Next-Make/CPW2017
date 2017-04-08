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

    title = "Pygame Keyboard Test"
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
        # key_test()
        # move_character()
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

# Within each row, each truck has the same speed and the same length.
# The trucks list is a list of lists where each sublist represents a row.
# Within each sublist, the first element denotes the length of the trucks in that row (2 to 4)
# The second element denotes the velocity (-2, -1, 1, or 2) with positive being to the right
# Subsequent elements denote the starting position of each truck
PRIOR_LEVEL = []
CURRENT_LEVEL = []
NEXT_LEVEL = []
START_ROW = 0

TRUCK_VELOCITIES = [-1,2,1,-2]
TRUCK_VELOCITY_COUNTER = 0

TRUCK_LENGTH = [2,3,4]
TRUCK_LENGTH_COUNTER = 0

# A blink counter (BLINK) and its reset point (BLINK_RATE)
BLINK = 0
BLINK_RATE = 70

# Truck movement counter (TRUCK_COUNTER) and its reset point (TRUCK_RATE)
TRUCK_COUNTER = 0
TRUCK_RATE = 400
# Variable to make truck move on every other cycle
TRUCK_HALF = 0

# Define program here.
def frogger():
    global TIMEOUT, FROG, BLINK, BLINK_RATE, TRUCK_COUNTER, CURRENT_LEVEL, PRIOR_LEVEL, NEXT_LEVEL

    CURRENT_LEVEL = generate_level()
    PRIOR_LEVEL = generate_level()
    NEXT_LEVEL = generate_level()

    if TIMEOUT >= 50:
        update_screen()
        update_frog()
        update_trucks()
    else:
        TIMEOUT += 1
        BLINK += 1
        TRUCK_COUNTER += 1

def generate_level():
    global TRUCK_VELOCITY_COUNTER, TRUCK_LENGTH_COUNTER
    level = [[0,0],[0,0],[0,0]]
    for row in range(7):
        new_row = [TRUCK_LENGTH[TRUCK_LENGTH_COUNTER], TRUCK_VELOCITIES[TRUCK_VELOCITY_COUNTER]]
        start_position = 1
        while start_position < 7:
            new_row.append(start_position)
            start_position += new_row[0] + abs(new_row[1]) + 1
        TRUCK_LENGTH_COUNTER = (TRUCK_LENGTH_COUNTER + 1) % len(TRUCK_LENGTH)
        TRUCK_VELOCITY_COUNTER = (TRUCK_VELOCITY_COUNTER + 1) % len(TRUCK_VELOCITIES)
        level.append(new_row)
    return level

def update_screen():
    clear_buffer()

def update_frog():
    global FROG, BLINK, BLINK_RATE, SCREEN_BUFFER, TIMEOUT

    key_press = False

    if KEYS[KEY_UP] and not(FROG[1] >= 4):
        FROG[1] += 1
        key_press = True
        TIMEOUT = 0
    if KEYS[KEY_LEFT] and not(FROG[0] <= 0):
        FROG[0] -= 1
        key_press = True
        TIMEOUT = 0
    if KEYS[KEY_DOWN] and not(FROG[1] <= 0):
        FROG[1] -= 1
        key_press = True
        TIMEOUT = 0
    if KEYS[KEY_RIGHT] and not(FROG[0] >= 6):
        FROG[0] += 1
        key_press = True
        TIMEOUT = 0

    #TODO: fix buggy response time with blinking
    if BLINK <= BLINK_RATE:
        SCREEN_BUFFER[FROG[0]][FROG[1]] = 2
    elif BLINK <= 2 * BLINK_RATE:
        SCREEN_BUFFER[FROG[0]][FROG[1]] = 0
    elif BLINK >= 2 * BLINK_RATE:
        BLINK = 0

    #TODO: add collision detection
    
def update_trucks():
    """
    Update position of trucks
    """
    global CURRENT_LEVEL, SCREEN_BUFFER, TIMEOUT, TRUCK_COUNTER, TRUCK_HALF

    if TRUCK_COUNTER >= TRUCK_RATE:
        for row in CURRENT_LEVEL:

            if row[1] == 2 or (row[1] == 1 and TRUCK_HALF == 1):
                for i in range(2, len(row)):
                    row[i] += 1
            elif row[1] == -2 or (row[1] == -1 and TRUCK_HALF == 1):
                for i in range(2,len(row)):
                    row[i] -= 1

            # Post-processing: delete trucks that are gone, add new ones as needed, 
            # Process positive velocities
            if row[1] == 2 or (row[1] == 1 and TRUCK_HALF == 1):

                # Check if the end of the last truck extends beyond the screen
                if row[-1] - row[0] + 1 > 6:
                    del(row[-1])

                # Check if the first truck has advanced far enough to add another truck
                # Sufficient gap is determined by velocity + 1 (row[1])
                if row[2] - row[0] + 1 > row[1]:
                    # Add a truck at position 0
                    row.insert(2,0)

            # Process negative velocities
            elif row[1] == -2 or row[1] == -1 and TRUCK_HALF == 1:
                if row[2] + row[0] -1 < 0:
                    del(row[2])

                if row[-1] + row[0] -1< 7 + row[1]:
                    row.append(7)
        
        # If trucks were updated, reset the truck counter
        TRUCK_COUNTER = 0
        TRUCK_HALF = (TRUCK_HALF + 1) % 2

        # Write all trucks to screen
        for row_num in range(START_ROW, min(START_ROW + 6, 10)):
            for truck in CURRENT_LEVEL[row_num][2:]:
                # Write the pixels for each truck
                for offset in range(CURRENT_LEVEL[row_num][0]):
                    if CURRENT_LEVEL[row_num][1] < 0 and truck + offset < 7 and truck + offset > -1:
                        SCREEN_BUFFER[truck + offset][row_num - START_ROW] = 2
                    elif CURRENT_LEVEL[row_num][1] > 0 and truck - offset > -1 and truck - offset < 7:
                        # print("row_num: {} \ntruck - offset: {}\n".format(row_num, truck - offset))
                        SCREEN_BUFFER[truck - offset][row_num - START_ROW] = 2
                    
    TIMEOUT = 0

def scroll():
    """
    Deletes bottom-most row of TRUCKS that moves off the screen.
    Create new row of trucks and adds it to top of TRUCKS.
    Moves frog position down to maintain its relative position
    """
    global FROG, TRUCKS, SCREEN_BUFFER, TRUCK_LENGTH_COUNTER, TRUCK_VELOCITY_COUNTER

    # Move frog down one position to allow scrolling.
    #if FROG[1] == 5




    FROG[1] -= 1

    del(TRUCKS[0])

    new_row = [TRUCK_LENGTH[TRUCK_LENGTH_COUNTER], TRUCK_VELOCITIES[TRUCK_VELOCITY_COUNTER]]
    TRUCK_VELOCITY_COUNTER = (TRUCK_VELOCITY_COUNTER + 1) % 4
    TRUCK_LENGTH_COUNTER = (TRUCK_LENGTH_COUNTER + 1) % 3

    # Add a little "randomness" to the starting point of the first truck of each new row
    truck_position = TRUCK_LENGTH_COUNTER
    
    # Add trucks to the new row
    while truck_position < 7:
        new_row.append(truck_position)

        # Beginning of next truck must be after the length of the first truck and with a gap
        # Gap is either 2 or 3 as determined by the pairity of TRUCK_VELOCITY_COUNTER
        truck_position += new_row[0] + (TRUCK_VELOCITY_COUNTER % 2 + 3)

    TRUCKS.append(new_row)    


if __name__ == "__main__":
    main()
