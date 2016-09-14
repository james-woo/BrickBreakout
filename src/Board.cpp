#include "Board.h"
#include <iostream>
#include <cstdlib>
#include <random>
#include <string>

namespace {
    std::random_device rd;
    std::mt19937 gen(rd());
}

int level[BOARD_BRICK_WIDTH][BOARD_BRICK_HEIGHT] = 
{ 
   { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1},
   { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1},
   { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1} 
};

Board::Board() {
	this->x_ = 0;
	this->y_ = 0;
	this->score_ = 0;
	this->lives_ = 3;
}

void Board::update_score(int score) {
	this->score_ += score;
}

bool Board::update_lives(int life) {
	if(lives_ != 0) {
		this->lives_ += life;
		return false;
	} else {
		return true;
	}
}

void Board::create_level() {
	for (int i = 0; i < BOARD_BRICK_WIDTH; ++i) {
        for (int j = 0; j < BOARD_BRICK_HEIGHT; ++j) {
            Brick brick;
            brick.x = i * BRICK_WIDTH + i + BRICK_OFFSET_X;
            brick.y = j * BRICK_HEIGHT + j + BRICK_OFFSET_Y;
            brick.hardness = level[i][j];   // Random color
            brick.state = true;         // Brick is present
            bricks_[i][j] = brick;
        }
    }
}
