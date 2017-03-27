"""
This version of Tetris is rotated, meaning it mimics the layout and controls of
the physical board.

A miniature version of Tetris, with 3- and 4-blocks.

Move and rotate falling blocks to form lines and clear the board!
Game over if the screen fills up.

The game speeds up over time, see how long you can last!
"""

import sys
import time
import pygame
from pygame.locals import *

# Window dimensions
WINDOW_WIDTH = 550
WINDOW_HEIGHT = 400

# Game board dimensions
BOARD_WIDTH = 7
BOARD_HEIGHT = 5

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
SURFACE = None
MOVE_TIMEOUT = None
DROP_TIMEOUT = None
DROP_INTERVAL = None
DROP_SPEEDUP_TIMEOUT = None
GAME_COUNTER = None
SCREEN_BUFFER = None
PIXELS = None
INDEX = None
CURRENT_PIECE = None
CURRENT_PIECE_TYPE = None
CURRENT_ORIENTATION = None
GAMEOVER = None

# Tetris pieces
LPIECE = [[0, 0], [1, 0], [0, 1]]
IPIECE = [[0, 0], [0, 1], [0, -1]]
SPIECE = [[0, 0], [0, -1], [1, 0], [1, -1]]

# Starting position for pieces
START_X = 0
START_Y = 2

# List to keep track of objects and their positions
PIECES = [['L', LPIECE], ['I', IPIECE], ['L', LPIECE], ['S', SPIECE], ['L', LPIECE], ['L', LPIECE], ['I', IPIECE]]

def main():
    global GAME_COUNTER, KEYS, SCREEN_BUFFER, SURFACE, PIXELS, MOVE_TIMEOUT, DROP_TIMEOUT, DROP_INTERVAL, DROP_SPEEDUP_TIMEOUT, PIECES, INDEX, CURRENT_PIECE, CURRENT_PIECE_TYPE, CURRENT_ORIENTATION

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
    SCREEN_BUFFER = [[0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0]]

    # Pixels representing blocks that have been previously dropped
    PIXELS = [[0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0]]

    # Counter used when moving a piece
    MOVE_TIMEOUT = 0

    # Counter used when dropping a piece
    DROP_TIMEOUT = 0

    # Initial interval between piece-dropping events
    DROP_INTERVAL = 500

    # Counter used when speeding up the drop interval
    DROP_SPEEDUP_TIMEOUT = 0

    # Array for keeping tracked of relevant
    # game keys and whether they have been
    # pressed or not.
    #
    # 0 = Key Not Pressed
    # 1 = Key Pressed
    KEYS = [0, 0, 0, 0, 0, 0, 0, 0]

    # Index into pieces list
    INDEX = 0

    # Current Tetris piece
    CURRENT_PIECE = [[p[0] + START_X, p[1] + START_Y] for p in PIECES[INDEX][1]]
    CURRENT_PIECE_TYPE = PIECES[INDEX][0]
    CURRENT_ORIENTATION = 0

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
def rotate_ccw():
    """
    Rotate current piece counterclockwise.
    """

    global CURRENT_PIECE, CURRENT_PIECE_TYPE, CURRENT_ORIENTATION

    if CURRENT_PIECE_TYPE == 'L':
        # Orientation 0
        # 2
        # 0 1
        if CURRENT_ORIENTATION == 0:
            CURRENT_ORIENTATION = 3
            CURRENT_PIECE[0][0] += 1
            CURRENT_PIECE[1][1] += 1
            CURRENT_PIECE[2][1] -= 1
        # Orientation 1
        # 0 2
        # 1
        elif CURRENT_ORIENTATION == 1:
            CURRENT_ORIENTATION = 0
            CURRENT_PIECE[0][1] -= 1
            CURRENT_PIECE[1][0] += 1
            CURRENT_PIECE[2][0] -= 1
        # Orientation 2
        # 1 0
        # 2
        elif CURRENT_ORIENTATION == 2:
            CURRENT_ORIENTATION = 1
            CURRENT_PIECE[0][0] -= 1
            CURRENT_PIECE[1][1] -= 1
            CURRENT_PIECE[2][1] += 1
        # Orientation 3
        #   1
        # 2 0
        elif CURRENT_ORIENTATION == 3:
            CURRENT_ORIENTATION = 2
            CURRENT_PIECE[0][1] += 1
            CURRENT_PIECE[1][0] -= 1
            CURRENT_PIECE[2][0] += 1
    if CURRENT_PIECE_TYPE == 'I':
        # Orientation 0
        # 2 0 1
        if CURRENT_ORIENTATION == 0:
            CURRENT_ORIENTATION = 1
            CURRENT_PIECE[1][0] -= 1
            CURRENT_PIECE[1][1] -= 1
            CURRENT_PIECE[2][0] += 1
            CURRENT_PIECE[2][1] += 1
        # Orientation 1
        # 2
        # 0
        # 1
        elif CURRENT_ORIENTATION == 1:
            CURRENT_ORIENTATION = 0
            CURRENT_PIECE[1][0] += 1
            CURRENT_PIECE[1][1] += 1
            CURRENT_PIECE[2][0] -= 1
            CURRENT_PIECE[2][1] -= 1

