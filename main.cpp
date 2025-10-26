#include "main_header.hpp"

vector<TileSprite*> m_vTileSprites;

void DrawTiles(HDC hDC)
{
	
	sort( m_vTileSprites.begin(), m_vTileSprites.end(), [](TileSprite* a, TileSprite* b){ return a->BitmapID < b->BitmapID; } );
	
	for(int i=0;i<m_vTileSprites.size();i++)
	{
		
		if( m_vTileSprites[i]!=NULL )
		{
			
			m_vTileSprites[i]->Draw(hDC);
			
		}
		
	}
	
}

void UpdateTiles()
{
	
	for(int i=0;i<m_vTileSprites.size();i++)
	{
		
		if( m_vTileSprites[i]!=NULL )
		{
			
			m_vTileSprites[i]->Update();
			
		}
		
	}
	
}

void CleanupTiles()
{
	
	for(int i=0;i<m_vTileSprites.size();i++)
	{
		
		if( m_vTileSprites[i]!=NULL )
		{
			
			m_vTileSprites.erase(m_vTileSprites.begin()+i);
			
		}
		
	}
	
}

int *CheckTile(int x, int y)
{
	
	int TileIndex, TileData, TileAbove;
	
	TileIndex=int(y)*MAP_WIDTH+int(x);
	
	if( ( x<0 ) || ( x>=MAP_WIDTH ) )
	{
		
		TileData=0;
		
	}
	else
	{
		
		if( ( y<0 ) || ( y>=MAP_HEIGHT ) )
		{
			
			TileData=0;
			
		}
		else
		{
			
			TileData=MAP[TileIndex];
		
			if( TileData>0 )
			{
				
				TileData=TILE_DATA[TileData].first;
				TileIndex+=MAP_WIDTH;
				TileAbove=TILE_DATA[ MAP[ TileIndex ] ].first;
			
			}
			
		}
		
	}

	int *ReturnData=new int[3];
	
	ReturnData[0]=TileData;
	ReturnData[1]=TileIndex;
	ReturnData[2]=TileAbove;
	
	return ReturnData;
	
}

int *CheckTiles(int x1, int y1, int x2, int y2)
{
	
	int *ReturnData;
	
	ReturnData=CheckTile(x1, y1);
	
	if( ReturnData[0]==0 )
	{
		
		ReturnData=CheckTile(x2, y2);
		
	}
	
	return ReturnData;
	
}

void MovePlayer()
{
	
	int *TileData;
	
	PLAYER_X+=SX;
	
	if( SX<0 )
	{
		
		TileData=CheckTiles(PLAYER_X+PLAYER_WIDTH/2, PLAYER_Y, PLAYER_X+PLAYER_WIDTH+PLAYER_WIDTH/2, PLAYER_Y-PLAYER_HEIGHT);
		
		if( TileData[0]>0 )
		{
			
			PLAYER_X=ceilling( PLAYER_X );
			PLAYER_X-=0.5-PLAYER_WIDTH/2;
			SX=0;
			
		}
		
	}
	else
	{
		
		if( SX>0 )
		{
			
			TileData=CheckTiles(PLAYER_X+PLAYER_WIDTH+PLAYER_WIDTH/2, PLAYER_Y, PLAYER_X+2*PLAYER_WIDTH+PLAYER_WIDTH/2, PLAYER_Y-PLAYER_HEIGHT);
			
			if( TileData[0]>0 )
			{
			
				PLAYER_X=int( PLAYER_X );
				PLAYER_X+=0.5-PLAYER_WIDTH/2;
				SX=0;
			
			}
			
		}
		
	}
	
	SY=gravity(SY);
	PLAYER_Y-=SY;
	
	if( SY<0 )
	{
		
		TileData=CheckTiles(PLAYER_X+PLAYER_WIDTH/2, PLAYER_Y+1, PLAYER_X+PLAYER_WIDTH+PLAYER_WIDTH/2, PLAYER_Y+1);
		
		if( TileData[0]>0 )
		{
			
			PLAYER_Y=int( PLAYER_Y );
			SY=0;
			
		}
		
	}
	else
	{
		
		TileData=CheckTiles(PLAYER_X+PLAYER_WIDTH/2, PLAYER_Y+PLAYER_HEIGHT, PLAYER_X+PLAYER_WIDTH+PLAYER_WIDTH/2, PLAYER_Y-PLAYER_HEIGHT);
		
		if( TileData[0]!=0 )
		{
			
			PLAYER_Y=int( PLAYER_Y );
			SY=0;
			
		}
		
	}
	
	delete TileData;
	
}

