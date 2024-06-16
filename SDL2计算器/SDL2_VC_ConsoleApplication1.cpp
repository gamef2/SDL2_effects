/*******************************************************************/
/*******************************************************************/
/*******************************************************************/
/***************    逆波兰计算器             **********************/
/***************      by悠然小赐             **********************/
/******************已扩展一元运算 *********************************/
/******************c4droid可以运行  *******************************/
/*******************************************************************/
/*******************************************************************/

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "calc.h"
using namespace std;
#if defined(_WIN32)
#include <SDL.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif


int WIDTH = 300, HEIGHT = 400;
const int SCREEN_TICK_FPS_FRAMES = 1000 / 60;
SDL_Window *win;
TTF_Font *gFont;
SDL_Surface *scr;

class LSurface
{
	int mWidth, mHeight;
	SDL_Surface *mSurface;
public:
	~LSurface()
	{
		free();
	}
	void free()
	{
		if (mSurface)
		{
			SDL_FreeSurface(mSurface);
			mSurface = NULL;
		}
	}
	void loadText(const char *text)
	{
		free();
		SDL_Color a = { 0,0,0 };
		mSurface = TTF_RenderUTF8_Blended(gFont, text, a);
		//SDL_SetColorKey(mSurface, SDL_TRUE, SDL_MapRGB(mSurface->format, 0, 0, 0));
		mWidth = mSurface->w, mHeight = mSurface->h;

	}
	void render(int x, int y, int w = 0, int h = 0, int r = 192, int g = 192, int b = 192)
	{
		SDL_Rect stretch = { x, y, mWidth, mHeight };
		if (w != 0 && h != 0)
		{
			stretch.w = w;
			stretch.h = h;
		}
		SDL_FillRect(scr, &stretch, SDL_MapRGB(scr->format, r, g, b));
		SDL_BlitScaled(mSurface, NULL, scr, &stretch);
	}
};

enum SCREEN_BUTTON
{
	BUTTON0,
	BUTTON1,
	BUTTON2,
	BUTTON3,
	BUTTON4,
	BUTTON5,
	BUTTON6,
	BUTTON7,
	BUTTON8,
	BUTTON9,
	BUTTON_POINT,
	BUTTON_PLUS,
	BUTTON_MINUS,
	BUTTON_MULTIPLY,
	BUTTON_DIVIDE,				// 14
	BUTTON_LEFT_SHIFT,
	BUTTON_RIGHT_SHIFT,
	BUTTON_MOD,					// 17
	BUTTON_AND,
	BUTTON_OR,
	BUTTON_XOR,
	BUTTON_NOT,
	BUTTON_NEGATE,				// 22
	BUTTON_SIN,
	BUTTON_COS,
	BUTTON_ATAN,
	BUTTON_SQRT,
	BUTTON_LPARENTHESES,
	BUTTON_RPARENTHESES,
	BUTTON_BACKSPACE,
	BUTTON_CLEAR,
	BUTTON_ENTER,

	BUTTON_TOTAL
};
const char *B[] =
{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",".", "+", "-", u8"×", u8"÷", "<<", ">>",
"%", "&", "|", "^", "!", "~", "sin", "cos", "atan","sqrt","(",")","Back", "AC", "="
};
const char op[] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9','.', '+', '-', '*', '/', 'l', 'r',
'%', '&', '|', '^', '!', '~', 's', 'c', 'a','S','(',')','B', 'C', '='
};
class Button
{
	char *elemType;
	SDL_Rect mbox;
	int keyValue, keyPress;
public:
	Button(const char *Na, int v, int x, int y, int w, int h) :keyValue(v), mbox({ x, y, w, h }),
		keyPress(0)
	{
		int l = strlen(Na) + 1;
		elemType = new char[l];
		memset(elemType, 0, l);
		memcpy(elemType, Na, l);
	}
	int GetKeyVal() { return keyValue; }
	~Button() {
	}
	char *GetElem()
	{
		return elemType;
	}
	int GetValue() { return keyValue; }
	int handleEvent(SDL_Event & e)
	{
		keyPress = 0;
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_Point mouse = { e.motion.x, e.motion.y };
			if (!(mouse.x < mbox.x || mouse.x > mbox.x + mbox.w || mouse.y < mbox.y
				|| mouse.y > mbox.y + mbox.h))
			{
				keyPress = 1;
			}
			else
				keyPress = 0;
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
			keyPress = 0;
		return keyPress;
	}
	void render();
};
class LCD {
	SDL_Rect mPosition;
	SDL_Surface *dir;
public:
	LCD() :mPosition({ 1,1,WIDTH - 2,HEIGHT / 6 - 2 }) {
		dir = SDL_CreateRGBSurface(0, mPosition.w, mPosition.h, 32, 0, 0, 0, 0);
		if (dir == NULL)
			cout << SDL_GetError() << endl;
	}
	~LCD() {
		if (dir)
			SDL_FreeSurface(dir);
	}
	SDL_Rect GetPosition() { return mPosition; }
	void clearScreen() {
		SDL_FillRect(dir, NULL, SDL_MapRGB(dir->format, 128, 128, 128));
	}
	void Draw(const string &text, int Offset = 1) {

		if (text.size()) {
			SDL_Surface *load = TTF_RenderUTF8_Blended(gFont, text.c_str(), SDL_Color{ 0,0,0 });
#if defined(_WIN32)
			SDL_Rect r = { mPosition.w - load->w,mPosition.h - load->h*Offset,load->w,load->h };
#else
			SDL_Rect r = { mPosition.w - load->w,mPosition.h - load->h*Offset,load->w,load->h };
#endif
			SDL_BlitScaled(load, NULL, dir, &r);
			SDL_FreeSurface(load);
		}
		SDL_BlitScaled(dir, NULL, scr, &mPosition);
	}
};
class ButtonQuery
{
	LCD lcd;
	int MsX, MsY, Counter = 0;
	int mWidth, mHeight;
	Button *button[BUTTON_TOTAL];

