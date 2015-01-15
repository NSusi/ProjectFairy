#include "Graphics.h"
#include "Sound.h"
#include "KeyboardInput.h"
#include "Player.h"
#include "Enemy.h"
#include <string>


#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

static const int maxEnemiesOnScreen = 3, colSize = 11, rowSize = 51/*(11*4) + 7*/; //tiles är 80*60; X tiles är ca 4 och en halv skärmar stor

struct characterContainer
{
	int x, y, type, eTime, tAngle, tY, tState, nBombs, sCheck;
	bool onScreen, isActive;
};

struct ChPoint
{
	int tsCD, cpSI, eSI, wSI, tC; //tilestepCoolDown, checkpointScanIndex, enemyScanIndex, winScanIndex, theCheckpoint;
	characterContainer player, enemies[maxEnemiesOnScreen];
};

class Game
{
private:
	HWND theHWND;
	HINSTANCE theHINSTANCE;
	int GameState; //0 = TitleScreen, 1 = GameScreen, 2 = PauseScreen, 3 = WinScreen
	int PauseCooldown, tileStepCooldown, checkPointScanIndex, enemyScanIndex, winScanIndex, theCheckpoint;
	ChPoint Checkpoint;
	Graphics grafiken;
	Sound playerShoot, enemyShoot[maxEnemiesOnScreen], playerDesu, enemyDesu[maxEnemiesOnScreen], backgroundMusics;
	KeyboardInput tangentbordet;
	PlayerClass spelaren;
	EnemyClass enemies[3], fienden, andraFienden;
	bool playingSound;
	char theLevel[colSize][rowSize];
public:
	Game(HWND hwnd, HINSTANCE hInstance);
	~Game();
	bool initGame();
	void startGame(){ this->GameState = 1; }
	void endGame(){ this->GameState = 0; }
	void togglePauseGame();
	void saveCheckpoint();
	void loadCheckpoint();
	void youWon();
	void gameInput();
	void gameUpdate();
	float calculateViewPosition(double Pos, double Velo, double Inter);
	void gameGraphics(double interpolation);
	bool isHit(const my_Rectangle &rTarget, const my_Rectangle &rIsHitting);
	int getGameState(){ return this->GameState; }
};

#endif