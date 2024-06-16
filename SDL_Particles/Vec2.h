#ifndef _VEC2_H_
#define _VEC2_H_

#include <cmath>
#include <iostream>
using namespace std;


struct  Vec2
{
	float x, y;
public:
	Vec2() :x(0), y(0) {}
	Vec2(float x,float y):x(x),y(y){}

// 	Vec2(int w = 0, int h = 0) :x((float)x), y((float)y) {
// 	}

	Vec2(const Vec2 &rhs):x(rhs.x),y(rhs.y){}
	Vec2 & operator=(const Vec2& rhs) {
		if (this != &rhs)
		{
			x = rhs.x;
			y = rhs.y;
		}
		return *this;
	}

	~Vec2() {}

	Vec2 operator+(const Vec2 &rhs)
	{
		float cx = x + rhs.x;
		float cy = y + rhs.y;
		return { cx,cy };
	}
	Vec2& operator+=(const Vec2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vec2 operator-(const Vec2 &rhs)
	{
		float cx = x - rhs.x;
		float cy = y - rhs.y;
		return { cx,cy };
	}

	Vec2 operator*(const Vec2 &rhs)
	{
		float cx = x * rhs.x;
		float cy = y * rhs.y;
		return { cx,cy };
	}

	Vec2 operator/(const Vec2 &rhs)
	{
		float cx = x, cy = y;
		if (rhs.x != 0)
			cx /= rhs.x;
		if (rhs.y != 0)
			cy /= rhs.y;
		return { cx,cy };
	}

	Vec2 operator+(float value)
	{
		float cx = x + value;
		float cy = y + value;
		return { cx,cy };
	}

	Vec2 operator-(float value)
	{
		float cx = x - value;
		float cy = y - value;
		return { cx,cy };
	}
	Vec2 operator*(float value)
	{
		float cx = x * value;
		float cy = y * value;
		return { cx,cy };
	}
	Vec2 operator*(float value)const 
	{
		return { x * value,y * value };
	}
	Vec2 operator/(float value)
	{
		float cx = x, cy = y;;
		if (value != 0)
		{
			cx /= value;
			cy /= value;
		}
		return { cx,cy };
	}

	friend std::ostream & operator<<(std::ostream& os, const Vec2 &rhs) {
		return os << " (" << rhs.x << " " << rhs.y << ") ";
	}

	bool operator != (const Vec2 &rhs)const {
		return x != rhs.x && y != rhs.y;
	}
	bool operator == (const Vec2 &rhs)const {
		return x == rhs.x && y == rhs.y;
	}

	inline float getAngle() const {
		return atan2f(y, x);
	}

	static const Vec2 ZERO;
};

struct Size {
	float width, height;
public:
	Size(float w =0.0, float h =0.0) :width(w), height(h) {
	}
// 	Size(int w = 0, int h = 0) :width((float)w), height((float)h) {
// 	}
	Size(const Size &rhs):width(rhs.width),height(rhs.height){}
	Size & operator=(const Size& rhs) {
		if (this != &rhs)
		{
			width = rhs.width;
			height = rhs.height;
		}
		return *this;
	}
	

	operator Vec2()const 
	{
		return Vec2(width, height);
	}
	Size operator+(const Size &rhs)
	{
		float cw = width + rhs.width;
		float ch = height + rhs.height;
		return { cw,ch };
	}
	Size operator-(const Size &rhs)
	{
		float cw = width - rhs.width;
		float ch = height - rhs.height;
		return { cw,ch };
	}
	Size operator*(const Size &rhs)
	{
		float cw = width * rhs.width;
		float ch = height * rhs.height;
		return { cw,ch };
	}


	Size operator/(const Size &rhs)
	{
		float cw = width, ch = height;
		if (rhs.width != 0)
			cw /= rhs.width;
		if (rhs.height != 0)
			ch /= rhs.height;
		return { cw,ch };
	}

	Size operator*(const float &value)
	{
		float cw = width * value;
		float ch = height * value;
		return { cw,ch };
	}

	Size operator*(const float &value)const
	{
		return { width * value,height * value };
	}

	Size operator/(float value)
	{
		float cw = width, ch = height;
		if (value != 0)
		{
			cw /= value;
			ch /= value;
		}
		return { cw,ch };
	}

	friend std::ostream & operator<<(std::ostream& os, const Size &rhs) {
		return os << " (" << rhs.width << " " << rhs.height << ") ";
	}

	bool operator != (const Size &rhs)const {
		return width != rhs.width && height != rhs.height;
	}
	bool operator == (const Size &rhs)const {
		return width == rhs.width && height == rhs.height;
	}
};


struct Rect {
	Vec2 origin;;
	Size size;
public:
	Rect (float x = 0.0, float y = 0.0, float w = 0.0, float h = 0.0):origin(x,y), size(w,h){}
	Rect(const Vec2 &position,const Size &contentSize) :origin(position), size(contentSize) {}

	Rect(const Rect& rhs):origin(rhs.origin), size(rhs.size){}
	Rect & operator=(const Rect &rhs)
	{
		if (this != &rhs)
		{
			origin = rhs.origin;
			size = rhs.size;
		}
		return *this;
	}

	Size getContentSize()const { return size; }
	Rect & operator=(const Vec2 &rhs)
	{
		origin = rhs;
		return *this;
	}
	Rect & operator=(const Size &rhs)
	{
		size = rhs;
		return *this;
	}

	bool operator != (const Rect &rhs)const {
		return origin != rhs.origin && size != rhs.size;
	}
	bool operator == (const Rect &rhs)const {
		return origin == rhs.origin && size == rhs.size;
	}

	Rect operator*(const float &value)
	{
		auto p = origin * value;
		auto cwin = size * value;
		return { origin,cwin };
	}

	Rect operator*(const float &value)const
	{
		return { origin * value, size * value };
	}
	Rect operator/(float value)
	{
		auto cp = origin;
		auto cs = size;
		if (value != 0)
		{
			cp = cp / value;
			cs = cs / value;
		}
		return { cp,cs };
	}

// 	Rect& operator*=(const float &value)
// 	{
// 		origin  = origin * value;
// 		size = size * value;
// 		return *this;
// 	}

	static const Rect RECT_ZERO;
// 
// 	void  getPosition() { return _position };
// 	Size getContentSize() { _contentSize };
};


#if defined(_MSC_VER)

__declspec(selectany) const Vec2 Vec2::ZERO(0.0f, 0.0f);
__declspec(selectany) const Rect Rect::RECT_ZERO(0.0f, 0.0f, 0.0f, 0.0f);

#elif defined(__GNUC__)||defined(__GNUG__)

__attribute((weak)) const Vec2 Vec2::ZERO(0.0f, 0.0f);
__attribute((weak)) const Rect Rect::RECT_ZERO(0.0f, 0.0f, 0.0f, 0.0f);

#endif

#include "SDLView.h"




#endif
