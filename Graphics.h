#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DxErr.lib")

#ifndef GRAPHICS_H_INLCUDED
#define GRAPHICS_H_INCLUDED

struct TDPoint
{
	int X, Y;
};

struct ParallaxLayer
{
	ParallaxLayer();
	ParallaxLayer(float p, int d, int s, int sy, int w, int h);
	void set(float p, int d, int s, int sy, int w, int h);
	float parallax;
	int drawYOffset;
	int surface, surfaceYOffset;
	int width, height;
};

struct TextureContainer
{
	LPDIRECT3DTEXTURE9 MemimSprites[11];
	LPDIRECT3DTEXTURE9 PentarouSprites[6];
	LPDIRECT3DTEXTURE9 KidDraculaSprites[9];
	LPDIRECT3DTEXTURE9 FriendlyBullets[2];
	LPDIRECT3DTEXTURE9 EnemyBullets[2];
	LPDIRECT3DTEXTURE9 Title;
	LPDIRECT3DTEXTURE9 Paused;
	LPDIRECT3DTEXTURE9 Win;
	LPDIRECT3DTEXTURE9 ParallaxOne;
	LPDIRECT3DTEXTURE9 ParallaxTwo;
	LPDIRECT3DTEXTURE9 ParallaxThree;
};

struct ColorRGB
{
	int r; int g; int b;
	ColorRGB(int nr = 0, int ng = 0, int nb = 0){ r = nr; g = ng; b = nb; }
};

class Graphics
{
private:
	HWND theHWND; //kanske är överflödigt att spara på den nu
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dDevice;
	ParallaxLayer layers[1 + 7 + 5];
public:
	Graphics();
	TextureContainer TexCon;
	bool SetMode(int xW, int yH, bool WindowM, HWND hwnd);
	void Clear(int r = 0, int g = 0, int b = 0);
	void Flip(bool waitVSync = true);
	LPDIRECT3DTEXTURE9 LoadTheTexture(const char* filename, ColorRGB CRGB);
	void setUpParallaxBackground(); //försökte göra denna ambigious först så att den skulle kunna fungera i andra projekt men jag "hårdkodade" den istället
	void Draw(LPDIRECT3DTEXTURE9 &theImage, int posX = 0, int posY = 0, bool isTransparent = false, float theScaleX = 1.0f, float theScaleY = 1.0f);
	void drawParallaxLayers(bool isPaused);
	void setUpDrawingParallaxLayer(ParallaxLayer *theLayer, int xpos = 0, int offset = 0);
	void blittingParallaxLayer(int index, RECT* sourceRectangle, int xpos, int ypos);
	void WriteText(const char *theText, int xW, int yH, int fSize, ColorRGB theColor);
	void ReleaseD3DStuff();
	~Graphics();
};

#endif