#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#endif

#include "Bullet.h"
#include "Bomb.h"

#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "MyRect.h"

#endif
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED


enum PlayerState
{
	TurningUp,
	TurningDown,
	Neutral,
	Dying,
	Dead
};

class PlayerClass
{
private:
	int SpritePY, SpritePX, nrOfBombs, shotsCooldown, loopdeloopSprite, spriteCheck, velocity_X, velocity_Y;
	my_Rectangle HBOX;
public: 
	static const int BoxH = 12, BoxW = 23, nrOfBullets = 10;
	bool isDead;
	PlayerState theState;
	//int theState; //0 = TurningUp, 1 = TurningDown, 2 = Neutral, 3 = Dying, 4 = Dead
	Bullet themBullets[nrOfBullets]; //10 bullets on the screen max
	Bomb theBomb;
	PlayerClass(int nX = 9, int nY = 75);
	~PlayerClass();
	my_Rectangle getRect(){ return HBOX; }
	void flushBullets();
	bool shootEm();
	void shotsCooldownTick();
	bool shotsCooldownZero();
	bool dropTheBomb();
	void update();
	void updateSpriteIndex();
	void Move(int direction = 5);
	void setNeutral();
	void Respawn(int nX = 0, int nY = 75, int Bombs = 0);
	void ImDying();
	int getX(){ return SpritePX; }
	int getY(){ return SpritePY; }
	int getVeloX(){ return velocity_X; }
	int getVeloY(){ return velocity_Y; }
	int getSpriteIndex(){ return spriteCheck; }
	int getNrOfBombs(){ return nrOfBombs; }
	bool isAlive();
};

#endif