def rotate_cw():
    """
    Rotate current piece clockwise.
    """

    global CURRENT_PIECE, CURRENT_PIECE_TYPE, CURRENT_ORIENTATION

    if CURRENT_PIECE_TYPE == 'L':
        # Orientation 0
        # 2
        # 0 1
        if CURRENT_ORIENTATION == 0:
            CURRENT_ORIENTATION = 1
            CURRENT_PIECE[0][1] += 1
            CURRENT_PIECE[1][0] -= 1
            CURRENT_PIECE[2][0] += 1
        # Orientation 1
        # 0 2
        # 1
        elif CURRENT_ORIENTATION == 1:
            CURRENT_ORIENTATION = 2
            CURRENT_PIECE[0][0] += 1
            CURRENT_PIECE[1][1] += 1
            CURRENT_PIECE[2][1] -= 1
        # Orientation 2
        # 1 0
        # 2
        elif CURRENT_ORIENTATION == 2:
            CURRENT_ORIENTATION = 3
            CURRENT_PIECE[0][1] -= 1
            CURRENT_PIECE[1][0] += 1
            CURRENT_PIECE[2][0] -= 1
        # Orientation 3
        #   1
        # 2 0
        elif CURRENT_ORIENTATION == 3:
            CURRENT_ORIENTATION = 0
            CURRENT_PIECE[0][0] -= 1
            CURRENT_PIECE[1][1] -= 1
            CURRENT_PIECE[2][1] += 1
    if CURRENT_PIECE_TYPE == 'I':
        # Orientation 0
        # 2 0 1
        if CURRENT_ORIENTATION == 0:
            CURRENT_ORIENTATION = 1
            CURRENT_PIECE[1][0] -= 1
            CURRENT_PIECE[1][1] -= 1
            CURRENT_PIECE[2][0] += 1
            CURRENT_PIECE[2][1] += 1
        # Orientation 1
        # 2
        # 0
        # 1
        elif CURRENT_ORIENTATION == 1:
            CURRENT_ORIENTATION = 0
            CURRENT_PIECE[1][0] += 1
            CURRENT_PIECE[1][1] += 1
            CURRENT_PIECE[2][0] -= 1
            CURRENT_PIECE[2][1] -= 1

def move_piece():
    """
    Move the piece left and right on the game board.
    """

    global MOVE_TIMEOUT, CURRENT_PIECE, CURRENT_PIECE_TYPE, CURRENT_ORIENTATION, PIXELS

    clear_buffer()

    if MOVE_TIMEOUT >= 75:
        if KEYS[KEY_DOWN]:
            # Move the piece to the "left".
            for i in range(len(CURRENT_PIECE)):
                CURRENT_PIECE[i][1] -= 1
            # Check bounds and collisions on our new location.
            for i in range(len(CURRENT_PIECE)):
                if CURRENT_PIECE[i][1] < 0 or PIXELS[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1]]:
                    # Collision on the left, scoot everything to the right
                    for j in range(len(CURRENT_PIECE)):
                        CURRENT_PIECE[j][1] += 1
            MOVE_TIMEOUT = 0
        if KEYS[KEY_UP]:
            # Move the piece to the "right".
            for i in range(len(CURRENT_PIECE)):
                CURRENT_PIECE[i][1] += 1
            # Check bounds and collisions on our new location.
            for i in range(len(CURRENT_PIECE)):
                if CURRENT_PIECE[i][1] >= BOARD_HEIGHT or PIXELS[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1]]:
                    # Collision on the right, scoot everything to the left
                    for j in range(len(CURRENT_PIECE)):
                        CURRENT_PIECE[j][1] -= 1
            MOVE_TIMEOUT = 0
        if KEYS[KEY_LEFT]:
            # Rotate counterclockwise
            rotate_ccw()
            if collide_with_edges() or collide_with_pixels():
                # If we collided with something, don't allow the rotation
                rotate_cw()
            MOVE_TIMEOUT = 0
        if KEYS[KEY_RIGHT]:
            # Rotate clockwise
            rotate_cw()
            if collide_with_edges() or collide_with_pixels():
                # If we collided with something, don't allow the rotation
                rotate_ccw()
            MOVE_TIMEOUT = 0
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
        PIXELS[CURRENT_PIECE[i][0] - 1][CURRENT_PIECE[i][1]] = 1