//-----------------------------------------------------------------
// Funkcje silnika
//-----------------------------------------------------------------

//Inicjacja gry
BOOL GameInitialize(HINSTANCE hInstance)
{

	_pGame = new Tengine(hInstance, TEXT("block"), TEXT("block"), IDI_SKELETON, IDI_SKELETON_SM, 1200, 675);

	if (_pGame == NULL)
	{

		return FALSE;

	}

	_pGame->SetFrameRate(80);

	_hInstance = hInstance;
	
	SetCameraPosition(0, 0);

	return TRUE;

}

//Ta funkcja jest wywo³ywana na pocz¹tek gry
void GameStart(HWND hWindow)
{

	srand(GetTickCount());
	
	MAP_HEIGHT=100;
	MAP_WIDTH=300;
	
	MAP=generate_map(MAP_HEIGHT, MAP_WIDTH, 2, 3, 2);
	TILE_DATA=load_tile_data();
	
	TILE_SIZE=75;
	SCROLL_X=MAP_WIDTH/2;
	SCROLL_Y=MAP_HEIGHT*0.1;
	SCREEN_X=17;
	SCREEN_Y=10;
	Hitpoints=10;
	PLAYER_X=SCROLL_X;
	PLAYER_Y=SCROLL_Y;
	SX=0;
	SY=0;
	SHIFT_X=600;
	SHIFT_Y=337;

	// £adujemy bitmapy
	HDC hDC = GetDC(hWindow);
	_pPlayerBitmap = new Bitmap(hDC, "internal\\graphic_content\\player.bmp");
	_pBackgroundBitmap = new Bitmap(hDC, 1200, 675, RGB(153, 217, 234)); //Bitmap(hDC, IDB_BACKGROUND, _hInstance);
	_pTilesBitmap = new Bitmap(hDC, "internal\\graphic_content\\blocks.bmp");
	_pEnemyBitmap = new Bitmap(hDC, "internal\\graphic_content\\enemy.bmp");
	
	PLAYER_WIDTH=_pPlayerBitmap->GetWidth()/TILE_SIZE;
	PLAYER_HEIGHT=_pPlayerBitmap->GetHeight()/TILE_SIZE;

	// Tworzymy t³o
	_pBackground = new Background(_pBackgroundBitmap);

	// Tworzymy duszki
	RECT rcBounds = { 0, 0, 1200, 675 };
	
	_pPlayer = new Sprite(_pPlayerBitmap, rcBounds, BA_NONE);
	_pPlayer->SetPosition((PLAYER_X-SCROLL_X)*TILE_SIZE+SHIFT_X-PLAYER_WIDTH*TILE_SIZE/2, (PLAYER_Y-SCROLL_Y)*TILE_SIZE+SHIFT_Y-PLAYER_HEIGHT*TILE_SIZE/2);
	_pPlayer->SetVelocity(0, 0);
	_pPlayer->SetCollisionReduction(0);
	_pGame->AddSprite(_pPlayer);
	
	TileSprite *pTileSprite;
	
	int acX=int(SCROLL_X), acY=int(SCROLL_Y);
	acX+=int(SCREEN_X/-2);
	acY+=int(SCREEN_Y/-2);
	
	int acIndex=0;
	
	for(int i=0;i<=SCREEN_Y;i++)
	{
		
		for(int i=0;i<=SCREEN_X;i++)
		{
			
			pTileSprite = new TileSprite(_pTilesBitmap);
			
			pTileSprite->SetNumFrames(11, TRUE, FALSE);
			
			pTileSprite->x=acX;
			pTileSprite->y=acY;
			pTileSprite->index=acIndex;
			pTileSprite->BitmapID=0;
			
			m_vTileSprites.push_back(pTileSprite);
			
			pTileSprite->translate(0, 0);
			
			acX++;
			acIndex++;
			
		}
		
		acX-=(SCREEN_X+1);
		acY++;
		
	}

}

