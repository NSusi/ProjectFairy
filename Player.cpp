#include "Player.h"
static const int PlayBoxResX = 800, PlayBoxResY = 600, MemSpriteW = 60, MemSpriteH = 30, SpritePaddingW = 8, SpritePaddingH = 5, HBOXsizeW = 40, HBOXsizeH = 18;

PlayerClass::PlayerClass(int nX, int nY)
{
	shotsCooldown = 0;
	velocity_X = velocity_Y = 0;
	HBOX.left = nX;
	HBOX.bottom = nY;
	HBOX.right = HBOX.left + HBOXsizeW;
	HBOX.top = HBOX.bottom + HBOXsizeH;
	SpritePX = HBOX.left - SpritePaddingW;
	SpritePY = HBOX.bottom - SpritePaddingH;
	theState = Neutral;
	loopdeloopSprite = 0;
	spriteCheck = 0;
	nrOfBombs = 3;
	isDead = false;
	for(int i = 0; i < nrOfBullets; i++)
	{
		themBullets[i].theBullet = FriendlyBullet;
	}
}

PlayerClass::~PlayerClass()
{
}

void PlayerClass::flushBullets()
{
	for(int i = 0; i < nrOfBullets; i++)
	{
		this->themBullets[i].DestroyBullet();
	}
}

bool PlayerClass::shootEm()
{
	bool result = false;
	if(shotsCooldown == 0)
	{
		shotsCooldown = 1;
		int i = 0;
		while(themBullets[i].onScreen == true)
		{
			i++;
			if(i > nrOfBullets - 1)
			{
				i = nrOfBullets - 1;
				break;
			}
		}
		if(themBullets[i].onScreen == false)
			if(themBullets[i].shotsFired(SpritePX + 15, SpritePY + 10))
				result = true;
	}
	return result;
}

void PlayerClass::shotsCooldownTick()
{
	shotsCooldown = (shotsCooldown + 1) % 10;
}

bool PlayerClass::shotsCooldownZero()
{
	if(shotsCooldown == 0)
		return true;
	else
		return false;
}

bool PlayerClass::dropTheBomb()
{
	if(nrOfBombs > 0 && theBomb.igniteExplosion())
	{
		--nrOfBombs;
		return true;
	}
	return false;
}

void PlayerClass::update()
{
	if(isAlive())
	{
		//Gör sig säker på att hitboxen är rätt
		HBOX.left += velocity_X;
		HBOX.bottom += velocity_Y;
		HBOX.right = HBOX.left + HBOXsizeW;
		HBOX.top = HBOX.bottom + HBOXsizeH;
		
		SpritePX = HBOX.left - SpritePaddingW;
		SpritePY = HBOX.bottom - SpritePaddingH;
		//Updaterar Cooldown
		if(!shotsCooldownZero())
			shotsCooldownTick();
	}
	else
	{
		//Om Memim inte lever så kör "ImDying()"
		ImDying();
	}
	//Updaterar skotten
	for(int i = 0; i < nrOfBullets; i++)
	{
		if(themBullets[i].onScreen)
		{
			themBullets[i].updateMove();
		}
		themBullets[i].updateSpriteIndex();
	}
	//Updaterar Bomben om den har blivit använd
	if(theBomb.onScreenExploding)
	{
		theBomb.updateExplosion();
	}
	//Uppdaterar vilken sprite som skall ritas ut
	updateSpriteIndex();
	velocity_X = velocity_Y = 0;
}

void PlayerClass::updateSpriteIndex()
{
	if(theState == Neutral)
	{
		if(spriteCheck > 3)
		{
			spriteCheck = 0;
			loopdeloopSprite = 1;
		}

		if(loopdeloopSprite == 0)
			spriteCheck = (spriteCheck + 1) % 4;

		loopdeloopSprite = (loopdeloopSprite + 1) % 3;
	}
	else if(theState == TurningUp)
	{
		if(spriteCheck > 5 || spriteCheck < 4)
		{
			spriteCheck = 4;
			loopdeloopSprite = 1;
		}

		if(spriteCheck == 4 && loopdeloopSprite == 0)
			spriteCheck = 5;

		loopdeloopSprite = (loopdeloopSprite + 1) % 10;
	}
	else if(theState == TurningDown)
	{
		if(spriteCheck > 7 || spriteCheck < 6)
		{
			spriteCheck = 6;
			loopdeloopSprite = 1;
		}

		if(spriteCheck == 6 && loopdeloopSprite == 0)
			spriteCheck = 7;
		
		loopdeloopSprite = (loopdeloopSprite + 1) % 10;
	}
	else if(theState == Dying)
	{
		//Dyingsprites stuff here
		if(spriteCheck > 10 || spriteCheck < 8)
		{
			spriteCheck = 8;
			loopdeloopSprite = 1;
		}

		if(spriteCheck < 10 && loopdeloopSprite == 0)
			spriteCheck = spriteCheck + 1;

		loopdeloopSprite = (loopdeloopSprite + 1) % 2;
	}
}

void PlayerClass::Move(int direction) //0 = up, 1 = down, 2 = right, 3 = left
{
	if(isAlive())
	{
		switch(direction)
		{
		case 0:
			if((HBOX.bottom - SpritePaddingH) - 5 > 0)
			{
				velocity_Y = -5;
				theState = TurningUp;
			}
			break;
		case 1:
			if((HBOX.bottom - SpritePaddingH) + 5 + MemSpriteH < PlayBoxResY)
			{
				velocity_Y = 5;
				theState = TurningDown;
			}
			break;
		case 2:
			if((HBOX.left - SpritePaddingW) + 5 + MemSpriteW < PlayBoxResX)
			{
				velocity_X = 5;
			}
			break;
		case 3:
			if((HBOX.left - SpritePaddingW) - 5 > 0)
			{
				velocity_X = -5;
			}
			break;
		}
	}
}

void PlayerClass::setNeutral()
{
	if(isAlive())
		theState = Neutral;
}

void PlayerClass::Respawn(int nX, int nY, int Bombs)
{
	isDead = false;
	shotsCooldown = 0;
	velocity_X = velocity_Y = 0;
	HBOX.left = nX;
	HBOX.bottom = nY;
	HBOX.right = HBOX.left + HBOXsizeW;
	HBOX.top = HBOX.bottom + HBOXsizeH;
	SpritePX = HBOX.left - SpritePaddingW;
	SpritePY = HBOX.bottom - SpritePaddingH;
	theState = Neutral;
	loopdeloopSprite = 0;
	spriteCheck = 0;
	nrOfBombs = Bombs;
}

void PlayerClass::ImDying()
{
	if(isAlive())
	{
		theState = Dying;
		HBOX.left = -300;
		HBOX.bottom = -300;
		HBOX.right = -300;
		HBOX.top = -300;
	}
	else if(theState == Dead)
		isDead = true;
		//Respawn();
	else if(spriteCheck == 10)
		theState = Dead;
}

bool PlayerClass::isAlive()
{
	if(theState == Dying)
		return false;
	else if(theState == Dead)
		return false;
	else
		return true;
}