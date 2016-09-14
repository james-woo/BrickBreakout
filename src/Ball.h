#ifndef BALL_H_
#define BALL_H_

class Paddle;
class Brick;

const int BALL_DIAMETER = 10;

class Ball {
public:
	Ball(int x, int y);
	~Ball() {}

	void launchBall();
	void updateSpeed();
	bool collidesWithTop() const;
	bool collidesWithSides() const;
	bool collidesWithPaddle(Paddle* paddle) const;
	bool collidesWithBrick(Brick* brick) const;
	void bounceOffPaddle(Paddle* paddle);
	void bounceOffBrick(Brick* brick);
	void reflect();
	bool outOfBounds();
	void reset();

	// Ball status
	enum Status { READY, LAUNCH, LAUNCHED };
	Status status;

	// Ball position
	int x;
	int y;

	// Ball movement
	int dx;
	int dy;
	bool bounce;
	int speed;
	float angle;
	int bounceCount;
};

#endif // BALL_H_