import pygame
from Settings import *
from Player import *

pygame.init()



display = pygame.display.set_mode((display_width, display_height))
pygame.display.set_caption("Test")


def drawGameWindow(player):
    player.move()
    display.fill(black)
    player.draw(display)
    pygame.display.update()


def gameLoop():
    game_over = False

    initial_x = display_width / 2
    initial_y = display_height / 2

    player = Player(initial_x, initial_y)

    while not game_over:

        drawGameWindow(player)


    pygame.quit()
    quit()


gameLoop()