def get_new_piece():
    """
    Generate a new Tetris piece.
    """

    global CURRENT_PIECE, CURRENT_PIECE_TYPE, CURRENT_ORIENTATION, PIECES, INDEX

    INDEX += 1
    INDEX %= len(PIECES)
    CURRENT_PIECE = [[p[0] + START_X, p[1] + START_Y] for p in PIECES[INDEX][1]]
    CURRENT_PIECE_TYPE = PIECES[INDEX][0]
    CURRENT_ORIENTATION = 0
    check_gameover()

def collide_with_edges():
    """
    Check for collisions between the current piece and the edges of the game board.
    """

    global CURRENT_PIECE

    for i in range(len(CURRENT_PIECE)):
        if CURRENT_PIECE[i][0] < 0 or CURRENT_PIECE[i][0] >= BOARD_WIDTH:
            return True
        if CURRENT_PIECE[i][1] < 0 or CURRENT_PIECE[i][1] >= BOARD_HEIGHT:
            return True
    return False

def collide_with_bottom():
    """
    Check for collisions between the current piece and the bottom of the game board.
    """

    global CURRENT_PIECE

    for i in range(len(CURRENT_PIECE)):
        if CURRENT_PIECE[i][0] >= BOARD_WIDTH:
            # The piece has hit the bottom of the screen
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
            return True
    return False

def handle_collision():
    """
    Freeze a piece, get a new piece, and clear completed rows. To be called upon a collision.
    """

    convert_to_pixels()
    get_new_piece()
    clear_rows()

def drop_pieces():
    """
    Drop the current piece down on the game board over time.
    """

    global DROP_TIMEOUT, DROP_INTERVAL, DROP_SPEEDUP_TIMEOUT, CURRENT_PIECE, PIXELS

    if DROP_TIMEOUT >= DROP_INTERVAL:
        DROP_TIMEOUT = 0

        for i in range(len(CURRENT_PIECE)):
            CURRENT_PIECE[i][0] += 1

        # Try colliding the current piece with the bottom of the screen and with other pieces
        if collide_with_bottom():
            handle_collision()
        elif collide_with_pixels():
            handle_collision()
    else:
        DROP_TIMEOUT += 1

    # Draw current piece
    for i in range(len(CURRENT_PIECE)):
        SCREEN_BUFFER[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1]] = 1

    # Draw pixels
    for i in range(len(PIXELS)):
        for j in range(len(PIXELS[0])):
            SCREEN_BUFFER[i][j] += PIXELS[i][j]

    if DROP_SPEEDUP_TIMEOUT >= 8000:
        DROP_INTERVAL = max(DROP_INTERVAL - 25, 50)
        DROP_SPEEDUP_TIMEOUT = 0
    else:
        DROP_SPEEDUP_TIMEOUT += 1

def completed_row(row):
    """
    Checks if a particular row on the game board is completed.
    """

    global PIXELS

    for i in range(BOARD_HEIGHT):
        if not PIXELS[row][i]:
            return False
    return True

def clear_rows():
    """
    Clear all completed rows on the game board.
    """

    global PIXELS

    row = BOARD_WIDTH - 1
    while True:
        if completed_row(row):
            # Clear the row and move everything down
            for r in range(row, 1, -1):
                for c in range(0, BOARD_HEIGHT):
                    PIXELS[r][c] = PIXELS[r-1][c]
            # Reset row to 0 so we'll check all rows again
            row = 0

        row -= 1
        if row < 0:
            return

def check_gameover():
    """
    Checks if the game has ended.
    """

    global GAMEOVER

    if collide_with_pixels():
        print 'Game over!'
        GAMEOVER = True


if __name__ == "__main__":
    main()
