#include "Enemy.h"
static const int SpritePaddingW = 5, SpritePaddingH = 3;

EnemyClass::EnemyClass()
{
}

EnemyClass::~EnemyClass()
{
}

void EnemyClass::SpawnEnemy(int typoType, int nX, int nY, int tY, bool oS, bool iA)
{
	shotsCooldown = 0;
	
	elapsedTime = 0;
	theAngle = 0;
	
	if(tY == -111)
		targetY = nY;
	else
		targetY = tY;

	cosLine = nY;

	HBOX.left = nX;
	HBOX.bottom = nY;
	HBOX.right = HBOX.left + BoxH;
	HBOX.top = HBOX.bottom + BoxW;
	
	SpritePX = HBOX.left - SpritePaddingW;
	SpritePY = HBOX.bottom - SpritePaddingH;
	
	onScreen = oS;
	isActive = iA;
	
	theType = typoType;
	theState = 0;
	spriteCheck = 0;
	if(typoType == 0)
	{
		velocity_X = -3;
		velocity_Y = 0;
	}
	else if(typoType == 1)
	{
		velocity_X = -2;
		velocity_Y = 3;
	}

	for(int i = 0; i < nrOfBullets; i++)
	{
		themBullets[i].theBullet = EnemyBullet;
	}
}

void EnemyClass::flushBullets()
{
	for(int i = 0; i < nrOfBullets; i++)
	{
		themBullets[i].DestroyBullet();
	}
}

bool EnemyClass::shootEm()
{
	bool result = false;
	if(onScreen && SpritePX > 0)
	{
		if(!this->shotsCooldownZero())
			this->shotsCooldownTick();

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
				if(themBullets[i].shotsFired(SpritePX + 125, SpritePY))
					result = true;
		}
	}
	return result;
}

void EnemyClass::shotsCooldownTick()
{
	shotsCooldown = (shotsCooldown + 1) % 100;
}

bool EnemyClass::shotsCooldownZero()
{
	if(shotsCooldown == 0)
		return true;
	else
		return false;
}

void EnemyClass::Move()
{
	//UnSpawn
	if(SpritePX <= 0 - 30 || SpritePY <= 0 - 25 || SpritePY >= 600 + 25/* || !isAlive*/)
	{
		this->UnSpawn();
	}

	if(onScreen)
	{
		if(theType == 0)
		{
			velocity_X = -3;
			velocity_Y = 0;
		}
		else if(theType == 1)
		{
			int YScale = 100;
			int CosSpeed = 1;
			elapsedTime++;

			//Nästa steg i sinus rörelsen etc etc etc...
			velocity_X = -2;

			if(targetY >= cosLine)
			{
				if(HBOX.bottom > targetY)
					targetY = (cos((double)theAngle) * YScale) + cosLine;
				velocity_Y = 3;
				theState = 2;
			}
			else if(targetY <= cosLine)
			{
				if(HBOX.bottom < targetY)
					targetY = (cos((double)theAngle) * YScale) + cosLine;
				velocity_Y = -3;
				theState = 1;
			}

			theAngle += CosSpeed * elapsedTime;
		}
	}
	else
	{
		if(HBOX.left > 815)
		{
			HBOX.left = 815;
		}
		else
		{
			HBOX.left = 810;
			onScreen = true;
		}
	}
}

void EnemyClass::UnSpawn()
{
	onScreen = false;
	isActive = false;
	SpritePX = -150;
	SpritePY = -150;
	HBOX.left = -150;
	HBOX.bottom = -150;
	HBOX.right = -150;
	HBOX.top = -150;
}

