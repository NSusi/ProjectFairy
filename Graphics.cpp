#include "Graphics.h"

ParallaxLayer::ParallaxLayer()
{
	this->set(0.0, 0, 0, 0, 0, 0);
}

ParallaxLayer::ParallaxLayer(float p, int d, int s, int sy, int w, int h)
{
	this->set(p, d, s, sy, w, h);
}

void ParallaxLayer::set(float p, int d, int s, int sy, int w, int h)
{
	parallax = p; 
	drawYOffset = d; 
	surface = s; 
	surfaceYOffset = sy; 
	width = w; 
	height = h; 
}

Graphics::Graphics(/*HWND hwnd*/)
{
	d3d = NULL;
	d3dDevice = NULL;
	for(int i = 0; i < 11; i++)
	{
		if(i < 11)
			TexCon.MemimSprites[i] = NULL;
		if(i < 6)
			TexCon.PentarouSprites[i] = NULL;
		if(i < 9)
			TexCon.KidDraculaSprites[i] = NULL;
		if(i < 2)
		{
			TexCon.FriendlyBullets[i] = NULL;
			TexCon.EnemyBullets[i] = NULL;
		}
	}
	TexCon.Title = NULL;
	TexCon.Paused = NULL;
	TexCon.Win = NULL;
	TexCon.ParallaxOne = NULL;
	TexCon.ParallaxTwo = NULL;
	TexCon.ParallaxThree = NULL;
}

Graphics::~Graphics()
{
	this->ReleaseD3DStuff();
}

//Ta bort om jag inte behöver globalt skit senare
//void Graphics::setHWND(HWND hwnd)
//{
//	theHWND = hwnd;
//}

bool Graphics::SetMode(int xW, int yH, bool WindowM, HWND hwnd)
{
	theHWND = hwnd;
	this->ReleaseD3DStuff();
	D3DPRESENT_PARAMETERS presentParameters = { 0 };
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if(WindowM)
	{
		presentParameters.Windowed = true;
		presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	}
	else if(!WindowM)
	{
		presentParameters.Windowed = false;
		presentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferWidth = xW;
	presentParameters.BackBufferHeight = yH;
	presentParameters.BackBufferCount = 1;
	presentParameters.hDeviceWindow = theHWND;

	HRESULT result = d3d->CreateDevice(
		D3DADAPTER_DEFAULT,						//ta första bästa grafikkort...
		D3DDEVTYPE_HAL,							//... som är hårdvaruaccelerat
		theHWND,								//koppla till fönstret vi skapade
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//kompabilitetsläge
		&presentParameters,						//parametrarna som vi skapade
		&d3dDevice);							//om allt gick bra så hamnar vår grafikenhet här

	if(FAILED(result))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(result),
			DXGetErrorString(result),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH DEVICE");
		return false;
	}

	return true;
}

void Graphics::Clear(int r, int g, int b)
{
	d3dDevice->Clear(
		0, 0,						// rektanglarna(pekare, antal) som ska rensas, om 0 töms hela fönstret/skärmen
		D3DCLEAR_TARGET,			// target innebär att det är färgen som ska rensas
		D3DCOLOR_XRGB(r, g, b), 	// den färg som används för att rensa
		0, 0);
}

void Graphics::Flip(bool waitVSync)
{
	if(waitVSync)
	{
		LPDIRECT3DSWAPCHAIN9 d3dSwapChain = NULL;
		d3dDevice->GetSwapChain(0, &d3dSwapChain);
		d3dSwapChain->GetDevice(&d3dDevice);
		d3dSwapChain->Present(0, 0, 0, 0, D3DPRESENT_INTERVAL_DEFAULT);
		d3dSwapChain->Release();
		d3dSwapChain = NULL;
	}
	else
	{
		d3dDevice->Present(0, 0, 0, 0);
	}
}

