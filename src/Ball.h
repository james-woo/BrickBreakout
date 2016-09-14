#ifndef BALL_H_
#define BALL_H_

class Paddle;
class Brick;

const int BALL_DIAMETER = 10;

class Ball {
public:
	Ball(int x, int y);
	~Ball() {}

    enum Status { READY, LAUNCH, LAUNCHED };

    void set_x(int x) { x_ = x; }
    void set_y(int y) { y_ = y; }
    void set_dx(int dx) { dx_ = dx; }
    void set_dy(int dy) { dy_ = dy; }
    void set_status(Status status) { status_ = status; }

    int get_x() { return x_; }
    int get_y() { return y_; }
    int get_dx() { return dx_; }
    int get_dy() { return dy_; }
    int get_speed() { return speed_; }
    int get_angle() { return angle_; }
    Status get_status() { return status_; }

    void add_x(int x) { x_ += x; }
    void add_y(int y) { y_ += y; }

    void reverse_dx() { dx_ *= -1; }

	void launch_ball();
	void update_speed();
	bool collides_with_top() const;
	bool collides_with_sides() const;
	bool collides_with_paddle(Paddle *paddle) const;
	bool collides_with_brick(Brick *brick) const;
	void bounce_off_paddle(Paddle *paddle);
	void bounce_off_brick(Brick *brick);
	bool out_of_bounds();
	void reset();

private:
	// Ball status
	Status status_;

	// Ball position
	int x_;
	int y_;

	// Ball movement
	int dx_;
	int dy_;
	bool bounce_;
	int speed_;
	float angle_;
	int hits_;
};

#endif // BALL_H_