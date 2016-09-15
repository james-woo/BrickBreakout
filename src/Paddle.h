#ifndef PADDLE_H_
#define PADDLE_H_

const int PADDLE_HEIGHT = 10;
const int PADDLE_WIDTH = 72;

class Paddle {
public:
	Paddle(int x, int y);
	int get_x() const { return x_; }
	int get_y() const { return y_; }

	void set_x(int x);
private:
	int x_;
	int y_;
};

#endif // PADDLE_H_