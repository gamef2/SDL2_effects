//����ȻС����ֲ
//cocos2dx��������
//�ο�ԭ��SDL2��������Ĳ��ִ���
//��������Ⱦ����
//qq108201645
#if defined(__ANDROID__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "SDLView.h"

#include "OGParticleExamples.h"

#include<vector>

ParticleExample::PatticleStyle  Ptype[]={
	ParticleExample::FIRE,
		ParticleExample::FIRE_WORK,
	ParticleExample::	SUN,
	ParticleExample::	GALAXY,
	ParticleExample::	FLOWER,
	ParticleExample::	METEOR,
	ParticleExample::	SPIRAL,
	ParticleExample::	EXPLOSION,
	ParticleExample::	SMOKE,
	ParticleExample::	SNOW,
	ParticleExample::	RAIN
};

int main(int argc, char*argv[])
{
	//��ʼ��sdl
	auto sdlview = SDLView::getInstance();
	sdlview->init(u8"����",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,800,800);


	//��ʼ������ʵ��
	std::vector<ParticleExample*> p;
	for (auto& i : Ptype)
		p.push_back(new ParticleExample(i));

	//��ʼ����Ļ�ֿ�
	int x = 0, y = 0;
	auto size = sdlview->getSize();
	int i = (int)size.width / 5;
	int j = (int)size.height / 5;
	int cursor = 0;
	for (int x = 1; x < 4; x++)
	{
		for (int y = 1; y < 4; y++)
			p[cursor++]->setPosition({ x*(float)i,y*(float)j });
	}
	p[cursor++]->setPosition({ (float)i,(float)j });
	p[cursor++]->setPosition({ (float)i,j*(float)2 });
	//����ѭ��

	SDL_Event e;
	while (1)
	{
		if (SDL_PollEvent(&e))
		{
			if (e.type==SDL_QUIT)
				break;
		}
		sdlview->cls();

		for (auto &i : p) {
			i->update(0.02f);
			i->draw();
		}

		sdlview->refersh();
	}


	return 0;
}