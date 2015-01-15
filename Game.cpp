#include "Game.h"

Game::Game(HWND hwnd, HINSTANCE hInstance)
{
	this->GameState = 0;
	this->playingSound = false;
	this->theHWND = hwnd;
	this->theHINSTANCE = hInstance;
	this->PauseCooldown = 0;
	this->tileStepCooldown = 0;
	this->checkPointScanIndex = 0;
	this->enemyScanIndex = 0;
	this->winScanIndex = 0;
	this->theCheckpoint = 0;
	for(int i = 0; i < colSize; i++)
	{
		for(int j = 0; j < rowSize; j++)
		{
			theLevel[i][j] = NULL;
		}
	}
}

Game::~Game()
{
	this->GameState = 0;
	this->playingSound = false;
	this->theHWND = 0;
	this->theCheckpoint = 0;
	grafiken.ReleaseD3DStuff();
	playerShoot.ReleaseSoundStuff();
	playerDesu.ReleaseSoundStuff();
	enemyShoot[0].ReleaseSoundStuff();
	enemyShoot[1].ReleaseSoundStuff();
	enemyShoot[2].ReleaseSoundStuff();
	enemyDesu[0].ReleaseSoundStuff();
	enemyDesu[1].ReleaseSoundStuff();
	enemyDesu[2].ReleaseSoundStuff();
	backgroundMusics.ReleaseSoundStuff();
	tangentbordet.ReleaseKIStuff();
	for(int i = 0; i < colSize; i++)
	{
		for(int j = 0; j < rowSize; j++)
		{
			theLevel[i][j] = NULL;
		}
	}
}

