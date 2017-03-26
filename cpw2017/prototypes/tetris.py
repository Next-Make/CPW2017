"""
Tetris (kinda).
"""

import sys
import time
import pygame
from pygame.locals import *

# Window dimensions
WINDOW_WIDTH = 400
WINDOW_HEIGHT = 550

# Game board dimensions
BOARD_WIDTH = 5
BOARD_HEIGHT = 7

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
START_X = None
START_Y = None
SURFACE = None
MOVE_TIMEOUT = None
DROP_TIMEOUT = None
GAME_COUNTER = None
SCREEN_BUFFER = None
PIXELS = None
PIECES = None
INDEX = None
CURRENT_PIECE = None

# Tetris pieces
LPIECE = [[0, 0], [0, -1], [1, 0]]
IPIECE = [[0, 0], [1, 0], [-1, 0]]
SPIECE = [[0, 0], [0, -1], [1, 0], [1, -1]]

GAMEOVER = None


def main():
    global GAME_COUNTER, KEYS, SCREEN_BUFFER, SURFACE, PIXELS, MOVE_TIMEOUT, DROP_TIMEOUT, START_X, START_Y, PIECES, INDEX, CURRENT_PIECE

    pygame.init()

    title = "Tetris"
    SURFACE = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))

    pygame.display.set_caption(title)
    pygame.mouse.set_visible(False)

    # The display counter used for setting how often
    # we update the screen buffer.
    GAME_COUNTER = 0

    # The screen is a 7x5 array and is the only item
    # that should be modified during your game.
    SCREEN_BUFFER = [[0, 0, 0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0]]

    # Pixels representing blocks that have been previously dropped
    PIXELS = [[0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0]]

    # Counter used when moving a piece
    MOVE_TIMEOUT = 0

    # Counter used when dropping a piece
    DROP_TIMEOUT = 0

    # Array for keeping tracked of relevant
    # game keys and whether they have been
    # pressed or not.
    #
    # 0 = Key Not Pressed
    # 1 = Key Pressed
    KEYS = [0, 0, 0, 0, 0, 0, 0, 0]

    # Starting position for pieces
    START_X = 2
    START_Y = 6

    # List to keep track of objects and their positions
    PIECES = [LPIECE, IPIECE, LPIECE, SPIECE, LPIECE, LPIECE, IPIECE]
    INDEX = 0

    # Current Tetris piece
    CURRENT_PIECE = [[p[0] + START_X, p[1] + START_Y] for p in PIECES[INDEX]]

    # Whether the game has ended
    GAMEOVER = False

    while not GAMEOVER:
        move_piece()
        drop_pieces()

        # Needed for simulation to run.
        update_simulator()


def clear_buffer():
    """
    Clear the screen buffer.
    """

    for x in range(BOARD_WIDTH):
        for y in range(BOARD_HEIGHT):
            SCREEN_BUFFER[x][y] = 0


def draw_screen():
    """
    Redraw the screen using the updated screen buffer.
    """

    for x in range(BOARD_WIDTH):
        for y in range(BOARD_HEIGHT):
            radius = 30
            pos = 50 + 75*x, 500 - 75*y

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
def move_piece():
    """
    Move the piece left and right on the game board.
    """

    global MOVE_TIMEOUT, CURRENT_PIECE, PIXELS

    clear_buffer()

    if MOVE_TIMEOUT >= 75:
        MOVE_TIMEOUT = 0
        if KEYS[KEY_LEFT]:
            # Move the piece.
            for i in range(len(CURRENT_PIECE)):
                CURRENT_PIECE[i][0] -= 1
            # Check bounds and collisions on our new location.
            for i in range(len(CURRENT_PIECE)):
                if CURRENT_PIECE[i][0] < 0 or PIXELS[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1]]:
                    # Collision on the left, scoot everything to the right
                    for j in range(len(CURRENT_PIECE)):
                        CURRENT_PIECE[j][0] += 1
        if KEYS[KEY_RIGHT]:
            # Move the piece.
            for i in range(len(CURRENT_PIECE)):
                CURRENT_PIECE[i][0] += 1
            # Check bounds and collisions on our new location.
            for i in range(len(CURRENT_PIECE)):
                if CURRENT_PIECE[i][0] >= BOARD_WIDTH or PIXELS[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1]]:
                    # Collision on the right, scoot everything to the left
                    for j in range(len(CURRENT_PIECE)):
                        CURRENT_PIECE[j][0] -= 1
        # TODO: rotate pieces
        if KEYS[KEY_A]:
            # rotate counterclockwise
            pass
        if KEYS[KEY_D]:
            # rotate clockwise
            pass
    else:
        MOVE_TIMEOUT += 1

    # Output current piece in display.
    for i in range(len(CURRENT_PIECE)):
        SCREEN_BUFFER[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1]] = 1

