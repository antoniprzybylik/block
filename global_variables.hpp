#pragma once

#define gravity(position) (position-0.02)*0.975

using tile_data=pair< int, pair< int, int> >;

const double Yvel=0.4;
const double Xvel=5.0;

HINSTANCE _hInstance;
Tengine* _pGame;
Bitmap* _pPlayerBitmap;
Bitmap* _pEnemyBitmap;
Bitmap* _pBackgroundBitmap;
Bitmap* _pTilesBitmap;
Background* _pBackground;
Sprite* _pPlayer;

int *MAP;
tile_data *TILE_DATA;
double MAP_WIDTH;
double MAP_HEIGHT;
double SCROLL_X;
double SCROLL_Y;
double TILE_SIZE;
double SCREEN_X;
double SCREEN_Y;

int Hitpoints;
double PLAYER_WIDTH;
double PLAYER_HEIGHT;
double PLAYER_X;
double PLAYER_Y;
double SX;
double SY;
double SHIFT_X;
double SHIFT_Y;

int ceilling(double a)
{
	
	if(a!=int(a))
	{
		
		return a+1;
		
	}
	
	return a;
	
}
