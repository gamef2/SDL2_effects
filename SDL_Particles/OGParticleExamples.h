//由悠然小赐移植
//cocos2dx粒子引擎
//参考原有SDL2粒子引擎的部分代码
//增加了渲染叠加
//qq108201645
#ifndef _OGPARTICLE_EXAMPLE_H_
#define _OGPARTICLE_EXAMPLE_H_

#include "OGParticleSystem.h"
 

class ParticleExample : public ParticleSystem {
public:
	enum PatticleStyle
	{
		NONE,
		FIRE,
		FIRE_WORK,
		SUN,
		GALAXY,
		FLOWER,
		METEOR,
		SPIRAL,
		EXPLOSION,
		SMOKE,
		SNOW,
		RAIN,
	};
	ParticleExample(PatticleStyle style);
	virtual ~ParticleExample() {}

	bool setStyle(PatticleStyle style);
protected:
	PatticleStyle _style = NONE;
private:

	ParticleExample(const ParticleExample &) = delete;
	ParticleExample &operator =(const ParticleExample &) = delete;

};
 

#endif