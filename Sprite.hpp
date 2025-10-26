#pragma once

#include <windows.h>
#include "Bitmap.hpp"
#include <algorithm>

using namespace std;

typedef WORD BOUNDSACTION;
const BOUNDSACTION BA_STOP = 0, BA_WRAP = 1, BA_BOUNCE = 2, BA_DIE = 3, BA_NONE = 4;

typedef WORD SPRITEACTION;
const SPRITEACTION SA_NONE = 0x0000L, SA_KILL = 0x0001L, SA_ADDSPRITE = 0x0002L;

class Sprite
{
	protected:
		
		// Zmienne wewnêtrzne
		Bitmap* m_pBitmap;
		RECT m_rcPosition;
		POINT m_ptVelocity;
		int m_iZOrder;
		RECT m_rcBounds;
		BOUNDSACTION m_baBoundsAction;
		BOOL m_bHidden;
		int m_iNumFrames, m_iCurFrame;
		int m_iFrameDelay, m_iFrameTrigger;
		int m_iReuestedFrame;
		RECT m_rcCollision;
		BOOL m_bDying;
		BOOL m_bOneCycle;
		BOOL m_bRequest;
		BOOL m_bRequested;
		int CollisionReduce;
		
		// Metody wewnêtrzne
		
		virtual void CalcCollisionRect();
		virtual Sprite* AddSprite();

	public:
	
		// Konstruktory i destruktor
		Sprite(Bitmap* pBitmap);
		Sprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
		Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
		virtual ~Sprite();

		// Metody g³ówne
		virtual SPRITEACTION Update();
		void Draw(HDC hDC);
		
		BOOL IsPointInside(int x, int y);
		
		void SetNumFrames (int iNumFrames, BOOL bRequest, BOOL bOneCycle);
		void SetFrameDelay(int iFrameDelay);
		void UpdateFrame();
		
		BOOL TestCollision(Sprite* pTestSprite);

		// Metody dostêpu
		
		RECT& GetPosition()
		{ 
		
			return m_rcPosition;
			
		}
		
		void SetPosition(int x, int y);
		void SetPosition(POINT ptPosition);
		
		void SetPosition(RECT& rcPosition)
		{
			
			CopyRect(&m_rcPosition, &rcPosition);
		
		}
		
		void OffsetPosition(int x, int y);
		
		POINT GetVelocity()
		{
			
			return m_ptVelocity;
		
		}
		
		void SetVelocity(int x, int y);
		void SetVelocity(POINT ptVelocity);
		
		int GetZOrder()
		{
			
			return m_iZOrder;
			
		}
		
		void SetZOrder(int iZOrder)
		{
			
			m_iZOrder = iZOrder;
		
		}
		
		void SetBounds(RECT& rcBounds)
		{
			
			CopyRect(&m_rcBounds, &rcBounds);
			
		}
		
		void SetBoundsAction(BOUNDSACTION ba)
		{
			
			m_baBoundsAction = ba;
		
		}
		
		BOOL IsHidden()
		{
		
			return m_bHidden;
		
		}
		
		void SetHidden(BOOL bHidden)
		{
			
			m_bHidden = bHidden;
		
		}
		
		int GetWidth()
		{
		
			return m_rcPosition.right-m_rcPosition.left;
			
		}
		
		int GetHeight()
		{
			
			return m_rcPosition.bottom-m_rcPosition.top;
			
		}
		
		RECT& GetCollision()
		{ 
		
			return m_rcCollision; 
			
		}
		
		void Kill()
		{ 
		
			m_bDying = TRUE;
			
		}
		
		void SetCollisionReduction(int reduction)
		{
			
			CollisionReduce=reduction;
			
		}
		
		void RequestFrameChange()
		{
			
			m_bRequested=TRUE;
			
		}
		
		void RequestFrame(int frame)
		{
			
			m_bRequested=TRUE;
			m_iReuestedFrame=frame;
			
		}

};

Sprite::Sprite(Bitmap* pBitmap)
{
	
	// Inicjalizujemy zmienne
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds, 0, 0, 1200, 675);
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;
	m_iNumFrames = 1;
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	m_bDying = FALSE;
	m_bOneCycle = FALSE;
	m_bRequest = FALSE;
	m_bRequested = FALSE;
	m_iReuestedFrame=-1;
	CollisionReduce=12;
	
	CalcCollisionRect();

}

Sprite::Sprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
	
	// Ustawiamy losow¹ pozycjê
	int iXPos = rand() % (rcBounds.right - rcBounds.left);
	int iYPos = rand() % (rcBounds.bottom - rcBounds.top);

	// Inicjalizujemy zmienne
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, iXPos, iYPos, iXPos + pBitmap->GetWidth(), iYPos + pBitmap->GetHeight());
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;
	m_iNumFrames = 1;
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	m_bDying = FALSE;
	m_bOneCycle = FALSE;
	m_bRequest = FALSE;
	m_bRequested = FALSE;
	m_iReuestedFrame=-1;
	CollisionReduce=12;
	
	CalcCollisionRect();

}

