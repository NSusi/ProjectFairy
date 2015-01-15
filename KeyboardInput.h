#ifndef GRAPHICS_H_INLCUDED
#define GRAPHICS_H_INCLUDED

#include <Windows.h>
#include <DxErr.h>
#pragma comment(lib, "DxErr.lib")

#endif

//#define DIRECTINPUT_VERSION 0x0800 //Säger att jag behöver en nyare version av directinput
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#ifndef KEYBOARDINPUT_H_INLCUDED
#define KEYBOARDINPUT_H_INCLUDED

enum MyInputs
{
	MDIK_ESCAPE,
	MDIK_I,
	MDIK_J,
	MDIK_P,
	MDIK_SPACE,
	MDIK_UP,
	MDIK_UPRIGHT,
	MDIK_UPLEFT,
	MDIK_DOWN,
	MDIK_DOWNRIGHT,
	MDIK_DOWNLEFT,
	MDIK_RIGHT,
	MDIK_LEFT
};

class KeyboardInput
{
private:
	HWND theHWND; //kanske är överflödigt att spara på den nu
	LPDIRECTINPUT8 di8input;
	LPDIRECTINPUTDEVICE8 di8device;
	BYTE diKeys[256];
public:
	KeyboardInput();
	bool ActivateInput(HINSTANCE hInstance, HWND hwnd);
	//bool SetBuffer(int nrOfSecs = 1);
	/*int*/ BYTE *UpdateInputState();
	HRESULT Aquire();
	HRESULT Unaquire();
	//bool Aquire();
	//bool Unaquire();
	void ReleaseKIStuff();
	~KeyboardInput();
};

#endif