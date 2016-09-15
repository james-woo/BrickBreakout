#include "Ball.h"
#include "Paddle.h"
#include "Board.h"

#include <cmath>
#include <random>
#include <iostream>

namespace {
    std::random_device rd;
    std::mt19937 gen(rd());
}

Ball::Ball(int x, int y) {
	this->x_ = x;
	this->y_ = y;

	status_ = READY;
	dx_ = 0;
	dy_ = 0;
	speed_ = 8;
	angle_ = 0.0f;
	hits_ = 0;
}

void Ball::launch_ball() {

	// Direction towards either paddle, -1 or 1
	std::uniform_int_distribution<int> dir(0,1);
	int direction = 1 + (-2) * (dir(gen)%2);

	// Angle between 45 and 135
	std::uniform_int_distribution<int> ang(45,135);
	angle_ = ang(gen);

	dx_ = (int) (direction * speed_ * std::cos(angle_ * M_PI/180.0f));
	dy_ = (int) (speed_ * std::sin(angle_ * M_PI/180.0f));

	status_ = LAUNCHED;
}

void Ball::update_speed() {
	// After 5 bounces, increase speed
	if (hits_ == 5) {
		speed_++;
		hits_ = 0;
	}
}

bool Ball::collides_with_top() const {
	return ((y_ + dy_) < BOARD_HEIGHT + BALL_DIAMETER);
}

bool Ball::collides_with_sides() const {
	return ((x_ + dx_) < 0) ||
	       ((x_ + BALL_DIAMETER + dx_) > SCREEN_WIDTH);
}

bool Ball::collides_with_paddle(Paddle* paddle) const {
	if ( x_ + BALL_DIAMETER > paddle->get_x() && x_ < paddle->get_x() + PADDLE_WIDTH &&
         y_ + BALL_DIAMETER > paddle->get_y() && y_ < paddle->get_y() + PADDLE_HEIGHT ) {
        return true;
    }
    return false;
}

bool Ball::collides_with_brick(Brick *brick) const {
	if (!brick->state) return false;
	if ( x_ + BALL_DIAMETER > brick->x && x_ < brick->x + BRICK_WIDTH &&
         y_ + BALL_DIAMETER > brick->y && y_ < brick->y + BRICK_HEIGHT ) {
		return true;
	}
	return false;
}

void Ball::bounce_off_paddle(Paddle* paddle) {
	if(paddle == nullptr) return;
	hits_++;

	int direction = (x_ < (paddle->get_x() + (PADDLE_WIDTH / 2) - (BALL_DIAMETER / 2))) ? 1 : -1;

	// Calculation for ball's new trajectory
	// Closer to the edges give larger dx
    int relativeX = (x_ - paddle->get_x() + BALL_DIAMETER);

    // Ranges from 0 ~ 70 follows this parabola: (7/100)(x-35)^2 + 90
    angle_ = (0.08f * (relativeX - 35)*(relativeX - 35) + 90);
	
	// Speed on both axis
    dx_ = (int) (direction * speed_ * std::cos(angle_ * M_PI/180.0f));
	dy_ = (int) (speed_ * std::sin(angle_ * M_PI/180.0f));
}

void Ball::bounce_off_brick(Brick *brick) {
    if ( y_ < brick->y + BRICK_HEIGHT && (x_ < brick->x || x_ > brick->x + BRICK_WIDTH) ) {
        dx_ *= -1;
        dy_ *= -1;
    }
	dy_ = (int) (speed_ * std::sin(-1 * angle_ * M_PI/180.0f));
	if(--brick->hardness < 1) {
		brick->state = false;
	}
}

bool Ball::out_of_bounds() {
	return y_ > SCREEN_HEIGHT || (x_ > SCREEN_WIDTH);
}

void Ball::reset() {
	y_ = SCREEN_HEIGHT - 30;
	speed_ = 8;
	hits_ = 0;
	dx_ = 0;
	dy_ = 0;

	status_ = READY;
}