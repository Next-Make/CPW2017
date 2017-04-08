"""
Simplified version of snake (wormy) in which you move
the snake around without colliding into itself or the walls.
"""

from __future__ import division

import sys
import pygame


class Square(object):
    """
    Class for keeping track of the snake's location.
    """

    def __init__(self, x, y, ahead=None, behind=None):
        """
        Initialize a Square instance.

        :param x: The x-index of the square.
        :param y: The y-index of the square.
        :param ahead: The square "ahead" of this one in the snake's body.
        :param behind: The square "behind" of this one in the snake's body.
        """

        self.x = x
        self.y = y

        self.ahead = ahead
        self.behind = behind

        if ahead is not None:
            ahead.behind = self

        if behind is not None:
            behind.ahead = self

    def set_ahead(self, other):
        """
        Set the ahead attribute of a Square instance.

        In addition, we set the behind parameter of other to self.
        This is the preferred way to set the ahead attribute instead
        of directly setting it because of that extra step.

        :param other: The Square instance ahead of self.
        """

        self.ahead = other

        if other is not None:
            other.behind = self

    def set_behind(self, other):
        """
        Set the ahead attribute of a Square instance.

        In addition, we set the behind parameter of other to self.
        This is the preferred way to set the ahead attribute instead
        of directly setting it because of that extra step.

        :param other: The Square instance behind of self.
        """

        self.ahead = other

        if other is not None:
            other.behind = self

    def destroy(self):
        """
        Remove this current square from the body.
        """

        ahead = self.ahead
        behind = self.behind

        if ahead is not None and behind is not None:
            ahead.behind = behind
            behind.ahead = ahead
        elif ahead is not None:
            ahead.behind = None
        elif behind is not None:
            behind.ahead = None

    def __str__(self):
        descr = "({x}, {y})".format(x=self.x, y=self.y)

        if self.ahead is not None or self.behind is not None:
            descr = "*" + descr

            if self.ahead is not None:
                ahead = "({x}, {y})".format(x=self.ahead.x, y=self.ahead.y)
                descr += "->" + ahead

            if self.behind is not None:
                behind = "({x}, {y})".format(x=self.behind.x, y=self.behind.y)
                descr = behind + "->" + descr

        return descr

    __repr__ = __str__


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

# Game global variables
SURFACE = None
SCREEN_BUFFER = None

# Game state variables
CONFIG = 0
UPDATE = 1
PAUSED = 2
RESTART = 3

GAME_STATE = CONFIG

# Direction variable
UP = 0
DOWN = 1
LEFT = 2
RIGHT = 3

DIRECTION = LEFT
DIRECTIONS = (LEFT, RIGHT, UP, DOWN)


# Initial snake location
def init_snake():
    """
    Initialize the HEAD and TAIL of a length-two snake.

    :return: Tuple of (head, tail) indicating the head and tail of the snake.
    """

    head = Square(5, 4)
    tail = Square(6, 4, ahead=head)

    return head, tail


HEAD, TAIL = init_snake()
SNAKE_LENGTH = 2

# Configuration variables that are
# used for the entire duration of
# a game (or series of games).

# Handedness configuration variables.
LEFT_HANDED = 0
RIGHT_HANDED = 1

LEFT_KEYS = {pygame.K_w, pygame.K_s, pygame.K_a, pygame.K_d}
RIGHT_KEYS = {pygame.K_UP, pygame.K_DOWN, pygame.K_LEFT, pygame.K_RIGHT}

HANDEDNESS = -1

UP_KEY = -1
DOWN_KEY = -1
LEFT_KEY = -1
RIGHT_KEY = -1

PAUSE_KEY = -1
SOFT_RESET_KEY = -1
HARD_RESET_KEY = -1

# Variables for determining when we should extend the snake.
EXTEND = False
EXTEND_DELAY = 0
EXTEND_COUNTER = 0


