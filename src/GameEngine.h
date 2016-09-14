#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

	int getMouseX() { return mouseX_; }

private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;

	// Objects
	Ball* ball_;
	Paddle* paddle_;
	Board* board_;

	int mouseX_, mouseY_;
	bool exit_;

	TTF_Font* font_;

	SDL_Surface* scoreSurface_;
	SDL_Texture* scoreMessage_;
	SDL_Rect scoreMessageRect_;

	SDL_Surface* livesSurface_;
	SDL_Texture* livesMessage_;
	SDL_Rect livesMessageRect_;
};

#endif // GAME_ENGINE_H_