LPDIRECT3DTEXTURE9 Graphics::LoadTheTexture(const char* filename, ColorRGB TransparentColorKey)
{
	LPDIRECT3DTEXTURE9 newTexture;
	HRESULT theResult = D3DXCreateTextureFromFileEx(
		d3dDevice,
		filename,
		0, 0,						// bredd och höjd på bilden. 0 så avrundas storleken till 
		// nästa 2-potens. D3DX_DEFAULT_NONPOW2 kan användas för
		// att D3D inte ska avrunda.
		1,							// ”mip-nivåer”, se SDK:n
		0,							// ”användning”, kan lämnas till 0
		D3DFMT_UNKNOWN,				// D3D väljer lämpligt bildformat
		D3DPOOL_MANAGED,			// D3D sköter resurshanteringen
		D3DX_DEFAULT,				// filter för skalning av bilden
		D3DX_DEFAULT,				// filter för skalning av mip-nivåer
		D3DCOLOR_XRGB(TransparentColorKey.r, TransparentColorKey.g, TransparentColorKey.b),	// ”color key”
		0, 0,						// bildinformation och palett, behövs ej
		&newTexture);

	if(FAILED(theResult))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(theResult),
			DXGetErrorString(theResult),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH IMAGE");
	}

	return newTexture;
}

void Graphics::setUpParallaxBackground()
{
	//
	//Skär upp bilderna för att få "3D" effekten
	//
	layers[0].set(0.4f, 115, 2, 2, 256, 93); //2 = ParallaxTwo, alltså, mountains and grass

	for(int i = 0; i < 7; i++)
		layers[1 + i].set(1.0f + i / 7.0f, 205 + i * 5, 2, 93 + i * 5, 256, 5);

	//layers[8].set(0.7f, 0/*-15*/, 3, 0, 256, 26); //3 = ParallaxTree, alltså, Clouds
	//layers[9].set(0.6f, 26, 3, 29, 256, 49);
	//layers[10].set(0.4f, 75, 3, 81, 256, 26);
	//layers[11].set(0.3f, 101, 3, 107, 256, 12);
	//layers[12].set(0.1f, 113, 3, 121, 256, 6);
	
	//layers[8].set(0.7f, -15, 3, 0, 256, 26); //3 = ParallaxTree, alltså, Clouds
	//layers[9].set(0.6f, 11, 3, 29, 256, 49);
	//layers[10].set(0.4f, 60, 3, 81, 256, 26);
	//layers[11].set(0.3f, 86, 3, 107, 256, 12);
	//layers[12].set(0.1f, 98, 3, 121, 256, 6);

	//layers[8].set(0.7f, -15, 3, 0, 256, 26); //3 = ParallaxTree, alltså, Clouds
	//layers[9].set(0.6f, 2, 3, 29, 256, 49);
	//layers[10].set(0.4f, 35, 3, 81, 256, 26);
	//layers[11].set(0.3f, 56, 3, 107, 256, 12);
	//layers[12].set(0.1f, 62, 3, 121, 256, 6);

	layers[8].set(0.1f, 62, 3, 121, 256, 6); //3 = ParallaxTree, alltså, Clouds
	layers[9].set(0.3f, 56, 3, 107, 256, 12);
	layers[10].set(0.4f, 35, 3, 81, 256, 26);
	layers[11].set(0.6f, 2, 3, 29, 256, 49);
	layers[12].set(0.7f, -15, 3, 0, 256, 26); 
}

