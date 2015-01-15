#include <time.h>

#ifndef GAMELOOPCONTROL_H_INCLUDED
#define GAMELOOPCONTROL_H_INCLUDED

class GameLoopControl
{
private:
	double TICKS_PER_SECOND, SKIP_TICKS, NEXT_GAME_TICK, INTERPOLATION;
	int MAX_FRAMESKIP, themLoops;
public:
	GameLoopControl(double tps = 30.00, int mfs = 5); //tps = ticks per second, mfs = max frame skip
	bool LETS_UPDATE();
	bool SKIPPING_UPDATE();
	void change_TICKS_PER_SECOND(double tps = 30.00);
	void change_MAX_FRAMESKIP(int mfs = 5);
	void initilize_NEXT_GAME_TICK();
	void update_NEXT_GAME_TICK();
	void update_INTERPOLATION();
	void resetLoops(){ themLoops = 0; }
	int getCurrentLoop(){ return themLoops; }
	double get_NEXT_GAME_TICK(){ return NEXT_GAME_TICK; }
	double get_INTERPOLATION(){ return INTERPOLATION; }
};

#endif