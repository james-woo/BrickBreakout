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
	this->x = 0;
	this->y = 0;
	this->score = 0;
	this->lives = 3;
}

void Board::updateScore(int score) {
	this->score += score;
}

bool Board::updateLives(int life) {
	if(lives != 0) {
		this->lives += life;
		return false;
	} else {
		return true;
	}
}

void Board::createLevel() {
	for (int i = 0; i < BOARD_BRICK_WIDTH; ++i) {
        for (int j = 0; j < BOARD_BRICK_HEIGHT; ++j) {
            Brick brick;
            brick.x = i * BRICK_WIDTH + i + BRICK_OFFSET_X;
            brick.y = j * BRICK_HEIGHT + j + BRICK_OFFSET_Y;
            brick.hardness = level[i][j];   // Random color
            brick.state = true;         // Brick is present
            bricks[i][j] = brick;
        }
    }
}