//Ta funkcja jest wywo³ywana na koniec gry
void GameEnd()
{

	// Czyœcimy bitmapy
	delete _pBackgroundBitmap;
	delete _pPlayerBitmap;
	delete _pTilesBitmap;

	// Czyœcimy duszki
	_pGame->CleanupSprites();
	CleanupTiles();
	
	delete _pBackground;

	// Czyscimy silnik
	delete _pGame;

}

//Aktywacja gry
void GameActivate(HWND hWindow)
{

	

}

//Dysaktywacja gry
void GameDeactivate(HWND hWindow)
{

	

}

//Rysowanie zawartoœci okna
void GamePaint(HDC hDC, HWND hWindow)
{
	
	static int NextSecond;
	
	static int FNPS;
	static int prevFNPS=0;
	FNPS++;

	if( time(NULL)>=NextSecond )
	{
		
		NextSecond=time(NULL)+1;
		
		RECT rect;
		HDC wdc = GetWindowDC(hWindow);
		GetClientRect(hWindow, &rect);
		rect.top=0;
		rect.left=rect.right-1000;
		rect.bottom=70;
	
		string FPStext="TIME: "+to_string(time(NULL))+"     DELAY: "+to_string(_pGame->GetFrameDelay())+"     FPS: "+to_string((FNPS+prevFNPS)/2)+"     SCROLL_X: "+to_string(SCROLL_X)+"     SCROLL_Y: "+to_string(SCROLL_Y)+"     PLAYER_X: "+to_string(PLAYER_X)+"     PLAYER_Y: "+to_string(PLAYER_Y);
	
		DrawText( wdc, TEXT(FPStext.c_str()), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		DeleteDC(wdc);
		
		prevFNPS=FNPS;
		FNPS=0;
		
	}

	// Rysujemy t³o
	_pBackground->Draw(hDC);

	// Rysujemy duszki
	DrawTiles(hDC);
	_pGame->DrawSprites(hDC);

}

//Cykl gry
void GameCycle()
{

	_pBackground->Update();

	SCROLL_X=( PLAYER_X+SCROLL_X*6 )/7;
	SCROLL_Y=( PLAYER_Y+SCROLL_Y*6 )/7;

	MovePlayer();
	
	_pPlayer->SetPosition( int( ( PLAYER_X-SCROLL_X )*TILE_SIZE )+SHIFT_X-PLAYER_WIDTH*TILE_SIZE/2, int( ( PLAYER_Y-SCROLL_Y )*TILE_SIZE )+SHIFT_Y-PLAYER_HEIGHT*TILE_SIZE/2 );

	_pGame->UpdateSprites();
	UpdateTiles();

	InvalidateRect(_pGame->GetWindow(), NULL, FALSE);

}

//Obs³uga klawiatury
void HandleKeys()
{

	if( GetAsyncKeyState(VK_LEFT) < 0 )
	{
		
		SX=(0-Xvel)/TILE_SIZE;
		
	}
	else
	{
		
		if( GetAsyncKeyState(VK_RIGHT) < 0 )
		{
			
			SX=Xvel/TILE_SIZE;
			
		}
		else
		{
			
			SX=0;
			
		}
		
	}
	
	if( GetAsyncKeyState(VK_UP) < 0 )
	{
		
		int *TileData=new int[3];
		
		TileData=CheckTiles(PLAYER_X+PLAYER_WIDTH/2, PLAYER_Y+1, PLAYER_X+PLAYER_WIDTH+PLAYER_WIDTH/2, PLAYER_Y+1);
		
		if( TileData[0]>0 )
		{
			
			SY=Yvel;
			
		}
		
		delete TileData;
		
	}

}

//Klawisz myszy klikniêty
void MouseButtonDown(int x, int y, BOOL bLeft)
{



}

//Klawisz myszy zwolniony
void MouseButtonUp(int x, int y, BOOL bLeft)
{



}

//Mysz siê rusza
void MouseMove(int x, int y)
{



}

//Wykrywanie kolizji
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{

	return TRUE;

}

//Ta funkcja jest wywo³ywna kiedy duszek umiera
void SpriteDying (Sprite * pSprite)
{



}
