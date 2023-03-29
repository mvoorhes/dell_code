import pygame as pg
import math
from Settings import *

class Player:
    def __init__(self, x, y, game):
        self.game = game
        self.x = x
        self.y = y
        self.width = 20
        self.height = 20
        self.dx = 0
        self.dy = 0
        self.angle = 0

    def draw(self, display):
        pg.draw.rect(display, green, [self.x, self.y, self.width, self.height])

    def move(self):

        sin_a = math.sin(self.angle)
        cos_a = math.cos(self.angle)
        speed = PLAYER_SPEED * self.game.delta_time
        speed_sin = speed * sin_a
        speed_cos = speed * cos_a

        keys = pg.key.get_pressed()
        if keys[pg.K_UP]:
            self.dx += speed_cos
            self.dy += speed_sin
        if keys[pg.K_DOWN]:
            self.dx -= speed_cos
            self.dy -= speed_sin

        self.x += self.dx
        self.y += self.dy

        if keys[pg.K_LEFT]:
            self.angle 

        for event in pg.event.get():
            if event.type == pg.KEYDOWN and event.key == pg.K_UP:
                self.dy -= 1
            if event.type == pg.KEYDOWN and event.key == pg.K_DOWN:
                self.dy += 1
            if event.type == pg.KEYDOWN and event.key == pg.K_LEFT:
                self.dx -= 1
            if event.type == pg.KEYDOWN and event.key == pg.K_RIGHT:
                self.dx += 1

            if event.type == pg.KEYUP and (event.key == pg.K_UP or event.key == pg.K_DOWN):
                self.dy = 0
            if event.type == pg.KEYUP and (event.key == pg.K_LEFT or event.key == pg.K_RIGHT):
                self.dx = 0

        # keys = pg.key.get_pressed()
        # if keys[pg.K_UP]:
        #     self.dy -= 1
        # if keys[pg.K_DOWN]:
        #     self.dy += 1
        # if keys[pg.K_LEFT]:
        #     self.dx -= 1
        # if keys[pg.K_RIGHT]:
        #     self.dx += 1


        self.x += self.dx
        self.y += self.dy
        
        if self.x >= display_width:
            self.x = display_width - 20
        elif self.x < 0:
            self.x = 0
        
        if self.y >= display_height:
            self.y = display_height - 20
        elif self.y < 0:
            self.y = 0