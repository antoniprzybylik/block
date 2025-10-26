#pragma once

#include "Sprite.hpp"
#include "global_variables.hpp"

class TileSprite : public Sprite
{
	
	public:
		
		int x;
		int y;
		int index;
		
		LPCTSTR BlockName;
		int BitmapID;
		int BlockType;
		int FramesCount;
		int Pass;
		
		TileSprite(Bitmap* pBitmap);
		
		void translate(int, int);
		void animate();
		
		virtual SPRITEACTION Update() override
		{
			
			animate();
			
			UpdateFrame();
	
			POINT ptNewPosition, ptSpriteSize, ptBoundsSize;
			ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
			ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
			ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
			ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
			ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
			ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;
			
			SetPosition(ptNewPosition);
			
			return SA_NONE;
			
		}
	
};

void TileSprite::translate(int tx, int ty)
{
	
	x+=tx;
	y+=ty;
	
	if( x<0 || !(x<MAP_WIDTH) )
	{
		
		index=0;
		
	}
	else
	{
		
		if( y<0 || !(y<MAP_HEIGHT) )
		{
			
			index=0;
			BitmapID=0;
			
		}
		else
		{
			
			index=y*MAP_WIDTH+x;
			
		}
		
	}
	
}

void TileSprite::animate()
{
	
	while( x-SCROLL_X < SCREEN_X/-2 )
	{
		
		translate( SCREEN_X+1, 0 );
		
	}
	
	while( x-SCROLL_X > SCREEN_X/2 )
	{
		
		translate( -1-SCREEN_X, 0 );
		
	}
	
	while( y-SCROLL_Y < SCREEN_Y/-2 )
	{
		
		translate( 0, SCREEN_Y+1 );
		
	}
	
	while( y-SCROLL_Y > SCREEN_Y/2 )
	{
		
		translate( 0, -1-SCREEN_Y );
		
	}
	
	if( index>0 )
	{
		
		BitmapID=MAP[index];
		
	}
	else
	{
		
		BitmapID=0;
		
	}
	
	if( BitmapID>0 )
	{
		
		int RelShiftX=TILE_DATA[ BitmapID ].second.first;
		int RelShiftY=TILE_DATA[ BitmapID ].second.second;
		
		SetPosition( int((x-SCROLL_X)*TILE_SIZE)+SHIFT_X-TILE_SIZE/2+RelShiftX, int((y-SCROLL_Y)*TILE_SIZE)+SHIFT_Y-TILE_SIZE/2+RelShiftY );
		
		RequestFrame(BitmapID);
		
		SetHidden(FALSE);
		
	}
	else
	{
		
		SetHidden(TRUE);
		
	}
	
}

TileSprite::TileSprite(Bitmap* pBitmap) : Sprite(pBitmap)
{
	
	// Inicjalizujemy zmienne
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds, 0, 0, 1200, 675);
	m_baBoundsAction = BA_NONE;
	m_bHidden = FALSE;
	m_iNumFrames = 1;
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	m_bDying = FALSE;
	m_bOneCycle = FALSE;
	m_bRequest = FALSE;
	m_bRequested = FALSE;
	m_iReuestedFrame=-1;
	CollisionReduce=0;
	
	CalcCollisionRect();
	
}