def main():
    global EXTEND_DELAY, GAME_STATE, SCREEN_BUFFER, SURFACE

    pygame.init()

    title = "Snake Game"
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

    while True:
        if GAME_STATE == CONFIG:
            await_configuration()
        elif GAME_STATE == UPDATE:
            update_game()
        elif GAME_STATE == PAUSED:
            await_restart(reset=False)
        elif GAME_STATE == RESTART:
            await_restart(reset=True)


def await_configuration():
    """
    Wait for the user to decide which hand to use before starting.
    """

    global GAME_STATE, HANDEDNESS, UP_KEY, DOWN_KEY, LEFT_KEY
    global RIGHT_KEY, PAUSE_KEY, SOFT_RESET_KEY, HARD_RESET_KEY

    draw_snake()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.display.quit()
                sys.exit(0)
            elif event.key in LEFT_KEYS:
                GAME_STATE = UPDATE
                HANDEDNESS = LEFT_HANDED

                UP_KEY = pygame.K_w
                DOWN_KEY = pygame.K_s
                LEFT_KEY = pygame.K_a
                RIGHT_KEY = pygame.K_d

                PAUSE_KEY = pygame.K_RIGHT
                SOFT_RESET_KEY = pygame.K_UP
                HARD_RESET_KEY = pygame.K_DOWN
            elif event.key in RIGHT_KEYS:
                GAME_STATE = UPDATE
                HANDEDNESS = RIGHT_HANDED

                UP_KEY = pygame.K_UP
                DOWN_KEY = pygame.K_DOWN
                LEFT_KEY = pygame.K_LEFT
                RIGHT_KEY = pygame.K_RIGHT

                PAUSE_KEY = pygame.K_a
                SOFT_RESET_KEY = pygame.K_w
                HARD_RESET_KEY = pygame.K_s


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


def reset_game(hard=False):
    """
    Reset the game state for a fresh game.

    In both types of reset, we reset the snake. A hard reset also includes
    resetting the game state to the user configuration state.

    :param hard: Whether or not we want to do a hard reset.
    """

    global GAME_STATE, DIRECTION, HANDEDNESS, HEAD, TAIL

    HEAD, TAIL = init_snake()
    GAME_STATE = UPDATE
    DIRECTION = LEFT
    draw_snake()

    if hard:
        HANDEDNESS = -1
        GAME_STATE = CONFIG


def update_screen():
    """
    Update the screen displayed to the user.
    """

    draw_screen()
    pygame.display.flip()
    SURFACE.fill(BLACK)


def await_restart(reset):
    """
    Await for the user to restart the game.

    :param reset: Whether or not to reset the game if restarted.
    """

    global GAME_STATE

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.display.quit()
                sys.exit(0)
            elif event.key in LEFT_KEYS or event.key in RIGHT_KEYS:
                if reset:
                    reset_game(hard=False)
                else:
                    GAME_STATE = UPDATE


def draw_snake():
    """
    Draw the snake on the screen buffer.
    """

    clear_buffer()
    square = HEAD

    while square is not None:
        SCREEN_BUFFER[square.x][square.y] = 2
        square = square.behind

    update_screen()


def update_game():
    """
    Update direction and snake position.
    """

    global EXTEND_DELAY

    update_direction()

    # We could have paused the game.
    if GAME_STATE != UPDATE:
        return

    if EXTEND_DELAY > 150000:#75000:
        EXTEND_DELAY = 0
        update_snake()
    else:
        EXTEND_DELAY += 1


def update_direction():
    """
    Update the direction of the snake, if necessary.
    """

    global DIRECTION, GAME_STATE

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.display.quit()
                sys.exit(0)
            elif event.key == LEFT_KEY:
                DIRECTION = LEFT
            elif event.key == RIGHT_KEY:
                DIRECTION = RIGHT
            elif event.key == UP_KEY:
                DIRECTION = UP
            elif event.key == DOWN_KEY:
                DIRECTION = DOWN
            elif event.key == PAUSE_KEY:
                GAME_STATE = PAUSED
            elif event.key == SOFT_RESET_KEY:
                reset_game(hard=False)
            elif event.key == HARD_RESET_KEY:
                reset_game(hard=True)