	string text, result;
public:
	ButtonQuery() :Counter(0)
	{
		MsX = 0;
		MsY = HEIGHT / 6;
		mWidth = WIDTH / 7, mHeight = (HEIGHT - MsY) / 5;
	}
	~ButtonQuery() {
		for (int i = 0; i < Counter ; ++i) {
			if (button[i] != NULL)
				delete button[i];
		}
	}
	void loadMedia();
	bool handleEvent(SDL_Event & e)
	{
		if (e.type != SDL_MOUSEMOTION && e.type != SDL_FINGERMOTION)
			for (int i = 0; i != Counter; i++)
				if (button[i]->handleEvent(e))
				{
					if (button[i]->GetKeyVal() == BUTTON_CLEAR)
					{
						text.clear();
						result.clear();
						return true;
					}
					else if (button[i]->GetKeyVal() == BUTTON_BACKSPACE)
					{
						if (text.size())
							text.pop_back();
						return true;
					}
					else if (button[i]->GetKeyVal() == BUTTON_ENTER)
					{
						if (text.size())
						{
							result.clear();
							for (auto &i : text)
								result += op[i];
							//	text.clear();
								// 计算
							stringstream is;
							try {
								result = Scanner(result);
								is << setprecision(2) << fixed << stof(calc(result,.0,.0));
							}
							catch (exception &e) {
								is << e.what();
							}
							result = "=" + is.str();
							return true;
						}
					}
					else
						//		if(text.size()<12)
					{
						if (result.size())
						{
							text.clear();
							result.clear();
						}
						text += button[i]->GetKeyVal();
						return true;
					}
				}
		return false;
	}
	void render();
};

LSurface gLText;
void init()
{
	SDL_Init(SDL_INIT_VIDEO);
#if !defined(_WIN32)
	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	WIDTH = dm.w, HEIGHT = dm.h;
#endif
	win =
		SDL_CreateWindow("Calculate", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH,
			HEIGHT, SDL_WINDOW_SHOWN);
	scr = SDL_GetWindowSurface(win);

	TTF_Init();
#if defined(_WIN32)
	gFont = TTF_OpenFont("font.ttf", 20);
#else
	gFont = TTF_OpenFont("font.ttf", 100);
#endif

}

void close()
{
	TTF_CloseFont(gFont);
	SDL_FreeSurface(scr);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void Button::render()
{
	gLText.loadText(elemType);

	if (keyPress)
		gLText.render(mbox.x, mbox.y, mbox.w, mbox.h, 255, 192, 255);
	else
		gLText.render(mbox.x, mbox.y, mbox.w, mbox.h, 128, 128, 128);

}

void ButtonQuery::loadMedia()
{
	fstream in("calc.keycfg");
	if (!in.is_open())
	{
		cout << "缺少配置文件!" << endl;
		exit(0);
	}
	int x = 0, y = MsY, i = 0;

	while (!in.eof())
	{
		int type = -1;
		in >> type;
		if (type >= 0 && type < BUTTON_TOTAL)
		{
			button[i] = new Button(B[type], type, x + 1, y + 1, mWidth - 2, mHeight - 2);
			Counter++;
			i++;
		}
		x += mWidth;
		if (x + mWidth > WIDTH)
			x = 0, y += mHeight;
		cout << Counter << endl;
	}
	cout << Counter << endl;
	in.close();
}
void ButtonQuery::render() {
	string s;
	if (text.size())
	{
		for (auto &i : text)
			s += B[i];
	}
	lcd.clearScreen();
	if (result.size())
	{
		lcd.Draw(s, 2);
		lcd.Draw(result, 1);
	}
	else
	{

		lcd.Draw(s, 1);
	}
	for (int i = 0; i < Counter; ++i)
		button[i]->render();
}
int main(int, char **)
{
	init();

	ButtonQuery bQuery;
	bQuery.loadMedia();


	bool quit = false;
	SDL_Event e;

	while (!quit) {
		int capTimer = SDL_GetTicks();
		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				quit = true;
			else if (bQuery.handleEvent(e))
				break;
		SDL_FillRect(scr, NULL, SDL_MapRGB(scr->format, 192, 192, 192));
		bQuery.render();
		SDL_UpdateWindowSurface(win);

		float timeFrames = SDL_GetTicks() - capTimer;
		if (timeFrames < SCREEN_TICK_FPS_FRAMES)
			SDL_Delay(SCREEN_TICK_FPS_FRAMES - timeFrames);
	}
	close();
	return 0;
}