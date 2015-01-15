#include "KeyboardInput.h"

KeyboardInput::KeyboardInput(/*HWND hwnd*/)
{
	di8input = NULL;
	di8device = NULL;
}

KeyboardInput::~KeyboardInput()
{
	this->ReleaseKIStuff();
}

bool KeyboardInput::ActivateInput(HINSTANCE hInstance, HWND hwnd)
{
	theHWND = hwnd;

	this->ReleaseKIStuff();

	HRESULT result = DirectInput8Create(
		hInstance, //hInstance
		DIRECTINPUT_VERSION, // DIRECTINPUT_VERSION
		IID_IDirectInput8, // typ av interface
		(LPVOID*)&di8input, // adressen till en pekare av typen IDirectInput8
		NULL);

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH INPUTACTIVATION");
		return false;
	}

	result = di8input->CreateDevice(
		GUID_SysKeyboard, //GUID för enheten
		&di8device, // adressen till en pekare till enheten som ska skapas.
		NULL); // NULL

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH INPUTDEVICE CREATION");
		return false;
	}

	result = di8device->SetCooperativeLevel(theHWND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH INPUTCOOPERATION");
		return false;
	}
	
	result = di8device->SetDataFormat(&c_dfDIKeyboard);

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH SETTING DATAFORMAT FOR INPUT");
		return false;
	}

	di8device->Acquire();

	return true;
}

/*int*/ BYTE *KeyboardInput::UpdateInputState() //skickar egentligen tillbaka Keyboard_Device
{
    HRESULT result;

    //Poll för "current state"
	result = di8device->Poll();
    if(FAILED(result))
    {
        //Om input strömmen är avbruten så reacquire tills vi har kontakt igen.
        result = di8device->Acquire();

        while(result == DIERR_INPUTLOST)
            result = di8device->Acquire();

        //result kan vara DIERR_OTHERAPPHASPRIO så vi stänger ner och försöker igen senare. 
        return 0;
		//return -1;
    }
    
	if(FAILED(di8device->GetDeviceState(256, &diKeys)))
		return 0; //Device't skulle ha blivit aquired under Poll()


	return diKeys;
}

HRESULT KeyboardInput::Aquire()
{
	HRESULT result;
	if(di8device)
	{
		result = di8device->Acquire();
		return result;
	}
	return 0;
}

HRESULT KeyboardInput::Unaquire()
{
	HRESULT result;
	if(di8device)
	{
		result = di8device->Unacquire();
		return result;
	}
	return 0;
}

//bool KeyboardInput::Aquire()
//{
//	if(di8device)
//	{
//		HRESULT result = di8device->Acquire();
//		if(FAILED(result))
//			return false;
//		else
//			return true;
//	}
//	return false;
//}

//bool KeyboardInput::Unaquire()
//{
//	HRESULT result = di8device->Unacquire();
//	if(FAILED(result))
//		return false;
//	else
//		return true;
//}

void KeyboardInput::ReleaseKIStuff()
{
	if(di8device)
	{
		di8device->Unacquire();
		di8device->Release();
		di8device = NULL;
	}
	if(di8input)
	{
		di8input->Release();
		di8input = NULL;
	}
}