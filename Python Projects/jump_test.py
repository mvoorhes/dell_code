import pygame

pygame.init()

red = (255, 0, 0)
black = (0, 0, 0)

dis_width = 400
dis_height = 300

dis = pygame.display.set_mode((dis_width, dis_height))


class Player:
    def __init__(self):
        self.x = dis_width / 2
        self.y = dis_height - 20
        self.gravity = 7
        self.width = 20
        self.height = 20
        self.color = red

    def draw(self, disp):
        pygame.draw.rect(disp, self.color, [self.x, self.y, self.width, self.height])


# def redrawGameWindow(player):
#     dis.fill(black)
#     player.draw(dis)
#     pygame.display.update()

clock = pygame.time.Clock()


def gameLoop():
    gameOver = False

    player = Player()
    jump = False
    double = False
    jumpCount = player.gravity
    while not gameOver:

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                gameOver = True
            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                if jump:
                    double = True
                jump = True

        # keys = pygame.key.get_pressed()

        if double:
            jumpCount = player.gravity
            double = False

        if jump:
            if jumpCount >= -player.gravity or player.y < dis_height - 20:
                player.y -= (jumpCount * abs(jumpCount)) * 0.5
                jumpCount -= 1
            else:
                jumpCount = player.gravity
                jump = False

        if player.y >= dis_height - 20:
            player.y = dis_height - 20
        elif player.y < 0:
            player.y = 0

        dis.fill(black)
        player.draw(dis)
        pygame.display.update()

        clock.tick(60)

        # redrawGameWindow(player)

    pygame.quit()
    quit()


gameLoop()
