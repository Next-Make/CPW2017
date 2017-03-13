"""
Game that tests your memory. How well can you remember
a sequence containing only four unique rectangles?
"""

import sys
import pygame
import random

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

# Game state variables
CONFIG = 0
DISPLAY = 1
AWAITING = 2

GAME_STATE = CONFIG

# Boarder square locations
BOARDER = [(0, 2), (1, 2), (2, 2), (3, 0), (3, 1), (3, 2),
           (3, 3), (3, 4), (4, 2), (5, 2), (6, 2)]

# Memory sequence variables
SEQUENCE = []
INDEX = 0

TOP_LEFT = 0
TOP_RIGHT = 1
BOTTOM_LEFT = 2
BOTTOM_RIGHT = 3

TOP_LEFT_SQUARES = [(0, 3), (0, 4), (1, 3), (1, 4), (2, 3), (2, 4)]
TOP_RIGHT_SQUARES = [(4, 3), (4, 4), (5, 3), (5, 4), (6, 3), (6, 4)]
BOTTOM_LEFT_SQUARES = [(0, 0), (0, 1), (1, 0), (1, 1), (2, 0), (2, 1)]
BOTTOM_RIGHT_SQUARES = [(4, 0), (4, 1), (5, 0), (5, 1), (6, 0), (6, 1)]

LOC_SQUARE_DICT = {
    TOP_LEFT: TOP_LEFT_SQUARES,
    TOP_RIGHT: TOP_RIGHT_SQUARES,
    BOTTOM_LEFT: BOTTOM_LEFT_SQUARES,
    BOTTOM_RIGHT: BOTTOM_RIGHT_SQUARES,
}

# Dictionary mapping which keys map to
# which rectangles depending on direction.
KEY_LOC_DICT = {}

LEFT_KEY_LOC_DICT = {
    pygame.K_w: TOP_LEFT,
    pygame.K_d: TOP_RIGHT,
    pygame.K_a: BOTTOM_LEFT,
    pygame.K_s: BOTTOM_RIGHT
}

RIGHT_KEY_LOC_DICT = {
    pygame.K_UP: TOP_LEFT,
    pygame.K_RIGHT: TOP_RIGHT,
    pygame.K_LEFT: BOTTOM_LEFT,
    pygame.K_DOWN: BOTTOM_RIGHT
}

# Dictionary mapping which keys map to
# which forms of game reset during game.
#
# A full reset means we reset the user
# configuration, whereas a partial reset
# means we retain the configuration and
# start generating the memory sequence
# immediately after the reset.
RESET_FULL = "FULL"
RESET_PART = "PARTIAL"

RESET_LOC_DICT = {}

LEFT_RESET_LOC_DICT = {
    pygame.K_w: RESET_PART,
    pygame.K_s: RESET_FULL
}

RIGHT_RESET_LOC_DICT = {
    pygame.K_UP: RESET_PART,
    pygame.K_DOWN: RESET_FULL
}

# Squares indicating correct or wrong for a sequence
RIGHT_SQUARES = [(1, 1), (2, 0), (3, 1), (4, 2), (5, 3), (6, 4)]
WRONG_SQUARES = [(1, 0), (1, 4), (2, 1), (2, 3), (3, 2), (4, 1),
                 (4, 3), (5, 0), (5, 4)]


def main():
    global CHAR_X, CHAR_Y, GAME_COUNTER, GAME_STATE
    global KEYS, SCREEN_BUFFER, SURFACE, TIMEOUT

    pygame.init()

    title = "Memory Game"
    SURFACE = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))

    pygame.display.set_caption(title)
    pygame.mouse.set_visible(False)

    # The display counter used for setting how often
    # we update the screen buffer.
    GAME_COUNTER = 0

    # The screen is a 7x5 array and is the only item
    # that should be modified during your game.
    SCREEN_BUFFER = [[0, 0, 1, 0, 0],
                     [0, 0, 1, 0, 0],
                     [0, 0, 1, 0, 0],
                     [1, 1, 1, 1, 1],
                     [0, 0, 1, 0, 0],
                     [0, 0, 1, 0, 0],
                     [0, 0, 1, 0, 0]]

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
        if GAME_STATE == CONFIG:
            await_configuration()
        elif GAME_STATE == DISPLAY:
            generate_sequence()
        elif GAME_STATE == AWAITING:
            await_sequence()