# Note: your code will run every 1 ms, and no other timing mechanisms are
# allowed, so you cannot use time.sleep(). Instead, make a counter variable
# and count time in milliseconds

# Game program.
def convert_to_pixels():
    """
    Freeze the current piece on the board.
    """

    global CURRENT_PIECE, PIXELS

    for i in range(len(CURRENT_PIECE)):
        # Move the current piece up and save it as pixels
        PIXELS[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1] + 1] = 1

def get_new_piece():
    """
    Generate a new Tetris piece.
    """

    global CURRENT_PIECE, PIECES, INDEX, START_X, START_Y

    INDEX += 1
    INDEX %= len(PIECES)
    CURRENT_PIECE = [[p[0] + START_X, p[1] + START_Y] for p in PIECES[INDEX]]

def collide_with_bottom():
    """
    Check for collisions between the current piece and the bottom of the game board.
    """

    global CURRENT_PIECE

    for i in range(len(CURRENT_PIECE)):
        if CURRENT_PIECE[i][1] < 0:
            # The piece has hit the bottom of the screen
            convert_to_pixels()
            get_new_piece()
            clear_rows()
            return True
    return False

def collide_with_pixels():
    """
    Check for collisions between the current piece and other pieces on the game board.
    """

    global CURRENT_PIECE, PIXELS

    for i in range(len(CURRENT_PIECE)):
        if PIXELS[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1]]:
            # Another piece is below this one
            convert_to_pixels()
            get_new_piece()
            clear_rows()
            return True
    return False

def drop_pieces():
    """
    Drop the current piece down on the game board over time.
    """

    global DROP_TIMEOUT, CURRENT_PIECE, PIXELS

    if DROP_TIMEOUT >= 500:
        DROP_TIMEOUT = 0

        for i in range(len(CURRENT_PIECE)):
            CURRENT_PIECE[i][1] -= 1

        # Try colliding the current piece with the bottom of the screen and with other pieces
        if not collide_with_bottom():
            collide_with_pixels()
    else:
        DROP_TIMEOUT += 1

    # Draw current piece
    for i in range(len(CURRENT_PIECE)):
        SCREEN_BUFFER[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1]] = 1

    # Draw pixels
    for i in range(len(PIXELS)):
        for j in range(len(PIXELS[0])):
            SCREEN_BUFFER[i][j] += PIXELS[i][j]

def completed_row(row):
    """
    Checks if a particular row on the game board is completed.
    """

    global PIXELS

    for i in range(BOARD_WIDTH):
        if not PIXELS[i][row]:
            return False
    return True

def clear_rows():
    """
    Clear all completed rows on the game board.
    """

    global PIXELS

    row = 0
    while True:
        if completed_row(row):
            # Clear the row and move everything down
            for r in range(row + 1, BOARD_HEIGHT):
                for c in range(0, BOARD_WIDTH):
                    PIXELS[r-1][c] = PIXELS[r][c]
            # Reset row to 0 so we'll check all rows again
            row = 0

        row += 1
        if row >= BOARD_HEIGHT:
            return

def check_gameover():
    """
    Checks if the game has ended.
    """

    global GAMEOVER

    if collide_with_pixels():
        GAMEOVER = True


if __name__ == "__main__":
    main()
