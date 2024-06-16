//由悠然小赐移植
//cocos2dx粒子引擎
//参考原有SDL2粒子引擎的部分代码
//增加了渲染叠加
//qq108201645
#include "LTexture.h"


LTexture::LTexture() :mTexture(nullptr), mSize(0), mPitch(0), mPixels(nullptr)
{
}


LTexture::~LTexture()
{
	free();
}

void LTexture::free()
{
	if (mTexture != nullptr)
		SDL_DestroyTexture(mTexture);
}
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//????????rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}
void LTexture::SetTextureAlphaMod(Uint8 a) {
	SDL_SetTextureAlphaMod(mTexture, a);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}


void LTexture::loadMemData(unsigned char * data, int len)
{
	free();

	SDL_RWops *memp = SDL_RWFromMem((void*)data, len);
	SDL_Surface *load = IMG_Load_RW(memp, 0);

	SDL_Surface *surfaceFormat = SDL_ConvertSurfaceFormat(load, SDL_PIXELFORMAT_RGBA8888, 0);

	mTexture = SDL_CreateTexture(SDLView::getInstance()->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
		surfaceFormat->w, surfaceFormat->h);

	SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

	SDL_LockTexture(mTexture, &surfaceFormat->clip_rect, &mPixels, &mPitch);

	memcpy(mPixels, surfaceFormat->pixels, mPitch * surfaceFormat->h);

	mSize = { (float)surfaceFormat->w,(float)surfaceFormat->h };

#ifdef TRANSPRENT_BLACK
	Uint32 *pixels = (Uint32*)mPixels;
	Uint32 colorKey = SDL_MapRGB(surfaceFormat->format, 0, 0, 0);
	Uint32 transParent = SDL_MapRGBA(surfaceFormat->format, 0, 0, 0, 0);

	Uint32 pixelCount = mPitch / 4 * surfaceFormat->h;

	for (int i = 0; i < pixelCount; ++i)
	{
		if (pixels[i] == colorKey)
			pixels[i] = transParent;
	}

#endif

	SDL_UnlockTexture(mTexture);

	SDL_FreeSurface(surfaceFormat);
	SDL_FreeSurface(load);
}

void LTexture::render(int x, int y, SDL_Rect *clip, double degress, SDL_Point *pointer, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x,y,(int)mSize.width,(int)mSize.height };
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(SDLView::getInstance()->getRenderer(), mTexture, clip, &renderQuad, degress, pointer, flip);
}

void LTexture::render(SDL_Rect *dest, SDL_Rect *clip, double degress, SDL_Point *pointer, SDL_RendererFlip flip)
{

	SDL_RenderCopyEx(SDLView::getInstance()->getRenderer(), mTexture, clip, dest, degress, pointer, flip);
}
 