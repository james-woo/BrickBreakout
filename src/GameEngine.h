#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>

class Ball;
class Paddle;
class Board;
class Brick;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCORE_SECTION_WIDTH = 640;
const int SCORE_SECTION_HEIGHT = 40;

class GameEngine {
public:
	GameEngine();
	~GameEngine();

	void execute();
	void input();
	void update();
	void render();
	void newGame();

private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;

	// Objects
	Ball* ball_;
	Paddle* paddle_;
	Board* board_;

	int mouse_x_, mouse_y_;
	bool exit_;

	TTF_Font* font_;

	SDL_Surface* score_surface_;
	SDL_Texture* score_message_;
	SDL_Rect score_message_rect_;

	SDL_Surface* lives_surface_;
	SDL_Texture* lives_message_;
	SDL_Rect lives_message_rect_;
};

#endif // GAME_ENGINE_H_