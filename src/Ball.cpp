#include "Ball.h"
#include "Paddle.h"
#include "Board.h"
#include "GameEngine.h"

#include <cmath>
#include <random>
#include <iostream>

namespace {
    std::random_device rd;
    std::mt19937 gen(rd());
}

Ball::Ball(int x, int y) {
	this->x = x;
	this->y = y;

	status = READY;
	dx = 0;
	dy = 0;
	bounce = false;
	speed = 8;
	angle = 0.0f;
	bounceCount = 0;
}

void Ball::launchBall() {

	// Direction towards either paddle, -1 or 1
	std::uniform_int_distribution<int> dir(0,1);
	int direction = 1 + (-2) * (dir(gen)%2);

	// Angle between 45 and 135
	std::uniform_int_distribution<int> ang(45,135);
	angle = ang(gen);

	dx = (int) (direction * speed * std::cos(angle * M_PI/180.0f));
	dy = (int) (speed * std::sin(angle * M_PI/180.0f));

	status = LAUNCHED;
}

void Ball::updateSpeed() {
	// After 5 bounces, increase speed
	if (bounceCount == 5) {
		speed++;
		bounceCount = 0;
	}
}

bool Ball::collidesWithTop() const {
	return ((y + dy) < BOARD_HEIGHT + BALL_DIAMETER);
}

bool Ball::collidesWithSides() const {
	return ((x + dx) < 0) || 
	       ((x + BALL_DIAMETER + dx) > SCREEN_WIDTH);
}

bool Ball::collidesWithPaddle(Paddle* paddle) const {
	if ( x + BALL_DIAMETER > paddle->getX() && x < paddle->getX() + PADDLE_WIDTH &&
         y + BALL_DIAMETER > paddle->getY() && y < paddle->getY() + PADDLE_HEIGHT ) {
        return true;
    }
    return false;
}

bool Ball::collidesWithBrick(Brick* brick) const {
	if (!brick->state) return false;
	if ( x + BALL_DIAMETER > brick->x && x < brick->x + BRICK_WIDTH &&
         y + BALL_DIAMETER > brick->y && y < brick->y + BRICK_HEIGHT ) {
		return true;
	}
	return false;
}

void Ball::bounceOffPaddle(Paddle* paddle) {
	if(paddle == nullptr) return;
	bounceCount++;

	int direction = (x < (paddle->getX() + (PADDLE_WIDTH / 2) - (BALL_DIAMETER / 2))) ? 1 : -1;

	// Calculation for ball's new trajectory
	// Closer to the edges give larger dx
    int relativeX = (x - paddle->getX() + BALL_DIAMETER);

    // Ranges from 0 ~ 70 follows this parabola: (7/100)(x-35)^2 + 90
    angle = (0.07f * (relativeX - 35)*(relativeX - 35) + 90);
	
	// Speed on both axis
    dx = (int) (direction * speed * std::cos(angle * M_PI/180.0f));
	dy = (int) (speed * std::sin(angle * M_PI/180.0f));
}

void Ball::bounceOffBrick(Brick* brick) {
	dy = (int) (speed * std::sin(-1 * angle * M_PI/180.0f));
	if(--brick->hardness < 1) {
		brick->state = false;
	}
}

bool Ball::outOfBounds() {
	return y > SCREEN_HEIGHT || (x > SCREEN_WIDTH);
}

void Ball::reset() {
	y = SCREEN_HEIGHT - 30;
	speed = 8;
	bounceCount = 0;
	dx = 0;
	dy = 0;

	status = READY;
}