Sprite::Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
	
	// 	Inicjalizujemy zmienne
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, ptPosition.x, ptPosition.y, pBitmap->GetWidth(), pBitmap->GetHeight());
	m_ptVelocity = ptPosition;
	m_iZOrder = iZOrder;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;
	m_iNumFrames = 1;
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	m_bDying = FALSE;
	m_bOneCycle = FALSE;
	m_bRequest = FALSE;
	m_bRequested = FALSE;
	m_iReuestedFrame=-1;
	CollisionReduce=12;
	
	CalcCollisionRect();

}

Sprite::~Sprite()
{
}

SPRITEACTION Sprite::Update()
{
	
	// Sprawdzamy, czy duszek nie powinien byæ zabity
	if (m_bDying)
	{
		
		return SA_KILL;
	
	}
	
	// Odœwierzanie klatki
	UpdateFrame();
	
	// Odœwierzanie pozycji
	POINT ptNewPosition, ptSpriteSize, ptBoundsSize;
	ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
	ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
	ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;

	// Sprawdzamy krawêdzie
	
	if (m_baBoundsAction == BA_WRAP) // Akcja - Modulo
	{
	
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left)
		{
			
			ptNewPosition.x = m_rcBounds.right;
		
		}
		else
		{	
			if (ptNewPosition.x > m_rcBounds.right)
			{
				
				ptNewPosition.x = m_rcBounds.left - ptSpriteSize.x;
			
			}
			
			if ((ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top)
			{
				
				ptNewPosition.y = m_rcBounds.bottom;
			
			}
			else
			{
				
				if (ptNewPosition.y > m_rcBounds.bottom)
				{
					
					ptNewPosition.y = m_rcBounds.top - ptSpriteSize.y;
				
				}
				
			}
		
		}
	
	}
	else
	{
	
		if (m_baBoundsAction == BA_BOUNCE) // Akcja - Odbicie
		{
			
			BOOL bBounce = FALSE;
			POINT ptNewVelocity = m_ptVelocity;
			
			if (ptNewPosition.x < m_rcBounds.left)
			{
				
				bBounce = TRUE;
				ptNewPosition.x = m_rcBounds.left;
				ptNewVelocity.x = -ptNewVelocity.x;
				
			}
			else
			{
			
				if ((ptNewPosition.x + ptSpriteSize.x) > m_rcBounds.right)
				{
					
					bBounce = TRUE;
					ptNewPosition.x = m_rcBounds.right - ptSpriteSize.x;
					ptNewVelocity.x = -ptNewVelocity.x;
				
				}
			
			}
			
			if (ptNewPosition.y < m_rcBounds.top)
			{
				
				bBounce = TRUE;
				ptNewPosition.y = m_rcBounds.top;
				ptNewVelocity.y = -ptNewVelocity.y;
			
			}
			else
			{
				
				if ((ptNewPosition.y + ptSpriteSize.y) > m_rcBounds.bottom)
				{
					
					bBounce = TRUE;
					ptNewPosition.y = m_rcBounds.bottom - ptSpriteSize.y;
					ptNewVelocity.y = -ptNewVelocity.y;
				
				}
			
			}
			
			if (bBounce)
			{
			
				SetVelocity(ptNewVelocity);
			
			}
			
		}
		else
		{
			
			if(m_baBoundsAction == BA_DIE)
			{
				
				return SA_KILL;
				
			}
			else
			{
				
				if(m_baBoundsAction == BA_STOP) // Akcja - Stop (domyœlna)
				{
					
					if (ptNewPosition.x < m_rcBounds.left || ptNewPosition.x > (m_rcBounds.right - ptSpriteSize.x))
					{
					
						ptNewPosition.x = max(m_rcBounds.left, min(ptNewPosition.x, m_rcBounds.right - ptSpriteSize.x));
						SetVelocity(0, 0);
					
					}
				
					if (ptNewPosition.y < m_rcBounds.top || ptNewPosition.y > (m_rcBounds.bottom - ptSpriteSize.y))
					{
					
						ptNewPosition.y = max(m_rcBounds.top, min(ptNewPosition.y, m_rcBounds.bottom - ptSpriteSize.y));
						SetVelocity(0, 0);
					
					}
					
				}
				
			}
			
		}
	
	}
	
	SetPosition(ptNewPosition);
	
	return SA_NONE;
	
}

void Sprite::Draw(HDC hDC)
{

	// Rysyjemy duszka jeœli nie jest schowany
	if (m_pBitmap != NULL && !m_bHidden)
	{
	
		// Rysujemy klatkê
		if (m_iNumFrames == 1)
		{
			
			m_pBitmap->Draw(hDC, m_rcPosition.left, m_rcPosition.top, TRUE);
		
		}
		else
		{
			
			m_pBitmap->DrawPart(hDC, m_rcPosition.left, m_rcPosition.top, m_iCurFrame * GetWidth(), 0, GetWidth(), GetHeight(), TRUE);
		
		}
	
	}

}