def get_adjacent(x, y, direction):
    """
    Get the coordinate of the square adjacent in the provided direction.

    :param x: The x-coordinate of the square provided.
    :param y: The y-coordinate of the square provided.
    :param direction: The direction in which to find the adjacent square.
    :return: Tuple of (new_x, new_y) indicating the adjacent square.
             Note that the values can be None.
    """

    if direction == LEFT and x > 0:
        return x - 1, y
    elif direction == RIGHT and x < 6:
        return x + 1, y
    elif direction == DOWN and y > 0:
        return x, y - 1
    elif direction == UP and y < 4:
        return x, y + 1
    else:
        return None, None


def is_clear(x, y):
    """
    Check if a square in the screen buffer is cleared.

    :param x: The x-coordinate of the screen buffer square.
    :param y: The y-coordinate of the screen buffer square.
    :return: Whether or not if the screen buffer is unoccupied.
             If the square is invalid, we just return False.
    """

    if not 0 <= x <= 6:
        return False
    if not 0 <= y <= 4:
        return False

    return SCREEN_BUFFER[x][y] == 0


def extend_head(new_x, new_y):
    """
    Extend the HEAD of the snake, if possible.

    :param new_x: The x-coordinate of the new snake HEAD.
    :param new_y: The y-coordinate of the new snake HEAD.
    :return: Whether or not the extension was successful.
    """

    global HEAD
    success = False

    if new_x is not None and new_y is not None:
        if is_clear(new_x, new_y):
            success = True
            HEAD = Square(new_x, new_y, behind=HEAD)

    return success


def extend_tail(new_x, new_y):
    """
    Extend the TAIL of the snake, if possible.

    :param new_x: The x-coordinate of the new snake TAIL.
    :param new_y: The y-coordinate of the new snake TAIL.
    :return: Whether or not the extension was successful.
    """

    global TAIL
    success = False

    if new_x is not None and new_y is not None:
        if is_clear(new_x, new_y):
            success = True
            TAIL = Square(new_x, new_y, ahead=TAIL)

    return success


def shorten_tail():
    """
    Shorten the TAIL of the snake.
    """

    global TAIL

    TAIL = TAIL.ahead
    TAIL.behind = None


def update_snake():
    """
    Update the position of the snake on the screen.
    """

    global EXTEND, EXTEND_COUNTER, GAME_STATE, HEAD, SNAKE_LENGTH, TAIL

    if EXTEND_COUNTER > 21:
        EXTEND = True
        EXTEND_COUNTER = 0
    else:
        EXTEND_COUNTER += 1

    extended = False

    head_x, head_y = HEAD.x, HEAD.y
    tail_x, tail_y = TAIL.x, TAIL.y

    if EXTEND:
        # We extend at the HEAD if possible in the direction
        # we are moving the snake. Otherwise, we attempt to
        # add at the TAIL in whichever direction possible.
        new_head_x, new_head_y = get_adjacent(head_x, head_y, DIRECTION)
        extended = extend_head(new_head_x, new_head_y)

        if not extended:
            for direction in DIRECTIONS:
                new_head_x, new_head_y = get_adjacent(
                    head_x, head_y, direction)
                extended = extend_head(new_head_x, new_head_y)

                if extended:
                    break

        if not extended:
            for direction in DIRECTIONS:
                new_tail_x, new_tail_y = get_adjacent(
                    tail_x, tail_y, direction)
                extended = extend_tail(new_tail_x, new_tail_y)

                if extended:
                    break

        if extended:
            EXTEND = False
            SNAKE_LENGTH += 1

    if not extended:
        new_head_x, new_head_y = get_adjacent(head_x, head_y, DIRECTION)
        extended = extend_head(new_head_x, new_head_y)

        if extended:
            shorten_tail()

    if not extended:
        new_length = int(SNAKE_LENGTH / 2)

        if new_length < 2:
            GAME_STATE = RESTART
        else:
            while SNAKE_LENGTH > new_length:
                shorten_tail()
                SNAKE_LENGTH -= 1

    draw_snake()


if __name__ == "__main__":
    main()