void Graphics::Draw(LPDIRECT3DTEXTURE9 &theImage, int posX, int posY, bool isTransparent, float theScaleX, float theScaleY)
{
	LPD3DXSPRITE d3dSprite = NULL;
	D3DXCreateSprite(d3dDevice, &d3dSprite);
	
	//Fixar transformerings matrisen
	D3DXMATRIX theTransform;
	D3DXVECTOR3 scaleing(theScaleX, theScaleY, 1.0f);
	D3DXMatrixTransformation(&theTransform, 0, 0, &scaleing, 0, 0, 0);

	//Transformerar
	d3dSprite->SetTransform(&theTransform);

	//Koordinaterna för bildens position
	D3DXVECTOR3 thePosition(posX, posY, 0);

	d3dDevice->BeginScene(); // talar om för enheten att vi ska rita en ”frame”
	
	if(isTransparent)
		d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	else
		d3dSprite->Begin(0); // finns ett antal flaggor att skicka hit, se SDK

	// alla sprite->Draw-anrop
	d3dSprite->Draw(
		theImage, // bilden (texturen) som ska ritas ut
		0, // den rektangel som ska ritas ut från bilden (0 innebär hela bilden)
		0, // mittenpositionen i bilden, kommer till användning vid rotering senare
		&thePosition, // en pekare till en vektor som definierar var bilden ska ritas ut.
		D3DCOLOR_XRGB(255,255,255)); // en färg som multipliceras med varje pixel i bilden

	d3dSprite->End();
	d3dDevice->EndScene();
	
	//Rensar
	if(d3dSprite)
	{
		d3dSprite->Release();
		d3dSprite = NULL;
	}
}

void Graphics::drawParallaxLayers(bool isPaused)
{
	static int cloudX = 0;
	static int posX = 0;
	if(!isPaused)
	{
		cloudX--;
		posX--;
	}

	this->Draw(TexCon.ParallaxOne, 0, 0, false, 1.6f, 1.9f);

	for(int i = 0; i < 8/*13 - 5*/; i++)
	{
		this->setUpDrawingParallaxLayer(&layers[i], posX);
	}

	for(int i = 8; i < 13; i++)
	{
		this->setUpDrawingParallaxLayer(&layers[i], posX, cloudX / 2);
	}
}

void Graphics::setUpDrawingParallaxLayer(ParallaxLayer *theLayer, int xpos, int offset )
{	
	// Calc parallax position
	int x = (int)(theLayer->parallax * (float)(xpos+offset));  
	if(x < 0)	// neg -> pos
	{
		x *= -1;	// invert sign, because % only works with positive integers
		x = theLayer->width - (x % theLayer->width);
	}
	else
	{
		x %= theLayer->width;
	}

	x -= theLayer->width;	// move one back

	//Draw the layer!
	for(int i = 0; i < 3; i++)
	{
		// Calc rects
		RECT srcRect = { 0, theLayer->surfaceYOffset, theLayer->width, theLayer->surfaceYOffset + theLayer->height };

		// Blit!
		blittingParallaxLayer(theLayer->surface, &srcRect, x, theLayer->drawYOffset);

		// Move...
		x += theLayer->width;
	}
}

void Graphics::blittingParallaxLayer(int index, RECT* sourceRectangle, int xpos, int ypos)
{
	LPDIRECT3DTEXTURE9 *theImage;
	switch(index)
	{
	case 2:
		theImage = &TexCon.ParallaxTwo;
		break;
	case 3:
		theImage = &TexCon.ParallaxThree;
		break;
	}

	LPD3DXSPRITE d3dSprite = NULL;
	D3DXCreateSprite(d3dDevice, &d3dSprite);
	
	//Fixar transformerings matrisen
	D3DXMATRIX theTransform;

	// scale all sprites to double size, very ad-hoc and can be solved in better ways..
	D3DXVECTOR3 scaleing(1.7f, 2.5f, 1.0f);
	D3DXMatrixTransformation(&theTransform, 0, 0, &scaleing, 0, 0, 0);

	//Transformerar
	d3dSprite->SetTransform(&theTransform);

	//Koordinaterna för bildens position
	D3DXVECTOR3 thePosition(float(xpos), float(ypos), 0);

	d3dDevice->BeginScene(); // talar om för enheten att vi ska rita en ”frame”
	d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// alla sprite->Draw-anrop
	d3dSprite->Draw(
		*theImage, // bilden (texturen) som ska ritas ut
		sourceRectangle, // den rektangel som ska ritas ut från bilden (0 innebär hela bilden)
		0, // mittenpositionen i bilden, kommer till användning vid rotering senare
		&thePosition, // en pekare till en vektor som definierar var bilden ska ritas ut.
		D3DCOLOR_XRGB(255,255,255)); // en färg som multipliceras med varje pixel i bilden

	d3dSprite->End();
	d3dDevice->EndScene();
	
	//Rensar
	if(d3dSprite)
	{
		d3dSprite->Release();
		d3dSprite = NULL;
	}
}

