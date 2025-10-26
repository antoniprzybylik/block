#pragma once

#include <windows.h>

class Bitmap
{
	
	protected:
		
		// Zmienne wewnêtrzne
		HBITMAP m_hBitmap;
		int m_iWidth, m_iHeight;

		// Metody pomocnicze
		void Free()
		{
		
			if(m_hBitmap)
			{
				
				DeleteObject( m_hBitmap );
				
			}
			
		}

	public:
		
		// Konstruktory i destruktor
		Bitmap();
		Bitmap(HDC hDC, LPCTSTR szFileName);
		Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance);
		Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor);
		
		virtual ~Bitmap();

		// Metody g³ówne
		BOOL Create(HDC hDC, LPCTSTR szFileName);
		BOOL Create(HDC hDC, UINT uiResID, HINSTANCE hInstance);
		BOOL Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor);
		
		void Draw(HDC hDC, int x, int y, BOOL bTrans, COLORREF crTransColor);
		void DrawPart(HDC hDC, int x, int y, int xPart, int yPart, int wPart, int hPart, BOOL bTrans, COLORREF crTransColor);
		
		int GetWidth()
		{ 
		
			return m_iWidth;
		
		}
		
		int GetHeight()
		{
		
			return m_iHeight;
		
		}

};

Bitmap::~Bitmap()
{
	
	Free();
	
}

Bitmap::Bitmap()
{
	
	Free();
	
}

Bitmap::Bitmap(HDC hDC, LPCTSTR szFileName)
{
	
	Create(hDC, szFileName);
	
}

Bitmap::Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance)
{
	
	Create(hDC, uiResID, hInstance);
	
}

Bitmap::Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor = RGB(0, 0, 0))
{
	
	Create(hDC, iWidth, iHeight, crColor);
	
}

Bitmap::Create(HDC hDC, LPCTSTR szFileName)
{
	
	Free();
	
	m_hBitmap =( HBITMAP ) LoadImage( NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	
	if(m_hBitmap)
	{
		
		BITMAP bmInfo;
    	GetObject( m_hBitmap, sizeof( bmInfo ), & bmInfo );
		
		m_iWidth=bmInfo.bmWidth;
		m_iHeight=bmInfo.bmHeight;
		
		return TRUE;
		
	}
	
	return FALSE;
	
}


BOOL Bitmap::Create(HDC hDC, UINT uiResID, HINSTANCE hInstance)
{
	
	Free();
	
	m_hBitmap =( HBITMAP ) LoadBitmap(hInstance, MAKEINTRESOURCE(uiResID));
	
	if(m_hBitmap)
	{
		
		BITMAP bmInfo;
    	GetObject( m_hBitmap, sizeof( bmInfo ), & bmInfo );
		
		m_iWidth=bmInfo.bmWidth;
		m_iHeight=bmInfo.bmHeight;
		
		return TRUE;
		
	}
	
	return FALSE;
	
}

BOOL Bitmap::Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
{
	
	Free();
	
	m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);
	
	HDC hDCn = CreateCompatibleDC( hDC );
	SelectObject( hDCn, m_hBitmap );

	HBRUSH Pedzel, Pudelko;
	Pedzel = CreateSolidBrush( crColor );
	Pudelko =( HBRUSH ) SelectObject( hDCn, Pedzel );
	Rectangle( hDCn, -1, -1, iWidth+2, iHeight+2 );
	
	SelectObject( hDCn, Pudelko );
	DeleteObject( Pedzel );
	DeleteDC( hDCn );
	
	if(m_hBitmap)
	{
		
		BITMAP bmInfo;
    	GetObject( m_hBitmap, sizeof( bmInfo ), & bmInfo );
		
		m_iWidth=bmInfo.bmWidth;
		m_iHeight=bmInfo.bmHeight;
		
		return TRUE;
		
	}
	
	return FALSE;
	
}

void Bitmap::Draw(HDC hDC, int x, int y, BOOL bTrans = FALSE, COLORREF crTransColor = 0xFFFFFF)
{

    DrawPart(hDC, x, y, 0, 0, GetWidth(), GetHeight(), bTrans, crTransColor);
	
}

void Bitmap::DrawPart(HDC hDC, int x, int y, int xPart, int yPart, int wPart, int hPart, BOOL bTrans = FALSE, COLORREF crTransColor = 0xFFFFFF)
{
	
	if (m_hBitmap != NULL)
	{
		
		// tworzymy kontekst na bitmapê
		HDC hMemDC = CreateCompatibleDC(hDC);

		// wybieramy bitmapê
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

		// Rysujemy bitmapê
		if (bTrans)
		{
			
			TransparentBlt(hDC, x, y, wPart, hPart, hMemDC, xPart, yPart, wPart, hPart, crTransColor);
		
		}
		else
		{
			
			BitBlt(hDC, x, y, wPart, hPart, hMemDC, xPart, yPart, SRCCOPY);
		
		}
		
		// Usuwamy kontekst urz¹dzenia
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);

	}

}


