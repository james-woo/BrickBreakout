#include "GameEngine.h"
#include "Paddle.h"
#include "Ball.h"
#include "Board.h"

#include <cmath>
#include <random>
#include <iostream>

GameEngine::GameEngine() {
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_ShowCursor(0);

	// Window
	window_ = SDL_CreateWindow("Brick Breakout",
		                       SDL_WINDOWPOS_UNDEFINED,
		                       SDL_WINDOWPOS_UNDEFINED,
		                       SCREEN_WIDTH,
		                       SCREEN_HEIGHT,
		                       SDL_WINDOW_SHOWN);

	renderer_ = SDL_CreateRenderer(window_,
		                           -1,
		                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	ball_ = new Ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30);
	paddle_ = new Paddle(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);
	board_ = new Board();
	exit_ = false;

	TTF_Init();

	font_ = TTF_OpenFont("resources/fonts/bitwise.ttf", 24);
	if (font_ == nullptr) {
		std::cout << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
	SDL_Color white = {255, 255, 255};
	scoreSurface_ = TTF_RenderText_Solid(font_, "000", white);
	scoreMessage_ = SDL_CreateTextureFromSurface(renderer_, scoreSurface_);

	scoreMessageRect_.x = 0;
	scoreMessageRect_.y = 0;
	scoreMessageRect_.w = 100;
	scoreMessageRect_.h = 40;

	livesSurface_ = TTF_RenderText_Solid(font_, "3", white);
	livesMessage_ = SDL_CreateTextureFromSurface(renderer_, scoreSurface_);

	livesMessageRect_.x = SCREEN_WIDTH - 42;
	livesMessageRect_.y = 0;
	livesMessageRect_.w = 42;
	livesMessageRect_.h = 40;

	SDL_RenderCopy(renderer_, scoreMessage_, NULL, &scoreMessageRect_);
	SDL_RenderPresent(renderer_);
}

GameEngine::~GameEngine() {
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void GameEngine::execute() {
	newGame();
	while (!exit_) {
		input();
		update();
		render();
		SDL_Delay(10);
	}
}

void GameEngine::newGame() {
	board_->create_level();
}

void GameEngine::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_MOUSEMOTION) {
			SDL_GetMouseState(&mouseX_, &mouseY_);
		}
		if (event.type == SDL_QUIT) {
			exit_ = true;
		}
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					exit_ = true;
					break;
				case SDLK_SPACE:
				if (ball_->get_status() == ball_->READY) {
					ball_->set_status(ball_->LAUNCH);
				}
				else {
					ball_->reset();
				}
				break;
			}
		}
	}
}

void GameEngine::update() {
	paddle_->setX(mouseX_);

	if (ball_->get_status() == ball_->READY) {
		if(mouseX_ < SCREEN_WIDTH - PADDLE_WIDTH) {
			ball_->set_x(mouseX_ + (PADDLE_WIDTH / 2) - (BALL_DIAMETER / 2));
		} else {
			ball_->set_x(SCREEN_WIDTH - (PADDLE_WIDTH / 2));
		}
		return;
	} else if (ball_->get_status() == ball_->LAUNCH) {
        ball_->launch_ball();
	}

	// Update ball speed
	ball_->update_speed();

	// Paddle collisions
	if (ball_->collides_with_paddle(paddle_)) {
		ball_->bounce_off_paddle(paddle_);
	}
	// Brick collisions
	for (int i = 0; i < BOARD_BRICK_WIDTH; ++i) {
    	for (int j = 0; j < BOARD_BRICK_HEIGHT; ++j) {
    		if (ball_->collides_with_brick(board_->get_brick(i, j))) {
				ball_->bounce_off_brick(board_->get_brick(i, j));
				board_->update_score(100);
			}
    	}
    }

	// Wall and bounds collisions
	if (ball_->collides_with_top()) {
		ball_->set_dy((int) (ball_->get_speed() * std::sin(-1 * ball_->get_angle() * M_PI/180.0f)));
	} else if (ball_->collides_with_sides()) {
		ball_->reverse_dx();
	}

	if (ball_->out_of_bounds()) {
		ball_->reset();
		exit_ = board_->update_lives(-1);
	}

	ball_->add_x(ball_->get_dx());
	ball_->add_y(-1 * ball_->get_dy());
}

void GameEngine::render() {
	// Clear screen (background color).
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);  // Dark grey
    SDL_RenderClear(renderer_);


    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255); // White
    // Ball
    SDL_Rect ball = { ball_->get_x(),
    	              ball_->get_y(),
    	              BALL_DIAMETER,
    	              BALL_DIAMETER };
    SDL_RenderFillRect(renderer_, &ball);

    // Paddle
    SDL_Rect paddle = { paddle_->getX(),
                        paddle_->getY(),
                        PADDLE_WIDTH, 
                        PADDLE_HEIGHT };
    SDL_RenderFillRect(renderer_, &paddle);

    // Board
    SDL_Rect board = { board_->get_x(),
                       board_->get_y() + SCORE_SECTION_HEIGHT,
                       SCREEN_WIDTH - 1,
                       SCREEN_HEIGHT - SCORE_SECTION_HEIGHT };
    SDL_RenderDrawRect(renderer_, &board);

    // Bricks
    for (int i = 0; i < BOARD_BRICK_WIDTH; ++i) {
    	for (int j = 0; j < BOARD_BRICK_HEIGHT; ++j) {
    		Brick brick = *board_->get_brick(i, j);
    		if(!brick.state) continue;

    		switch(brick.hardness) {
    			case 1: 
    				SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255); // Red
    				break;
				case 2:
					SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255); // Green
    				break;
				case 3:
					SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255); // Blue
    				break;
				case 4:
					SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255); // White
    				break;
				case 0: 
    				SDL_SetRenderDrawColor(renderer_, 128, 128, 128, 255); // Grey
    				break;
    		}

    		SDL_Rect brect;
    		brect.x = brick.x;
    		brect.y = brick.y;
    		brect.w = BRICK_WIDTH;
    		brect.h = BRICK_HEIGHT;

    		SDL_RenderFillRect(renderer_, &brect);
    	}
    }

    // Score
    SDL_Color white = {255, 255, 255};
    std::string score_string;
    if(board_->get_score() < 100) {
    	score_string = "00" + std::to_string(board_->get_score());
    } else {
    	score_string = std::to_string(board_->get_score());
    }
    scoreSurface_ = TTF_RenderText_Solid(font_, score_string.c_str(), white);
	scoreMessage_ = SDL_CreateTextureFromSurface(renderer_, scoreSurface_);

	SDL_RenderCopy(renderer_, scoreMessage_, NULL, &scoreMessageRect_);

	// Lives
	livesSurface_ = TTF_RenderText_Solid(font_, std::to_string(board_->get_lives()).c_str(), white);
	livesMessage_ = SDL_CreateTextureFromSurface(renderer_, livesSurface_);

	SDL_RenderCopy(renderer_, livesMessage_, NULL, &livesMessageRect_);

    // Swap buffers.
    SDL_RenderPresent(renderer_);
}