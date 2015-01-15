#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#endif

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Bullet.h"

#endif

#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <math.h>

#endif

#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "MyRect.h"

#endif

#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

class EnemyClass
{
private:
	int BoxPX, BoxPY, SpritePY, SpritePX, shotsCooldown, theAngle, elapsedTime, cosLine, targetY, loopdeloopSprite, spriteCheck, velocity_X, velocity_Y;
	my_Rectangle HBOX; //nästa gång klistrar jag på spriten på hitboxen istället för tvärt om, för mycket pill nu.
	int theType; //0 = Pentarou, 1 = Kid Dracula //enum EnemyType gjorde så att det alltid blev Pentarou typen
	int theState; //0 = Neutral Flight, 1 = Flying down, 2 = Flying up, 3 = Death Animation
	//Boundingbox Point, Boundingbox With and Height, Sprite Point... BoxP.X = SpriteP.X + 10; BoxP.Y = SpriteP.Y + 4;
public: 
	static const int BoxH = 10, BoxW = 20, nrOfBullets = 3;
	bool onScreen, isActive;
	Bullet themBullets[nrOfBullets]; //3 bullets on the screen max
	EnemyClass();
	~EnemyClass();
	void SpawnEnemy(int typoType = 0, int nX = 950, int nY = 100, int tY = -111, bool oS = false, bool iA = true);
	void flushBullets();
	bool shootEm();
	void shotsCooldownTick();
	bool shotsCooldownZero();
	my_Rectangle getRect(){ return HBOX; }
	void Move();
	void UnSpawn();
	bool update();
	void updateSpriteIndex();
	void Dying();
	bool isAlive();
	int getX(){ return SpritePX; }
	int getY(){ return SpritePY; }
	int getVeloX(){ return velocity_X; }
	int getVeloY(){ return velocity_Y; }
	int getType(){ return theType; }
	int getState(){ return theState; }
	int getSpriteIndex(){ return spriteCheck; }
	int getElapsedTime(){ return elapsedTime; }
	int getTheAngle(){ return theAngle; }
	int getTargetY(){ return targetY; }
};

#endif