void Graphics::WriteText(const char* theText, int xW, int yH, int fSize, ColorRGB theColor)
{
	HDC hdc = GetDC(theHWND); // hämta skärmens DC
	int logicalHeight = -MulDiv(fSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	
	LPD3DXFONT d3dFont = NULL;

	HRESULT hr = D3DXCreateFont(
		d3dDevice,
		logicalHeight,
		0, // bredden för fonten, eller 0 för att låta Windows bestämma
		0, // ”vikt”, till exempel fet stil
		1, // mip-nivåer
		0, // 1 = kursiv stil
		ANSI_CHARSET,
		OUT_TT_ONLY_PRECIS, // använd endast true-type fonter
		0, // ”kvalité”, används ej för true-type
		0, // ?
		"Times New Roman", // fontnamnet
		&d3dFont);
	
	if(FAILED(hr))
	{
		MessageBox(
			theHWND,
			DXGetErrorDescription(hr),
			DXGetErrorString(hr),
			MB_OK | MB_ICONERROR);
		OutputDebugString("ERROR WITH FONT");
		if(d3dFont)
		{
			d3dFont->Release();
			d3dFont = NULL;
		}
		return;
	}

	RECT rectangle = { xW, yH, 0, 0 };

	d3dFont->DrawText(
		0, // här kan ett spriteobjekt skickas in, men det är inte nödvändigt
		theText,
		-1, // antalet bokstäver, -1 så måste strängen vara 0-terminerad
		&rectangle,
		DT_NOCLIP, // klipp inte texten mot rektangeln
		D3DCOLOR_XRGB(theColor.r, theColor.g, theColor.b)); // färg på texten
	
	if(d3dFont)
	{
		d3dFont->Release();
		d3dFont = NULL;
	}
}

void Graphics::ReleaseD3DStuff()
{
	if(d3d)
	{
		d3d->Release();
		d3d = NULL;
	}
	if(d3dDevice)
	{
		d3dDevice->Release();
		d3dDevice = NULL;
	}
	if(TexCon.Title)
	{
		TexCon.Title->Release();
		TexCon.Title = NULL;
	}
	if(TexCon.Paused)
	{
		TexCon.Paused->Release();
		TexCon.Paused = NULL;
	}
	if(TexCon.Win)
	{
		TexCon.Win->Release();
		TexCon.Win = NULL;
	}
	if(TexCon.ParallaxOne)
	{
		TexCon.ParallaxOne->Release();
		TexCon.ParallaxOne = NULL;
	}
	if(TexCon.ParallaxTwo)
	{
		TexCon.ParallaxTwo->Release();
		TexCon.ParallaxTwo = NULL;
	}
	if(TexCon.ParallaxThree)
	{
		TexCon.ParallaxThree->Release();
		TexCon.ParallaxThree = NULL;
	}
	for(int i = 0; i < 11; i++)
	{
		if(TexCon.MemimSprites[i] && i < 11)
		{
			TexCon.MemimSprites[i]->Release();
			TexCon.MemimSprites[i] = NULL;
		}
		if(TexCon.PentarouSprites[i] && i < 6)
		{
			TexCon.PentarouSprites[i]->Release();
			TexCon.PentarouSprites[i] = NULL;
		}
		if(TexCon.KidDraculaSprites[i] && i < 9)
		{
			TexCon.KidDraculaSprites[i]->Release();
			TexCon.KidDraculaSprites[i] = NULL;
		}
		if(i < 2)
		{
			if(TexCon.FriendlyBullets[i])
			{
				TexCon.FriendlyBullets[i]->Release();
				TexCon.FriendlyBullets[i] = NULL;
			}
			if(TexCon.EnemyBullets[i])
			{
				TexCon.EnemyBullets[i]->Release();
				TexCon.EnemyBullets[i] = NULL;
			}
		}
	}
}