#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#endif

#include "MyRect.h"

//#ifndef PLAYER_H_INCLUDED
//#define PLAYER_H_INCLUDED
//
//#include "Player.h"
//
//#endif


#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

enum WhoHurt
{
	FriendlyBullet,
	EnemyBullet
}; 

class Bullet
{
private:
	static const int BoxH = 10, BoxW = 15;
	int BoxPX, BoxPY, SpritePY, SpritePX, spriteCheck, velocity_X, velocity_Y;
	my_Rectangle HBOX;
public:
	bool onScreen;
	WhoHurt theBullet;
	Bullet();
	~Bullet();
	my_Rectangle getRect(){ return HBOX; }
	bool shotsFired(int X = 0, int Y = 0);
	void updateMove();
	void DestroyBullet();
	void updateSpriteIndex();
	int getX(){ return SpritePX; }
	int getY(){ return SpritePY; }
	int getVeloX(){ return velocity_X; }
	int getVeloY(){ return velocity_Y; }
	int getSpriteIndex();
};

#endif