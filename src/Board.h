#ifndef BOARD_H_
#define BOARD_H_

#include "GameEngine.h"

const int BOARD_BRICK_WIDTH = 12;
const int BOARD_BRICK_HEIGHT = 4;
const int BRICK_WIDTH = 52;
const int BRICK_HEIGHT = 25;
const int BRICK_OFFSET_X = 2;
const int BRICK_OFFSET_Y = SCORE_SECTION_HEIGHT + 2;
const int BOARD_HEIGHT = SCORE_SECTION_HEIGHT + 2;

class Brick {
public:
	int x;
	int y;
	int hardness;
	bool state;
	void hit() { if (hardness > 0) hardness--; }
};

class Board {
public:
	Board();
	~Board();

	void updateScore(int score);
	bool updateLives(int life);
	void createLevel();

	// Board position
	int x;
	int y;

	int score;
	int lives;

	Brick bricks[BOARD_BRICK_WIDTH][BOARD_BRICK_HEIGHT];
};

#endif // BOARD_H_