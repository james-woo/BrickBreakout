#include "Board.h"
#include <iostream>
#include <random>

namespace {
    std::random_device rd;
    std::mt19937 gen(rd());
}

int level[BOARD_BRICK_HEIGHT][BOARD_BRICK_WIDTH] =
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

Board::Board() {
	this->x_ = 0;
	this->y_ = 0;
	this->score_ = 0;
	this->lives_ = 3;
    this->gameover_ = false;
}

Board::~Board() {

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
            brick.hardness = level[j][i];   // Random color
            brick.state = true;         // Brick is present
            bricks_[i][j] = brick;
        }
    }
}

void Board::game_over() {
    this->gameover_ = true;
}

void Board::new_game() {
    this->score_ = 0;
    this->lives_ = 3;
    this->gameover_ = false;
    create_level();
}
