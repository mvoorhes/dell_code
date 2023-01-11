import pygame
import random

pygame.init()

white = (255, 255, 255)
black = (0, 0, 0)
red = (255, 0, 0)
green = (0, 255, 0)
blue = (0, 0, 255)
yellow = (255, 255, 51)

RGB = [red, green, blue]

dis_width = 800
dis_height = 600

dis = pygame.display.set_mode((dis_width, dis_height))
pygame.display.set_caption("Worse than dark souls")

block = 20
speed = 60

bullets = []
enemies = []
bonuses = []
bosses = []


class Player:
    def __init__(self, x, y, play_speed):
        self.x = x
        self.y = y
        self.width = block
        self.height = block
        self.speed = play_speed
        self.change = 0
        self.color = green

    def draw(self, disp):
        pygame.draw.rect(disp, self.color, [self.x, self.y, self.width, self.height])

    def move(self):
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN and event.key == pygame.K_LEFT:
                self.change = -self.speed
            elif event.type == pygame.KEYDOWN and event.key == pygame.K_RIGHT:
                self.change = self.speed
            else:
                self.change = 0
        self.x += self.change
        if self.x >= dis_width:
            self.x = dis_width
        elif self.x < 0:
            self.x = 0

    def shoot(self):
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                if len(bullets) < 5:
                    bullets.append(Projectile(self.x, self.y))


class Enemy:
    def __init__(self, x, y, en_speed):
        self.x = x
        self.y = y
        self.width = block
        self.height = block
        self.dir = en_speed
        self.color = red

    def draw(self, disp):
        pygame.draw.rect(disp, self.color, [self.x, self.y, self.width, self.height])


class Projectile:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.radius = 10
        self.width = block
        self.height = block
        self.color = white

    def draw(self, disp):
        pygame.draw.circle(disp, self.color, (self.x, self.y), self.radius)


class PowerUp:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.width = block
        self.height = block
        self.color = blue

    def draw(self, disp):
        pygame.draw.rect(disp, self.color, [self.x, self.y, self.width, self.height])


class Boss:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.width = 4 * block
        self.height = 4 * block
        self.hp = 80
        self.speed = 1
        self.color = yellow

    def draw(self, disp):
        pygame.draw.rect(disp, self.color, [self.x, self.y, self.width, self.height])


font_style = pygame.font.SysFont("bahnschrift", 30)
score_font = pygame.font.SysFont("helvetica", 30)


def message(msg, color):
    mesg = font_style.render(msg, True, color)
    dis.blit(mesg, [dis_width / 2, dis_height / 2])


def yourScore(score):
    value = score_font.render("Your Score: " + str(score), True, red)
    dis.blit(value, [0, 0])


def lives(live):
    value = score_font.render("Lives: " + str(live), True, red)
    dis.blit(value, [700, 0])


def changeColor(color):
    if color == blue:
        return red
    elif color == red:
        return green
    elif color == green:
        return blue


def redrawGameWindow(player, score, live):
    dis.fill(black)
    player.draw(dis)
    yourScore(score)
    lives(live)
    for enemy in enemies:
        enemy.draw(dis)
    for bullet in bullets:
        bullet.draw(dis)
    for powerUp in bonuses:
        powerUp.draw(dis)
    for boss in bosses:
        boss.draw(dis)
    pygame.display.update()


def isColliding(thing1, thing2):
    isLeft = (thing2.x + thing2.width) < thing1.x
    isRight = (thing1.x + thing1.width) < thing2.x
    isBelow = (thing2.y + thing2.height) < thing1.y
    isAbove = (thing1.y + thing1.height) < thing2.y

    collide = not (isLeft or isRight or isBelow or isAbove)

    return collide


clock = pygame.time.Clock()


def gameLoop():
    game_over = False
    game_close = False

    x1 = dis_width / 2
    y1 = dis_height * (7 / 8)

    x1_change = 0

    time = 0
    special = 0
    score = 0
    life = 3

    player_speed = 7
    enemy_speed = 3
    bullet_speed = 6
    powerUp_speed = 5

    shoot = False

    player = Player(x1, y1, player_speed)
    while not game_over:
        while game_close:
            dis.fill(black)
            message("You lost bucko", red)
            yourScore(score)
            pygame.display.update()

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    game_over = True
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q:
                        game_over = True
                        game_close = False
                    elif event.key == pygame.K_c:
                        enemies.clear()
                        bullets.clear()
                        score = 0
                        life = 3
                        gameLoop()

        for bullet in bullets:
            if bullet.y > 0:
                bullet.y -= bullet_speed
            else:
                bullets.pop(bullets.index(bullet))

            for enemy in enemies:
                bul_en = isColliding(bullet, enemy)
                if bul_en:
                    enemies.remove(enemy)
                    if bullet in bullets:
                        bullets.remove(bullet)
                    score += 10

            for boss in bosses:
                bul_boss = isColliding(bullet, boss)
                if bul_boss:
                    if bullet in bullets:
                        bullets.remove(bullet)
                    boss.hp -= 5
                    if boss.hp == 0:
                        bosses.remove(boss)
                        score += 100

        for enemy in enemies:
            enemy.y += enemy_speed
            enemy.x += enemy.dir
            if enemy.x >= dis_width or enemy.x < 0:
                enemy.dir = -enemy.dir

            play_en = isColliding(player, enemy)
            if play_en or enemy.y >= dis_height:
                enemies.remove(enemy)
                life -= 1
            if life == 0:
                game_close = True

        for boss in bosses:
            boss.y += boss.speed
            play_boss = isColliding(player, boss)
            if play_boss or boss.y >= dis_height:
                bosses.remove(boss)
                life = 0
                game_close = True

        for powerUp in bonuses:
            powerUp.y += powerUp_speed
            powerUp.color = changeColor(powerUp.color)
            play_pow = isColliding(player, powerUp)
            if play_pow:
                life += 1
                bonuses.remove(powerUp)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                game_over = True

            if event.type == pygame.KEYDOWN and event.key == pygame.K_LEFT:
                x1_change = -player.speed
            elif event.type == pygame.KEYDOWN and event.key == pygame.K_RIGHT:
                x1_change = player.speed

            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                bullets.append(Projectile(player.x, player.y))
                shoot = True
            elif event.type == pygame.KEYUP and event.key == pygame.K_SPACE:
                shoot = False

            if event.type == pygame.KEYUP and (event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT):
                x1_change = 0

        player.x += x1_change
        if player.x >= dis_width - 20:
            player.x = dis_width - 20
        elif player.x < 0:
            player.x = 0

        time += 1

        if shoot and time % 15 == 0:
            bullets.append(Projectile(player.x, player.y))

        if time % speed == 0:
            asteroid_x = round(random.randrange(0, dis_width - block) / 10) * 10
            asteroid_y = 0
            enemy = Enemy(asteroid_x, asteroid_y, enemy_speed)
            enemies.append(enemy)
            special += 1

        if special % 5 == 0 and time % speed == 0:
            asteroid_x = round(random.randrange(0, dis_width - block) / 10) * 10
            asteroid_y = 0
            powerUp = PowerUp(asteroid_x, asteroid_y)
            bonuses.append(powerUp)

        if score % 250 == 0 and score != 0 and len(bosses) < 1:
            boss_x = 400
            boss_y = 0
            boss = Boss(boss_x, boss_y)
            bosses.append(boss)

        redrawGameWindow(player, score, life)

        clock.tick(speed)

    pygame.quit()
    quit()


gameLoop()
