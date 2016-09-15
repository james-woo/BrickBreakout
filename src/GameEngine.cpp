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
	score_surface_ = TTF_RenderText_Solid(font_, "000", white);
	score_message_ = SDL_CreateTextureFromSurface(renderer_, score_surface_);

	score_message_rect_.x = 0;
	score_message_rect_.y = 0;
	score_message_rect_.w = 200;
	score_message_rect_.h = 40;

	lives_surface_ = TTF_RenderText_Solid(font_, "3", white);
	lives_message_ = SDL_CreateTextureFromSurface(renderer_, score_surface_);

	lives_message_rect_.x = SCREEN_WIDTH - 42;
	lives_message_rect_.y = 0;
	lives_message_rect_.w = 42;
	lives_message_rect_.h = 40;

	SDL_RenderCopy(renderer_, score_message_, NULL, &score_message_rect_);
	SDL_RenderPresent(renderer_);
}

GameEngine::~GameEngine() {
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);

    SDL_DestroyTexture(score_message_);
    SDL_DestroyTexture(lives_message_);

    SDL_FreeSurface(score_surface_);
    SDL_FreeSurface(lives_surface_);

    TTF_CloseFont(font_);

    delete ball_;
    delete paddle_;
    delete board_;

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
			SDL_GetMouseState(&mouse_x_, &mouse_y_);
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
                    if(board_->game_is_over()) {
                        board_->new_game();
                    }
				}
				else {
					ball_->reset();
				}
                default:
                    break;
			}
		}
	}
}

void GameEngine::update() {
    paddle_->set_x(mouse_x_);

	if (ball_->get_status() == ball_->READY) {
		if(mouse_x_ < SCREEN_WIDTH - PADDLE_WIDTH) {
			ball_->set_x(mouse_x_ + (PADDLE_WIDTH / 2) - (BALL_DIAMETER / 2));
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
        if(board_->update_lives(-1)) {
            board_->game_over();
            ball_->reset();
        }
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
    SDL_Rect paddle = {paddle_->get_x(),
                       paddle_->get_y(),
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
				default:
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
    	score_string = "SCORE   00" + std::to_string(board_->get_score());
    } else {
    	score_string = "SCORE   " + std::to_string(board_->get_score());
    }
    score_surface_ = TTF_RenderText_Solid(font_, score_string.c_str(), white);
	score_message_ = SDL_CreateTextureFromSurface(renderer_, score_surface_);

	SDL_RenderCopy(renderer_, score_message_, NULL, &score_message_rect_);

	// Lives
	lives_surface_ = TTF_RenderText_Solid(font_, std::to_string(board_->get_lives()).c_str(), white);
	lives_message_ = SDL_CreateTextureFromSurface(renderer_, lives_surface_);

	SDL_RenderCopy(renderer_, lives_message_, NULL, &lives_message_rect_);

    // Gameover
    if(board_->game_is_over()) {
        SDL_Surface* game_over_surface = TTF_RenderText_Solid(font_, "GAME OVER", white);
        SDL_Texture* game_over_message = SDL_CreateTextureFromSurface(renderer_, game_over_surface);
        SDL_Rect game_over_rect;
        game_over_rect.x = SCREEN_WIDTH / 2 - 342/2;
        game_over_rect.y = SCREEN_HEIGHT / 2 - 142/2;
        game_over_rect.w = 342;
        game_over_rect.h = 142;
        SDL_RenderCopy(renderer_, game_over_message, NULL, &game_over_rect);
    }

    // Swap buffers.
    SDL_RenderPresent(renderer_);
}