//Initsierar spelet
bool Game::initGame()
{
	//Aktiverar bild, ljud och tangentbord
	if(!grafiken.SetMode(800, 600, true, theHWND))
	{
		grafiken.ReleaseD3DStuff();
		return false;
	}

	if(!playerShoot.ActivateSound(theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		return false;
	}
	if(!playerDesu.ActivateSound(theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		return false;
	}
	if(!enemyShoot[0].ActivateSound(theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		return false;
	}
	if(!enemyShoot[1].ActivateSound(theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		return false;
	}
	if(!enemyShoot[2].ActivateSound(theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		return false;
	}
	if(!enemyDesu[0].ActivateSound(theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		return false;
	}
	if(!enemyDesu[1].ActivateSound(theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		return false;
	}
	if(!enemyDesu[2].ActivateSound(theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		return false;
	}
	if(!backgroundMusics.ActivateSound(theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}

	if(!playerShoot.LoadWaveSound("Sound\\playerShoot.wav", 0.192))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}
	if(!playerDesu.LoadWaveSound("Sound\\playerDeath.wav", 0.39))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}
	if(!enemyShoot[0].LoadWaveSound("Sound\\enemyShoot.wav", 0.159))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}
	if(!enemyShoot[1].LoadWaveSound("Sound\\enemyShoot.wav", 0.159))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}
	if(!enemyShoot[2].LoadWaveSound("Sound\\enemyShoot.wav", 0.159))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}
	if(!enemyDesu[0].LoadWaveSound("Sound\\enemyDeath.wav", 0.34))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}
	if(!enemyDesu[1].LoadWaveSound("Sound\\enemyDeath.wav", 0.34))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}
	if(!enemyDesu[2].LoadWaveSound("Sound\\enemyDeath.wav", 0.34))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}
	if(!backgroundMusics.LoadWaveSound("Sound\\bgm.wav", 3.99))
	//if(!backgroundMusics.OpenStream("Sound\\BGMMUSICPROJGAME.wav"))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		return false;
	}

	//playingSound = true;

	if(!tangentbordet.ActivateInput(theHINSTANCE, theHWND))
	{
		grafiken.ReleaseD3DStuff();
		playerShoot.ReleaseSoundStuff();
		playerDesu.ReleaseSoundStuff();
		enemyShoot[0].ReleaseSoundStuff();
		enemyShoot[1].ReleaseSoundStuff();
		enemyShoot[2].ReleaseSoundStuff();
		enemyDesu[0].ReleaseSoundStuff();
		enemyDesu[1].ReleaseSoundStuff();
		enemyDesu[2].ReleaseSoundStuff();
		backgroundMusics.ReleaseSoundStuff();
		tangentbordet.ReleaseKIStuff();
		return false;
	}

	//C = Checkpoint, N = Nothing, W = Win
	strcat_s(theLevel[0], "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
	strcat_s(theLevel[1], "NNNNNNNNNNNNNPNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
	strcat_s(theLevel[2], "NNNNNNNNNNNNNNNNNNNNNNNNKNNNNNNNNNNNNNNNNNNNNNNNNN");
	strcat_s(theLevel[3], "NNNNNNNNNNPNNNNNNNNNNNNNNNNNNNNNNNKNNNNNNNNNNNNNNN");
	strcat_s(theLevel[4], "NNNNNNNNNNNNNNNNNNNNNNNNNNNNPNNNNNNNNNNNPNNNNNNNNN");
	strcat_s(theLevel[5], "NNNNNNNNNNNNNNNNNNNNPNNNNNNNNNNNNNNNNNNNNNKNNNNNNN");
	strcat_s(theLevel[6], "NNNNNNNNNNKNNNNNNKNNNNNNNNNNNNKNNNNNNNNNPNNNNNNNNN");
	strcat_s(theLevel[7], "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
	strcat_s(theLevel[8], "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
	strcat_s(theLevel[9], "NNNNNNNNNNNNNNNNNNNNPNNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
   strcat_s(theLevel[10], "CNNNNNNNNNCNNNNNNNNNCNNNNNNNNNCNNNNNNNNNNNNNNNNNNW");

	grafiken.setUpParallaxBackground();

	//Läser in alla bilder
	grafiken.TexCon.MemimSprites[0] = grafiken.LoadTheTexture("Graphics\\Memim\\01MemimFly.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[1] = grafiken.LoadTheTexture("Graphics\\Memim\\02MemimFly.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[2] = grafiken.LoadTheTexture("Graphics\\Memim\\03MemimFly.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[3] = grafiken.LoadTheTexture("Graphics\\Memim\\04MemimFly.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[4] = grafiken.LoadTheTexture("Graphics\\Memim\\05MemimFly.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[5] = grafiken.LoadTheTexture("Graphics\\Memim\\06MemimFly.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[6] = grafiken.LoadTheTexture("Graphics\\Memim\\07MemimFly.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[7] = grafiken.LoadTheTexture("Graphics\\Memim\\08MemimFly.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[8] = grafiken.LoadTheTexture("Graphics\\Memim\\09MemimDeath.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[9] = grafiken.LoadTheTexture("Graphics\\Memim\\10MemimDeath.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.MemimSprites[10] = grafiken.LoadTheTexture("Graphics\\Memim\\11MemimDeath.png", ColorRGB(255, 0, 255));


	grafiken.TexCon.PentarouSprites[0] = grafiken.LoadTheTexture("Graphics\\Enemies\\01Pentarou.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.PentarouSprites[1] = grafiken.LoadTheTexture("Graphics\\Enemies\\02Pentarou.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.PentarouSprites[2] = grafiken.LoadTheTexture("Graphics\\Enemies\\03Pentarou.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.PentarouSprites[3] = grafiken.LoadTheTexture("Graphics\\Enemies\\04PentarouDeath.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.PentarouSprites[4] = grafiken.LoadTheTexture("Graphics\\Enemies\\05PentarouDeath.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.PentarouSprites[5] = grafiken.LoadTheTexture("Graphics\\Enemies\\06PentarouDeath.png", ColorRGB(255, 0, 255));

	grafiken.TexCon.KidDraculaSprites[0] = grafiken.LoadTheTexture("Graphics\\Enemies\\01KidDracula.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.KidDraculaSprites[1] = grafiken.LoadTheTexture("Graphics\\Enemies\\02KidDracula.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.KidDraculaSprites[2] = grafiken.LoadTheTexture("Graphics\\Enemies\\03KidDracula.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.KidDraculaSprites[3] = grafiken.LoadTheTexture("Graphics\\Enemies\\04KidDracula.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.KidDraculaSprites[4] = grafiken.LoadTheTexture("Graphics\\Enemies\\05KidDracula.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.KidDraculaSprites[5] = grafiken.LoadTheTexture("Graphics\\Enemies\\06KidDracula.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.KidDraculaSprites[6] = grafiken.LoadTheTexture("Graphics\\Enemies\\07KidDraculaDeath.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.KidDraculaSprites[7] = grafiken.LoadTheTexture("Graphics\\Enemies\\08KidDraculaDeath.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.KidDraculaSprites[8] = grafiken.LoadTheTexture("Graphics\\Enemies\\09KidDraculaDeath.png", ColorRGB(255, 0, 255));

	grafiken.TexCon.FriendlyBullets[0] = grafiken.LoadTheTexture("Graphics\\Bullets\\01MemimBullet.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.FriendlyBullets[1] = grafiken.LoadTheTexture("Graphics\\Bullets\\02MemimBullet.png", ColorRGB(255, 0, 255));

	grafiken.TexCon.EnemyBullets[0] = grafiken.LoadTheTexture("Graphics\\Bullets\\01EnemyBullet.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.EnemyBullets[1] = grafiken.LoadTheTexture("Graphics\\Bullets\\02EnemyBullet.png", ColorRGB(255, 0, 255));

	grafiken.TexCon.Title = grafiken.LoadTheTexture("Graphics\\Screens\\Title.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.Paused = grafiken.LoadTheTexture("Graphics\\Screens\\Pause.png", ColorRGB(255, 0, 255));
	grafiken.TexCon.Win = grafiken.LoadTheTexture("Graphics\\Screens\\Win.png", ColorRGB(255, 0, 255));

	grafiken.TexCon.ParallaxOne = grafiken.LoadTheTexture("Graphics\\Background\\Sky.png", ColorRGB(0, 0, 0));
	grafiken.TexCon.ParallaxTwo = grafiken.LoadTheTexture("Graphics\\Background\\MountainsGrass.png", ColorRGB(0, 0, 0));
	grafiken.TexCon.ParallaxThree = grafiken.LoadTheTexture("Graphics\\Background\\Clouds.png", ColorRGB(0, 0, 0));

	//Sätter ut vilka tiles som skall scannas
	//-1 eftersom de ökar +1 innan det scannas
	checkPointScanIndex = -1;
	enemyScanIndex = 8;
	winScanIndex = 8;

	theCheckpoint = 0;

	return true;
}

//Toggla Pause
void Game::togglePauseGame()
{
	if(GameState == 0)
		return;
	if(PauseCooldown == 0)
	{
		if(GameState == 2)
			this->GameState = 1;
		else if(GameState == 1)
			this->GameState = 2;
		this->PauseCooldown = 5;
	}
}

void Game::saveCheckpoint()
{
	//sparar alla scan indexes
	Checkpoint.tsCD = this->tileStepCooldown;
	Checkpoint.cpSI = this->checkPointScanIndex;
	Checkpoint.eSI = this->enemyScanIndex;
	Checkpoint.wSI = this->winScanIndex;
	Checkpoint.tC = this->theCheckpoint;

	//sparar spelaren
	Checkpoint.player.x = this->spelaren.getX();
	Checkpoint.player.y = this->spelaren.getY();
	//Sparar Bomber
	Checkpoint.player.nBombs = this->spelaren.getNrOfBombs();
	//Så att det inte sparas massa junk på minnesplatserna
	Checkpoint.player.type = 0;
	Checkpoint.player.eTime = 0;
	Checkpoint.player.tAngle = 0;
	Checkpoint.player.tY = 0;
	Checkpoint.player.tState = 0;
	Checkpoint.player.sCheck = 0;
	Checkpoint.player.onScreen = true;

	//sparar fienderna
	for(int i = 0; i < maxEnemiesOnScreen; i++)
	{
		Checkpoint.enemies[i].type = this->enemies[i].getType();
		Checkpoint.enemies[i].x = this->enemies[i].getX();
		Checkpoint.enemies[i].y = this->enemies[i].getY();
		Checkpoint.enemies[i].tY = this->enemies[i].getTargetY();
		Checkpoint.enemies[i].onScreen = this->enemies[i].onScreen;
		Checkpoint.enemies[i].isActive = this->enemies[i].isActive;
	}
}

void Game::loadCheckpoint()
{
	//laddar alla scan indexes
	this->tileStepCooldown = Checkpoint.tsCD;
	this->checkPointScanIndex = Checkpoint.cpSI;
	this->enemyScanIndex = Checkpoint.eSI;
	this->winScanIndex = Checkpoint.wSI;
	this->theCheckpoint = Checkpoint.tC;

	//Respawna spelaren
	spelaren.flushBullets();
	spelaren.Respawn(Checkpoint.player.x, Checkpoint.player.y, Checkpoint.player.nBombs);

	//Unspawna alla fiender
	for(int i = 0; i < maxEnemiesOnScreen; i++)
	{
		enemies[i].flushBullets();
		enemies[i].UnSpawn();
	}

	//Respawna alla fiender
	for(int i = 0; i < maxEnemiesOnScreen; i++)
	{
		enemies[i].SpawnEnemy(Checkpoint.enemies[i].type, //typen
			Checkpoint.enemies[i].x, Checkpoint.enemies[i].y, // x och y
			Checkpoint.enemies[i].tY, Checkpoint.enemies[i].onScreen, //targetY och onScreen
			Checkpoint.enemies[i].isActive); // if the enemy is active
	}
}

void Game::youWon()
{
	this->GameState = 3;
	backgroundMusics.StopTheSound();
}

//Input hantering
void Game::gameInput()
{
	BYTE *diKeys = tangentbordet.UpdateInputState();
	if(diKeys != 0)
	{
		
		//Alltid tillgång till att stänga av spelet genom 'ESC'
		if(diKeys[DIK_ESCAPE] & 0x80) //"FIXED" - dvs att Escape fungerar inte i andra program när Photoshop CS2 är igång //denna slutade fungera helt plötsligt //DIK_ESCAPE vill inte vara med längre...
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		
		if(this->getGameState() == 1)
		{
			//Rätt uppenbart; är spelet igång så styr man Memim
			spelaren.setNeutral();

			if(diKeys[DIK_SPACE] & 0x80)
				if(spelaren.shootEm())
					playerShoot.PlayTheSound(false);

			if(diKeys[DIK_B] & 0x80)
				if(spelaren.dropTheBomb())
					playerShoot.PlayTheSound(false);

			if((diKeys[DIK_UP] & 0x80) || (diKeys[DIK_W] & 0x80))
				spelaren.Move(0);
			else if((diKeys[DIK_DOWN] & 0x80) || (diKeys[DIK_S] & 0x80))
				spelaren.Move(1);

			if((diKeys[DIK_RIGHT] & 0x80) || (diKeys[DIK_D] & 0x80))
				spelaren.Move(2);
			else if((diKeys[DIK_LEFT] & 0x80) || (diKeys[DIK_A] & 0x80))
				spelaren.Move(3);

			if(diKeys[DIK_P] & 0x80)
				this->togglePauseGame();
		}
		else if(this->getGameState() == 0)
		{
			//Om titlescreen så starta spelet med enter
			if(diKeys[DIK_RETURN] & 0x80)
			{
				this->startGame();
				backgroundMusics.PlayTheSound(true);
			}
		}
		else if(this->getGameState() == 2 && this->getGameState() != 0 && this->getGameState() != 3)
		{
			//Pause funktionen
			if(diKeys[DIK_P] & 0x80)
				this->togglePauseGame();
		}
	}
}

//Uppdatera spelet
void Game::gameUpdate()
{
	if(GameState == 1)
	{
		//Updatera spelaren
		spelaren.update();

		//Uppdatera fienderna
		for(int o = 0; o < maxEnemiesOnScreen; o++)
		{
			if(enemies[o].update())
			{
				enemyShoot[o].PlayTheSound(false);
			}

		}

		//Hitdetection
		if(spelaren.isAlive())
		{
			for(int i = 0; i < maxEnemiesOnScreen; i++)
			{
				if(enemies[i].onScreen)
				{
					if(this->isHit(spelaren.getRect(), enemies[i].getRect()))
					{
						spelaren.ImDying();
						playerDesu.PlayTheSound(false);
					}
					else
					{
						for(int j = 0; j < enemies[i].nrOfBullets; j++)
						{
							if(enemies[i].themBullets[j].onScreen)
							{
								if(this->isHit(spelaren.getRect(), enemies[i].themBullets[j].getRect()))
								{
									spelaren.ImDying();
									enemies[i].themBullets[j].DestroyBullet();
									playerDesu.PlayTheSound(false);
								}
							}
						}
					}
					for(int j = 0; j < spelaren.nrOfBullets; j++)
					{
						if(spelaren.themBullets[j].onScreen)
						{
							if(this->isHit(enemies[i].getRect(), spelaren.themBullets[j].getRect()))
							{
								enemies[i].Dying();
								spelaren.themBullets[j].DestroyBullet();
								enemyDesu[i].PlayTheSound(false);
							}
						}
					}
					if(spelaren.theBomb.onScreenExploding)
					{
						//fienden dör, det ska inte behövas att spela upp ljud då explosionen ska höras över allt annat
						enemies[i].Dying();
					}
				}
			}
		}
		if(spelaren.isDead)
		{
			this->loadCheckpoint();
		}

		//Flyttar till nästa tile om det är dags, annars så räkna ner cooldown
		if(tileStepCooldown > 0)
		{
			tileStepCooldown--;
		}
		else if(tileStepCooldown == 0)
		{
			//Reset'ar cooldown
			tileStepCooldown = 70; //Måste justeras
			
			//Ökar scanindex
			if(checkPointScanIndex >= -1 && checkPointScanIndex < rowSize)
				checkPointScanIndex++;
			if(enemyScanIndex >= 8 && enemyScanIndex < rowSize)
				enemyScanIndex++;
			if(winScanIndex >= 8 && enemyScanIndex < rowSize)
				winScanIndex++;
			
			//Scannar
			if(this->theLevel[10][checkPointScanIndex] == 'C')
			{
				//Spara checkpoint
				theCheckpoint++;
				this->saveCheckpoint();
			}
			if(this->theLevel[10][winScanIndex] == 'W')
			{
				//Win the game! huzzah!
				this->youWon();
			}
			if(this->GameState == 1)
			{
				//Spawnar fienderna
				for(int i = 0; i < colSize - 1; i ++)
				{
					if(this->theLevel[i][enemyScanIndex] == 'P')
					{
						//Spawna en Pentarou på i * 60
						for(int j = 0; j < maxEnemiesOnScreen; j++)
						{
							if(!enemies[j].isActive)
							{
								enemies[j].SpawnEnemy(0, 950, (i*60) + 5);
								break;
							}
						}
					}
					else if(this->theLevel[i][enemyScanIndex] == 'K')
					{
						//Spawna en Kid Dracula på i * 60
						for(int j = 0; j < maxEnemiesOnScreen; j++)
						{
							if(!enemies[j].isActive)
							{
								enemies[j].SpawnEnemy(1, 950, (i*60) + 10);
								break;
							}
						}
					}
				}
			}
		}
	}

	//Cooldown för Pause så att det inte pausas "tysn" gånger i sekunden
	if(PauseCooldown > 0)
		PauseCooldown--;
	//Uppdaterar bakgrundsmusiken, borde vara någon annanstans men då spelloopen inte sköts av en timer så fungerar detta. 
	//if(!backgroundMusics.UpdateStream())
	//{
	//	PostQuitMessage(0);
	//}
}

float Game::calculateViewPosition(double Pos, double Velo, double Inter)
{
	return (float)(Pos + (Velo * Inter));
}

//Rita ut grafiken
void Game::gameGraphics(double interpolation)
{
	grafiken.Clear(255, 255, 255);

	if(this->getGameState() == 0)
	{
		//Om gamestate är 0 så ritar man ut titlescreen
		//Skulle behöva att göra om titlescreen igen, då den blev för liten i upplösningen
		grafiken.Draw(grafiken.TexCon.Title, 0, 0, false, 1.562, 1.171);
	}
	else if(this->getGameState() == 3)
	{
		//Om gamestate är 3 så ritar man ut winscreen
		grafiken.Draw(grafiken.TexCon.Win, 0, 0, false, 1.562, 1.171); //Kanske gör en bättre winscreen senare
	}
	else if(getGameState() != 0 && getGameState() != 3)
	{
		
		//Ritar ut ParallaxaBakgrunden, kollar om det är pausat eller inte.
		if(this->GameState != 2)
			grafiken.drawParallaxLayers(false);
		else
			grafiken.drawParallaxLayers(true);

		//Ritar ut Memim aka spelaren
		
		grafiken.Draw(grafiken.TexCon.MemimSprites[spelaren.getSpriteIndex()], this->calculateViewPosition(spelaren.getX(), spelaren.getVeloX(), interpolation), this->calculateViewPosition(spelaren.getY(), spelaren.getVeloY(), interpolation), true);

		//Ritar ut Memims aka spelarens missiler
		for(int i = 0; i < spelaren.nrOfBullets; i++)
			if(spelaren.themBullets[i].onScreen)
				grafiken.Draw(grafiken.TexCon.FriendlyBullets[spelaren.themBullets[i].getSpriteIndex()],  this->calculateViewPosition(spelaren.themBullets[i].getX(), spelaren.themBullets[i].getVeloX(), interpolation), this->calculateViewPosition(spelaren.themBullets[i].getY(), spelaren.themBullets[i].getVeloY(), interpolation), true);

		//Ritar ut fiender och deras missiler om de finns på skärmen, onScreen == true;
		for(int i = 0; i < maxEnemiesOnScreen; i++)
		{
			if(enemies[i].onScreen)
			{
				if(enemies[i].getType() == 0)
					grafiken.Draw(grafiken.TexCon.PentarouSprites[enemies[i].getSpriteIndex()], this->calculateViewPosition(enemies[i].getX(), enemies[i].getVeloX(), interpolation), this->calculateViewPosition(enemies[i].getY(), enemies[i].getVeloY(), interpolation), true);
				else if(enemies[i].getType() == 1)
					grafiken.Draw(grafiken.TexCon.KidDraculaSprites[enemies[i].getSpriteIndex()], this->calculateViewPosition(enemies[i].getX(), enemies[i].getVeloX(), interpolation), this->calculateViewPosition(enemies[i].getY(), enemies[i].getVeloY(), interpolation), true);
			}
			for(int j = 0; j < enemies[i].nrOfBullets; j++)
				if(enemies[i].themBullets[j].onScreen)
					grafiken.Draw(grafiken.TexCon.EnemyBullets[enemies[i].themBullets[j].getSpriteIndex()], this->calculateViewPosition(enemies[i].themBullets[j].getX(),enemies[i].themBullets[j].getVeloX(), interpolation), this->calculateViewPosition(enemies[i].themBullets[j].getY(),enemies[i].themBullets[j].getVeloY(), interpolation), true);
		}
	}
	/////////////////////////TEMPORARY
	if(spelaren.theBomb.onScreenExploding)
	{
		switch(spelaren.theBomb.getSpriteIndex())
		{
		case 0:
			grafiken.WriteText("BOMB EXPLODING! FRAME: 0", 150, 250, 30, ColorRGB(150, 150, 150));
			break;
		case 1:
			grafiken.WriteText("BOMB EXPLODING! FRAME: 1", 150, 250, 30, ColorRGB(150, 150, 150));
			break;
		case 2:
			grafiken.WriteText("BOMB EXPLODING! FRAME: 2", 150, 250, 30, ColorRGB(150, 150, 150));
			break;
		case 3:
			grafiken.WriteText("BOMB EXPLODING! FRAME: 3", 150, 250, 30, ColorRGB(150, 150, 150));
			break;
		case 4:
			grafiken.WriteText("BOMB EXPLODING! FRAME: 4", 150, 250, 30, ColorRGB(150, 150, 150));
			break;
		case 5:
			grafiken.WriteText("BOMB EXPLODING! FRAME: 5", 150, 250, 30, ColorRGB(150, 150, 150));
			break;
		}
	}
	/////////////////////////TEMPORARY


	//Om gamestate är 2 så blaffar man ut "PAUSED" över spelskärmen
	if(getGameState() == 2)
			grafiken.Draw(grafiken.TexCon.Paused, 150, 200);

	///////////////////////////////////////////
	///////TA BORT SENARE!!!!!////////////////
	/////////////////////////////////////////
	if(theCheckpoint > 0 && theCheckpoint < 5)
	{
		switch(spelaren.getNrOfBombs())
		{
			case 0:
				grafiken.WriteText("BOMBS: 0", 100, 550, 20, ColorRGB(150, 150, 150));
				break;
			case 1:
				grafiken.WriteText("BOMBS: 1", 100, 550, 20, ColorRGB(150, 150, 150)); 
				break;
			case 2:
				grafiken.WriteText("BOMBS: 2", 100, 550, 20, ColorRGB(150, 150, 150)); 
				break;
			case 3:
				grafiken.WriteText("BOMBS: 3", 100, 550, 20, ColorRGB(150, 150, 150));
				break;
		}
	}

	if(theCheckpoint > 0 && theCheckpoint < 5)
	{
		switch(theCheckpoint)
		{
			case 1:
				grafiken.WriteText("Checkpoint: 1", 500, 550, 20, ColorRGB(150, 150, 150));
				break;
			case 2:
				grafiken.WriteText("Checkpoint: 2", 500, 550, 20, ColorRGB(150, 150, 150)); 
				break;
			case 3:
				grafiken.WriteText("Checkpoint: 3", 500, 550, 20, ColorRGB(150, 150, 150)); 
				break;
			case 4:
				grafiken.WriteText("Checkpoint: 4", 500, 550, 20, ColorRGB(150, 150, 150));
				break;
		}
	}
	///////////////////////////////////////////
	///////TA BORT SENARE!!!!!////////////////
	/////////////////////////////////////////
	
	//Skickar ut grafiken som nyss ritades på en yta
	grafiken.Flip();
}

//Hit detection med boundingboxes
bool Game::isHit(const my_Rectangle &rTarget, const my_Rectangle &rIsHitting)
{
	if(rTarget.right < rIsHitting.left)
	{
		return false;
	}
	if(rTarget.left > rIsHitting.right)
	{
		return false;
	}
	if(rTarget.bottom > rIsHitting.top)
	{
		return false;
	}
	if(rTarget.top < rIsHitting.bottom)
	{
		return false;
	}
	return true;
}