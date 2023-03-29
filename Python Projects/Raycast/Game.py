import pygame as pg
from Settings import *
from Player import *
from Map import *


class Game:
    def __init__(self):
        pg.init()
        self.screen = pg.display.set_mode((display_width, display_height))
        self.clock = pg.time.Clock()
        self.new_game()

    def new_game(self):
        self.player = Player(x_start, y_start, self)
        self.map = Map(self)

    def update(self):
        pg.display.flip()
        self.player.move()
        self.clock.tick(60)

    def draw(self):
        self.screen.fill(black)
        self.map.draw()
        self.player.draw(self.screen)
        pg.display.update()

    def check_events(self):
        for event in pg.event.get():
            if event.type == pg.QUIT or (event.type == pg.KEYDOWN and event.key == pg.K_ESCAPE):
                pg.quit()
                quit()

    def run(self):
        while True:
            self.check_events()
            self.update()
            self.draw()


game = Game()
game.run()


# pg.init()

# display = pg.display.set_mode((display_width, display_height))
# pg.display.set_caption("Test")


# def drawGameWindow(player):
#     player.move()
#     display.fill(black)
#     player.draw(display)
#     pg.display.update()


# def gameLoop():
#     game_over = False

#     initial_x = display_width / 2
#     initial_y = display_height / 2

#     player = Player(initial_x, initial_y)

#     while not game_over:

#         drawGameWindow(player)


#     pg.quit()
#     quit()


# gameLoop()