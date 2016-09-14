#ifndef PADDLE_H_
#define PADDLE_H_

const int PADDLE_HEIGHT = 10;
const int PADDLE_WIDTH = 60;

class Paddle {
public:
	Paddle(int x, int y);
	int getX() const { return x_; }
	int getY() const { return y_; }

	void setX(int x);
	void setY(int y);

	void addToX(int x);
private:
	int x_;
	int y_;
};

#endif // PADDLE_H_