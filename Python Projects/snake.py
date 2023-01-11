import pygame
import time
import random

pygame.init()

white = (255,255,255)
black = (0,0,0)
blue = (0,0,255)
red = (255,0,0)
green = (0,255,0)

dis_width = 800
dis_height = 600

dis = pygame.display.set_mode((dis_width, dis_height))
pygame.display.set_caption("Snek")

clock = pygame.time.Clock()

block = 10
speed = 15

font_style = pygame.font.SysFont(None, 20)
score_font = pygame.font.SysFont(None, 20)


def your_score(score):
    value = score_font.render("Your Score: " + str(score), True, red)
    dis.blit(value, [0, 0])


def our_snake(block, list):
    for x in list:
        pygame.draw.rect(dis, black, [x[0], x[1], block, block])


def message(msg, color):
    mesg = font_style.render(msg, True, color)
    dis.blit(mesg, [dis_width / 6, dis_height / 3])


def game_loop():
    game_over = False
    game_close = False

    x1 = dis_width / 2
    y1 = dis_height / 2

    x1_change = 0
    y1_change = 0

    snake_list = []
    snake_len = 1

    foodx = round(random.randrange(0, dis_width - block) / 10) * 10
    foody = round(random.randrange(0, dis_height - block) / 10) * 10

    while not game_over:

        while game_close:
            dis.fill(white)
            message("You lost you piece of shit. FUck you", red)
            pygame.display.update()

            for event in pygame.event.get():
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q:
                        game_over = True
                        game_close = False
                    elif event.key == pygame.K_c:
                        game_loop()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                game_over = True
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    x1_change = -block
                    y1_change = 0
                elif event.key == pygame.K_RIGHT:
                    x1_change = block
                    y1_change = 0
                elif event.key == pygame.K_UP:
                    x1_change = 0
                    y1_change = -block
                elif event.key == pygame.K_DOWN:
                    x1_change = 0
                    y1_change = block

        if x1 >= dis_width or x1 < 0:
            game_close = True
        if y1 >= dis_height or y1 < 0:
            game_close = True

        x1 += x1_change
        y1 += y1_change
        dis.fill(white)

        pygame.draw.rect(dis, green, [foodx, foody, block, block])

        snake_head = []
        snake_head.append(x1)
        snake_head.append(y1)
        snake_list.append(snake_head)
        if len(snake_list) > snake_len:
            del snake_list[0]

        for x in snake_list[:-1]:
            if x == snake_head:
                game_close = True

        our_snake(block, snake_list)
        your_score(snake_len - 1)

        pygame.display.update()

        if x1 == foodx and y1 == foody:
            foodx = round(random.randrange(0, dis_width - block) / 10) * 10
            foody = round(random.randrange(0, dis_height - block) / 10) * 10
            snake_len += 1

        clock.tick(speed)

    pygame.quit()
    quit()


game_loop()
