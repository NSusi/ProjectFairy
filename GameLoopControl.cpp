#include "GameLoopControl.h"

GameLoopControl::GameLoopControl(double tps, int mfs)
{
	TICKS_PER_SECOND = tps;
	SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	MAX_FRAMESKIP = mfs;
	themLoops = 0;
	NEXT_GAME_TICK = 0.00;
	INTERPOLATION = 0.00;
}

bool GameLoopControl::LETS_UPDATE()
{
	if(difftime(clock(), 0) > NEXT_GAME_TICK)
		return true;
	else
		return false;
}

bool GameLoopControl::SKIPPING_UPDATE()
{
	if(themLoops <  MAX_FRAMESKIP)
		return false;
	else
		return true;
}

void GameLoopControl::change_TICKS_PER_SECOND(double tps)
{
	TICKS_PER_SECOND = tps;
	SKIP_TICKS = 1000 / TICKS_PER_SECOND;
}

void GameLoopControl::change_MAX_FRAMESKIP(int mfs)
{
	MAX_FRAMESKIP = mfs;
}

void GameLoopControl::initilize_NEXT_GAME_TICK()
{
	NEXT_GAME_TICK = difftime(clock(), 0) - (SKIP_TICKS * SKIP_TICKS);
}

void GameLoopControl::update_NEXT_GAME_TICK()
{
	NEXT_GAME_TICK += SKIP_TICKS;
	themLoops++;
}

void GameLoopControl::update_INTERPOLATION()
{
	INTERPOLATION = (difftime(clock(), 0) + SKIP_TICKS - NEXT_GAME_TICK) / SKIP_TICKS;
}