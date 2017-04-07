"""
Example game that you can use as a template for your own games!

This code contains a lot of useful functions that you can use
for coming up with ideas for your own games, such as how you
move characters around.
"""

import sys
import time
import pygame
import random
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

SCREEN_BUFFER = None

# Handedness configuration variables.
UP_KEY = pygame.K_UP
DOWN_KEY = pygame.K_DOWN
LEFT_KEY = pygame.K_LEFT
RIGHT_KEY = pygame.K_RIGHT

LEFT_KEY_LOC_DICT = {
    pygame.K_w: 3,
    pygame.K_d: 2,
    pygame.K_a: 2,
    pygame.K_s: 1
}

RIGHT_KEY_LOC_DICT = {
    pygame.K_UP: 3,
    pygame.K_RIGHT: 2,
    pygame.K_LEFT: 2,
    pygame.K_DOWN: 1
}

# Game state variables
WAIT = 0
PLAY = 1
WINS_LEFT = 0
WINS_RIGHT = 0

DELAY_THRESHOLD = 100000
MIN_DELAY_THRESHOLD = 50000
DECREMENT_DELAY_THRESH = 700
TRANSITION_OCCURRED = False

GAME_STATE = WAIT

LEFT_SIDE = 0
MIDDLE = 3
RIGHT_SIDE = 6

TRANSITION_DELAY = 0

def main():
    global GAME_COUNTER, KEYS, SCREEN_BUFFER, SURFACE, TIMEOUT

    pygame.init()

    title = "Guitar Hero?"
    SURFACE = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))

    pygame.display.set_caption(title)
    pygame.mouse.set_visible(False)

    # The screen is a 7x5 array and is the only item
    # that should be modified during your game.
    SCREEN_BUFFER = [[0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0]]

    # Array for keeping tracked of relevant
    # game keys and whether they have been
    # pressed or not.
    #
    # 0 = Key Not Pressed
    # 1 = Key Pressed
    KEYS = [0, 0, 0, 0, 0, 0, 0, 0]

    while True:
        if GAME_STATE == WAIT:
            paused()
        elif GAME_STATE == PLAY:
            play_game()

def paused():
    """
    Wait for the user to decide which hand to use before starting.
    """

    global GAME_STATE, WINS_LEFT, WINS_RIGHT, TRANSITION_OCCURRED

    print_score()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.display.quit()
                sys.exit(0)
            elif event.key in LEFT_KEY_LOC_DICT or event.key in RIGHT_KEY_LOC_DICT:
                GAME_STATE = PLAY
                clear_buffer()
                TRANSITION_OCCURRED = False
                if WINS_LEFT >= 5 or WINS_RIGHT >= 5:
                    reset_wins()

def play_game():
    global MIN_DELAY_THRESHOLD, DELAY_THRESHOLD, DECREMENT_DELAY_THRESH, TRANSITION_DELAY, TRANSITION_OCCURRED
    global GAME_STATE, SCREEN_BUFFER

    left_press = None
    right_press = None

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.display.quit()
                sys.exit(0)
            if event.key in LEFT_KEY_LOC_DICT:
                left_press = LEFT_KEY_LOC_DICT[event.key]
            if event.key in RIGHT_KEY_LOC_DICT:
                right_press = RIGHT_KEY_LOC_DICT[event.key]

    # Move dots?
    if TRANSITION_DELAY > max(DELAY_THRESHOLD, MIN_DELAY_THRESHOLD):
        TRANSITION_DELAY = 0 # reset
        transition()
        generate_new()
        update_screen()
        TRANSITION_OCCURRED = True
        if DELAY_THRESHOLD > MIN_DELAY_THRESHOLD:
            DELAY_THRESHOLD = max(MIN_DELAY_THRESHOLD, DELAY_THRESHOLD - DECREMENT_DELAY_THRESH)
    else:
        TRANSITION_DELAY += 1

    # Did left press the correct button at the correct time
    if left_press is not None:
        if SCREEN_BUFFER[0][left_press] == 1:
            SCREEN_BUFFER[LEFT_SIDE][left_press] = 0
            update_screen()
        elif TRANSITION_OCCURRED:  # mispress
            right_wins()

    # Did right press the correct button at the correct time
    if right_press is not None:
        if SCREEN_BUFFER[6][right_press] == 1:
            SCREEN_BUFFER[RIGHT_SIDE][right_press] = 0
            update_screen()
        elif TRANSITION_OCCURRED:
            left_wins()

def generate_new():
    SCREEN_BUFFER[3][random.randint(1,3)] = 1

def transition():
    global SCREEN_BUFFER, WINS_LEFT, WINS_RIGHT

    for col in range(len(SCREEN_BUFFER)):
        for row in range(1, len(SCREEN_BUFFER[col]) - 1):
            if SCREEN_BUFFER[col][row] % 2 == 1:
                SCREEN_BUFFER[col][row] -= 1
                if col == MIDDLE: # propagate both directions
                    SCREEN_BUFFER[col + 1][row] += 2
                    SCREEN_BUFFER[col - 1][row] += 2
                elif MIDDLE < col < RIGHT_SIDE:
                    SCREEN_BUFFER[col + 1][row] += 2
                elif LEFT_SIDE < col < MIDDLE:
                    SCREEN_BUFFER[col - 1][row] += 2
                elif col == LEFT_SIDE:
                    right_wins()
                elif col == RIGHT_SIDE:
                    left_wins()
    for col in range(len(SCREEN_BUFFER)):
        for row in range(1, len(SCREEN_BUFFER[col]) - 1):
            if SCREEN_BUFFER[col][row] > 0:
                SCREEN_BUFFER[col][row] = 1

def left_wins():
    global WINS_LEFT
    WINS_LEFT += 1
    reset_game()

def right_wins():
    global WINS_RIGHT
    WINS_RIGHT += 1
    reset_game()

def reset_wins():
    global WINS_LEFT, WINS_RIGHT
    WINS_LEFT = 0
    WINS_RIGHT = 0

def reset_game():
    """
    Reset the game state for a fresh game.
    """

    global GAME_STATE

    GAME_STATE = WAIT
    for _ in range(1000000):
        pass
    pygame.event.clear() # flush buffer

    print("Game reset")

def print_score():
    global SCREEN_BUFFER, WINS_LEFT, WINS_RIGHT

    clear_buffer()

    for row in range(WINS_LEFT):
        for col in range(0, 3):
            SCREEN_BUFFER[col][row] = 1

    for row in range(WINS_RIGHT):
        for col in range(4, 7):
            SCREEN_BUFFER[col][row] = 1

    update_screen()

    count = 0


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

if __name__ == "__main__":
    main()