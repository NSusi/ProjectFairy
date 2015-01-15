#include "Bullet.h"
static const int PlayBoxResX = 800, PlayBoxResY = 600, SpritePaddingW = 4, SpritePaddingH = 2;

Bullet::Bullet()
{
	onScreen = false;
	spriteCheck = 0;
	SpritePY = SpritePX = -150;
	velocity_X = velocity_Y = 0;
}

Bullet::~Bullet()
{
}

bool Bullet::shotsFired(int X, int Y)
{
	if((X - 150 > 0 && X < PlayBoxResX) || (Y - 10 > 0 && Y + 15 < PlayBoxResY))
	{
		if(theBullet == FriendlyBullet)
		{
			HBOX.left = X + 30;
			HBOX.bottom = Y;
			HBOX.right = HBOX.left + BoxW;
			HBOX.top = HBOX.bottom + BoxH;
			velocity_X = 6;
		}
		else if(theBullet == EnemyBullet)
		{
			HBOX.left = X - 150;
			HBOX.bottom = Y;
			HBOX.right = HBOX.left + BoxW;
			HBOX.top = HBOX.bottom + BoxH;
			velocity_X = -6;
		}

		SpritePX = HBOX.left - SpritePaddingW;
		SpritePY = HBOX.bottom - SpritePaddingH;

		onScreen = true;
		return true;
	}
	else
	{
		onScreen = false;
		return false;
	}
}

void Bullet::updateMove()
{
	if(onScreen)
	{
		HBOX.left += velocity_X;
		HBOX.right = HBOX.left + BoxW;
		SpritePX = HBOX.left - SpritePaddingW;
	}
	if(SpritePX <= 0 - 30 || SpritePX >= 800 + 100 || SpritePY <= 0 - 25 || SpritePY >= 600 + 25)
	{
		DestroyBullet();
	}
}

void Bullet::DestroyBullet()
{
	onScreen = false;
	if(theBullet == FriendlyBullet)
	{
		SpritePX = -300;
		SpritePY = -300;
		HBOX.left = -300;
		HBOX.bottom = -300;
		HBOX.right = -300;
		HBOX.top = -300;
		velocity_X = 0;
	}
	else if(theBullet == EnemyBullet)
	{
		SpritePX = -150;
		SpritePY = -150;
		HBOX.left = -150;
		HBOX.bottom = -150;
		HBOX.right = -150;
		HBOX.top = -150;
		velocity_X = 0;
	}
	spriteCheck = 0;
}

void Bullet::updateSpriteIndex()
{
	spriteCheck = (spriteCheck + 1) % 4;
}

int Bullet::getSpriteIndex()
{
	if(spriteCheck >= 0 && spriteCheck <= 1)
		return 0;
	if(spriteCheck >= 2 && spriteCheck <= 3)
		return 1;
}