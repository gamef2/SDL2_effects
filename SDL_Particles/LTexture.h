//由悠然小赐移植
//cocos2dx粒子引擎
//参考原有SDL2粒子引擎的部分代码
//增加了渲染叠加
//qq108201645
#ifndef _LTEXTURE_H_
#define _LTEXTURE_H_

//#include "PlatformMacros.h"
#include "SDLView.h"
#include "Vec2.h"
 

class LTexture
{
	SDL_Texture *mTexture;
	Size mSize;
	int mPitch;
	void* mPixels;
public:
	LTexture();
	~LTexture();
	void free();
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void SetTextureAlphaMod(Uint8 a);
	void setBlendMode(SDL_BlendMode blending);

	Size getContentSize() { return mSize; }

	void loadMemData(unsigned char * data, int len);
	void render(int x, int y, SDL_Rect * clip = nullptr, double degress = 0.0, SDL_Point * pointer = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void render(SDL_Rect * dest, SDL_Rect * clip = nullptr, double degress = 0.0, SDL_Point * pointer = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	LTexture(const LTexture &) = delete;
	LTexture &operator =(const LTexture &) = delete;
};

 

#endif