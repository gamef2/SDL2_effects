//由悠然小赐移植
//cocos2dx粒子引擎
//参考原有SDL2粒子引擎的部分代码
//增加了渲染叠加
//qq108201645
#include "SDLView.h"

SDLView::SDLView()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

SDLView::~SDLView()
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();
}

void SDLView::init(const char * title, int x, int y, int w, int h)
{
	win = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	IMG_Init(IMG_INIT_PNG);
#if defined(__ANDROID__)

SDL_DisplayMode dm;

SDL_GetCurrentDisplayMode(0, &dm);
	size = {(float)dm.w ,(float) dm.h};
	#else
	size = { (float)w,(float)h };
	#endif
//	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
}

void SDLView::cls()
{
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren);
}



void SDLView::refersh()
{
	SDL_RenderPresent(ren);
}