bool EnemyClass::update()
{
	bool result = false;
	if(isAlive())
	{
		//Updaterar position
		this->Move();
		//Uppdaterar hitbox
		HBOX.left += velocity_X;
		HBOX.bottom += velocity_Y;
		HBOX.right = HBOX.left + BoxW;
		HBOX.top = HBOX.bottom + BoxH;
		SpritePX = HBOX.left - SpritePaddingW;
		SpritePY = HBOX.bottom - SpritePaddingH;
		//Uppdaterar skott
		if(this->shootEm())
			result = true;
	}
	else
	{
		Dying();
	}

	for(int i = 0; i < nrOfBullets; i++)
	{
		if(themBullets[i].onScreen)
		{
			themBullets[i].updateMove();
		}
		themBullets[i].updateSpriteIndex();
	}
	//Uppdaterar vilken sprite som skall ritas ut
	updateSpriteIndex();

	return result;
}

void EnemyClass::updateSpriteIndex()
{
	if(theState == 0) //Universal för båda typerna
	{
		if(theType == 0)
		{
			if(spriteCheck > 2)
			{
				spriteCheck = 0;
				loopdeloopSprite = 1;
			}

			if(loopdeloopSprite == 0)
				spriteCheck = (spriteCheck + 1) % 3;

			loopdeloopSprite = (loopdeloopSprite + 1) % 5;
		}
		else if(theType == 1)
		{
			if(spriteCheck > 0)
			{
				spriteCheck = 0;
				loopdeloopSprite = 1;
			}

			if(loopdeloopSprite == 0)
			{
				if(spriteCheck == 0)
					spriteCheck = 3;
				else if(spriteCheck == 3)
					spriteCheck = 0;
			}
		}
	}
	else if(theState == 1)//DOWN
	{
		if(spriteCheck > 2 || spriteCheck < 0)
		{
			spriteCheck = 0;
			loopdeloopSprite = 3;
		}

		if(spriteCheck == 0 && loopdeloopSprite == 0)
			spriteCheck = 1;
		else if(spriteCheck == 1 && loopdeloopSprite == 0)
			spriteCheck = 2;

		loopdeloopSprite = (loopdeloopSprite + 1) % 6;

	}
	else if(theState == 2) //UP
	{
		if(spriteCheck > 5 || spriteCheck < 3)
		{
			spriteCheck = 3;
			loopdeloopSprite = 3;
		}

		if(spriteCheck == 3 && loopdeloopSprite == 0)
			spriteCheck = 4;
		else if(spriteCheck == 4 && loopdeloopSprite == 0)
			spriteCheck = 5;

		loopdeloopSprite = (loopdeloopSprite + 1) % 6;
	}
	else if(theState == 3)
	{
		if(theType == 0)
		{
			if(spriteCheck > 5 || spriteCheck < 3)
			{
				spriteCheck = 3;
				loopdeloopSprite = 1;
			}

			if(spriteCheck < 5 && loopdeloopSprite == 0)
			spriteCheck = spriteCheck + 1;

			loopdeloopSprite = (loopdeloopSprite + 1) % 5;
		}
		else if(theType == 1)
		{
			if(spriteCheck > 8 || spriteCheck < 6)
			{
				spriteCheck = 6;
				loopdeloopSprite = 1;
			}

			if(spriteCheck < 8 && loopdeloopSprite == 0)
			spriteCheck = spriteCheck + 1;

			loopdeloopSprite = (loopdeloopSprite + 1) % 5;
		}
	}
}

void EnemyClass::Dying()
{
	if(theType == 0)
	{
		if(isAlive())
		{
			theState = 3;
			HBOX.left = -150;
			HBOX.bottom = -150;
			HBOX.right = -150;
			HBOX.top = -150;
		}
		else if(theState == 4)
			UnSpawn();
		else if(spriteCheck == 5)
			theState = 4;
	}
	else if(theType == 1)
	{
		if(isAlive())
		{
			theState = 3;
			HBOX.left = -150;
			HBOX.bottom = -150;
			HBOX.right = -150;
			HBOX.top = -150;
		}
		else if(theState == 4)
			UnSpawn();
		else if(spriteCheck == 8)
			theState = 4;
	}
}

bool EnemyClass::isAlive()
{
	if(theState == 3)
		return false;
	else if(theState == 4)
		return false;
	else
		return true;
}