void Sprite::SetVelocity(int x, int y)
{
	
	m_ptVelocity.x=x;
	m_ptVelocity.y=y;
	
}

void Sprite::SetVelocity(POINT ptVelocity)
{
	
	m_ptVelocity=ptVelocity;
	
}

void Sprite::SetPosition(int x, int y)
{
	
	SetRect(&m_rcPosition, x, y, x + m_pBitmap->GetWidth()/m_iNumFrames, y + m_pBitmap->GetHeight());
	
	CalcCollisionRect();
	
}

void Sprite::SetPosition(POINT ptPosition)
{
	
	SetRect(&m_rcPosition, ptPosition.x, ptPosition.y, ptPosition.x + m_pBitmap->GetWidth()/m_iNumFrames, ptPosition.y + m_pBitmap->GetHeight());
	
	CalcCollisionRect();
	
}

void Sprite::OffsetPosition(int x, int y)
{
	
	SetRect(&m_rcPosition, m_rcPosition.left+x, m_rcPosition.top+y, m_rcPosition.left+x + m_pBitmap->GetWidth()/m_iNumFrames, m_rcPosition.top+y + m_pBitmap->GetHeight());
	
	CalcCollisionRect();
	
}

BOOL Sprite::IsPointInside(int x, int y)
{
	
	if( x<=m_rcPosition.right && x>=m_rcPosition.left )
	{
		
		if( y>=m_rcPosition.top && y<=m_rcPosition.bottom )
		{
			
			return TRUE;
			
		}
		
	}
	
	return FALSE;
	
}

void Sprite::SetNumFrames(int iNumFrames, BOOL bRequest=FALSE, BOOL bOneCycle=FALSE)
{
	
	// ustawiamy FPS
	m_iNumFrames = iNumFrames;

	// Obliczamy pozycjê
	RECT rect = GetPosition();
	rect.right = rect.left + ((rect.right - rect.left) / iNumFrames);
	SetPosition(rect);
	CalcCollisionRect();
	
	m_bOneCycle=bOneCycle;
	m_bRequest=bRequest;

}

inline void Sprite::UpdateFrame()
{
	
	if ((m_iFrameDelay >= 0) && (--m_iFrameTrigger <= 0) && !m_bRequest)
	{
		
		// Resetujemy licznik
		m_iFrameTrigger = m_iFrameDelay;
	
		// zwiêkszamy klatkê
		if (++m_iCurFrame >= m_iNumFrames)
		{
			
			if(m_bOneCycle)
			{
				
				m_bDying=TRUE;
				
			}
			else
			{
				
				m_iCurFrame = 0;
				
			}
			
		}	
	
	}
	else
	{
		
		if( m_bRequest && m_bRequested && m_iReuestedFrame==-1 )
		{
			
			// zwiêkszamy klatkê
			if (++m_iCurFrame >= m_iNumFrames)
			{
			
				if(m_bOneCycle)
				{
				
					m_bDying=TRUE;
					
				}
				else
				{
				
					m_iCurFrame = 0;
				
				}
			
			}
			
			m_bRequested=FALSE;
			
		}
		else
		{
			
			if( m_bRequest && m_bRequested )
			{
				
				m_iCurFrame = m_iReuestedFrame;
				
				m_iReuestedFrame=-1;
				m_bRequested=FALSE;
				
			}
			
		}
		
	}

}

void Sprite::SetFrameDelay(int iFrameDelay)
{
	
	m_iFrameDelay=iFrameDelay;
	
}

inline void Sprite::CalcCollisionRect()
{
	
	CopyRect(&m_rcCollision, &m_rcPosition);
	
	if(CollisionReduce!=0)
	{
	
		int iXShrink = (m_rcPosition.left - m_rcPosition.right) / CollisionReduce;
		int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / CollisionReduce;
	
		InflateRect(&m_rcCollision, iXShrink, iYShrink);

	}

}

inline BOOL Sprite::TestCollision(Sprite* pTestSprite)
{
	
	RECT& rcTest = pTestSprite->GetCollision();

	return ( ( m_rcCollision.left <= rcTest.right && m_rcCollision.left >= rcTest.left ) || ( m_rcCollision.right <= rcTest.right && m_rcCollision.right >= rcTest.left ) )
	&& ( ( m_rcCollision.top >= rcTest.top && m_rcCollision.top <= rcTest.bottom ) || ( m_rcCollision.bottom >= rcTest.top && m_rcCollision.bottom <= rcTest.bottom ) );

}

Sprite* Sprite::AddSprite()
{
	
	return NULL;
		
}

