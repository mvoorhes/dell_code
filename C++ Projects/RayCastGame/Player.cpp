#include <iostream>
#include <math.h>

#include "Player.h"

Player::Player(float _x, float _y, float _a) {
    x = _x;
    y = _y;
    angle = _a;
}

void Player::move(char key) {
    if (key == 'a') {
        x -= 1;
    } else if (key == 'd') {
        x += 1;
    } else if (key == 'w') {
        y -= 1;
    } else if (key == 's') {
        y += 1;
    }
}

void Player::draw() {
    return;
}