def await_configuration():
    """
    Wait for the user to decide which hand to use before starting.
    """

    global GAME_STATE, KEY_LOC_DICT, RESET_LOC_DICT

    fill_boarder()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.display.quit()
                sys.exit(0)
            elif event.key in LEFT_KEY_LOC_DICT:
                GAME_STATE = DISPLAY
                KEY_LOC_DICT = LEFT_KEY_LOC_DICT.copy()
                RESET_LOC_DICT = RIGHT_RESET_LOC_DICT.copy()
            elif event.key in RIGHT_KEY_LOC_DICT:
                GAME_STATE = DISPLAY
                KEY_LOC_DICT = RIGHT_KEY_LOC_DICT.copy()
                RESET_LOC_DICT = LEFT_RESET_LOC_DICT.copy()


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


def update_screen():
    """
    Update the screen displayed to the user.
    """

    draw_screen()
    pygame.display.flip()
    SURFACE.fill(BLACK)


def fill_boarder():
    """
    Fill the appropriate squares for the boarder in the screen buffer.
    """

    clear_buffer()

    for x, y in BOARDER:
        SCREEN_BUFFER[x][y] = 1

    update_screen()


def await_sequence():
    """
    Wait for the next element in the sequence.
    """

    global GAME_STATE, INDEX

    next_rect = SEQUENCE[INDEX]
    user_rect = None

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.display.quit()
                sys.exit(0)
            elif event.key in KEY_LOC_DICT:
                user_rect = KEY_LOC_DICT[event.key]
            elif event.key in RESET_LOC_DICT:
                reset_type = RESET_LOC_DICT[event.key]

                reset_game()

                if reset_type == RESET_PART:
                    GAME_STATE = DISPLAY

                return

    if user_rect is not None:
        squares = LOC_SQUARE_DICT[user_rect]

        for x, y in squares:
            SCREEN_BUFFER[x][y] = 1

        if user_rect == next_rect:
            INDEX += 1

            if INDEX == len(SEQUENCE):
                signal_right()
        else:
            signal_wrong()


def signal_right():
    """
    Signal the user has replayed the entire sequence correctly.
    """

    global GAME_STATE, INDEX
    clear_buffer()

    for x, y in RIGHT_SQUARES:
        SCREEN_BUFFER[x][y] = 1

    update_screen()
    counter = 0

    while counter < 10000000:
        counter += 1

    fill_boarder()

    counter = 0

    while counter < 10000000:
        counter += 1

    # Reset index for the next iteration of displaying
    GAME_STATE = DISPLAY
    INDEX = 0


def signal_wrong():
    """
    Signal the user has chosen the wrong next element.
    """

    clear_buffer()

    for x, y in WRONG_SQUARES:
        SCREEN_BUFFER[x][y] = 1

    update_screen()
    counter = 0

    while counter < 10000000:
        counter += 1

    fill_boarder()

    counter = 0

    while counter < 10000000:
        counter += 1

    reset_game()


def generate_sequence():
    """
    Generate the memory sequence and display.
    """

    global SEQUENCE, GAME_STATE

    rect = random.randint(TOP_LEFT, BOTTOM_RIGHT)
    SEQUENCE.append(rect)

    index = 0
    counter = 0
    displaying = False

    fill_boarder()

    while index < len(SEQUENCE):
        if counter % 1000000 == 0:
            if displaying:
                clear_buffer()
                fill_boarder()

                index += 1
            else:
                for x, y in LOC_SQUARE_DICT[SEQUENCE[index]]:
                    SCREEN_BUFFER[x][y] = 1

            displaying = not displaying

            draw_screen()
            pygame.display.flip()
            SURFACE.fill(BLACK)

        counter += 1

    GAME_STATE = AWAITING


def reset_game():
    """
    Reset the game state for a fresh game.
    """

    global GAME_STATE, INDEX, SEQUENCE

    GAME_STATE = CONFIG
    SEQUENCE = []
    INDEX = 0


if __name__ == "__main__":
    main()
