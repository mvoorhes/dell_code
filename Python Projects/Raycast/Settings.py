import pygame as pg

display_width = 800
display_height = 600

x_start = display_width / 2
y_start = display_height / 2

PLAYER_ANGLE = 0
PLAYER_SPEED = 0.004
PLAYER_ROT_SPEED = 0.002

white = (255, 255, 255)
black = (0, 0, 0)
red = (255, 0, 0)
green = (0, 255, 0)
blue = (0, 0, 255)

RGB = [red, green, blue]

# Defining some shorter constants from pygame library
KEY_DOWN = pg.KEYDOWN
U = pg.K_UP
D = pg.K_DOWN
L = pg.K_LEFT
R = pg.K_RIGHT