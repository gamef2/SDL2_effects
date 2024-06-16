

//由悠然小赐移植
//cocos2dx粒子引擎
//参考原有SDL2粒子引擎的部分代码
//增加了渲染叠加
//qq108201645
#ifndef _SDL2_VIEW_H_
#define _SDL2_VIEW_H_




#if defined(_WIN32)||defined(_WIN64)
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif



#include "Vec2.h"

class SDLView {
	SDL_Window *win;
	SDL_Renderer *ren;
	Size size;
public:
	static SDLView *getInstance() {
		static SDLView _instance;
		return &_instance;
	}
	SDLView();
	~SDLView();

	void init(const char *title, int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED,
		int w = 640, int h = 480);
	void cls();

	void refersh();

	SDL_Window *getWindow() { return win; }
	SDL_Renderer *getRenderer() { return ren; }
	Size getSize() { return size; }
};



#endif