#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#endif

#ifndef BOMB_H_INCLUDED
#define BOMB_H_INCLUDED

class Bomb
{
private:
	int spriteIndex, spriteCheck;
public:
	bool onScreenExploding;
	Bomb();
	~Bomb(){}
	bool igniteExplosion();
	void updateExplosion();
	int getSpriteIndex(){ return spriteIndex; }
};

#endif