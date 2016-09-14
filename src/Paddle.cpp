#include "Paddle.h"
#include "GameEngine.h"

Paddle::Paddle(int x, int y) {
	x_ = x;
	y_ = y;
}

void Paddle::setX(int x) {
	x_ = x;
	if(x_ < 0) {
		x_ = 0;
	} else if (x_ + PADDLE_WIDTH > SCREEN_WIDTH) {
		x_ = SCREEN_WIDTH - PADDLE_WIDTH;
	}
}

void Paddle::addToX(int x) {
	x_ += x;
	if(x_ < 0) {
		x_ = 0;
	} else if (x_ + PADDLE_WIDTH > SCREEN_WIDTH) {
		x_ = SCREEN_WIDTH - PADDLE_WIDTH;
	}
}