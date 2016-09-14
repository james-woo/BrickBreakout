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

    void set_x(int x) { x_ = x; }
    void set_y(int y) { y_ = y; }
    int get_x() { return x_; }
    int get_y() { return y_; }
    int get_score() { return score_; }
    int get_lives() { return lives_; }
    Brick* get_brick(int i, int j) { return &bricks_[i][j]; }
	void update_score(int score);
	bool update_lives(int life);
	void create_level();

private:
	// Board position
	int x_;
	int y_;

	int score_;
	int lives_;

	Brick bricks_[BOARD_BRICK_WIDTH][BOARD_BRICK_HEIGHT];
};

#endif // BOARD_H_