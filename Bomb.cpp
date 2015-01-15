#include "Bomb.h"

Bomb::Bomb()
{
	spriteIndex = spriteCheck = 0;
	onScreenExploding = false;
}

bool Bomb::igniteExplosion()
{
	if(!onScreenExploding)
	{
		spriteIndex = spriteCheck = 0;
		onScreenExploding = true;
		return true;
	}
	return false;
}

void Bomb::updateExplosion()
{
	if(onScreenExploding && spriteIndex <= 5)
	{
		spriteCheck = (spriteCheck + 1) % 15;
		if(spriteCheck == 0)
			spriteIndex++;
		return;
	}
	else
	{
		spriteIndex = spriteCheck = 0;
		onScreenExploding = false;
		return;
	}
}