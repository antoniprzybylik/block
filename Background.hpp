#pragma once

#include <windows.h>

class Background
{

	protected:
		
		// Zmienne wewnêtrzne
		int m_iWidth, m_iHeight;
		COLORREF m_crColor;
		Bitmap* m_pBitmap;

	public:
		
		// Konstruktory i destruktor
		Background(int iWidth, int iHeight, COLORREF crColor);
		Background(Bitmap* pBitmap);
		virtual ~Background();

		// Metody g³ówne
		virtual void Update();
		virtual void Draw(HDC hDC);

		// Metody dostêpu
		int GetWidth()
		{
		
			return m_iWidth; 
			
		}
		
		int GetHeight()
		{ 
		
			return m_iHeight; 
			
		}

};

Background::Background(int iWidth, int iHeight, COLORREF crColor)
{
	
	// Inicjalizujemy zmienne
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_crColor = crColor;
	m_pBitmap = NULL;

}

Background::Background(Bitmap* pBitmap)
{
	
	// Inicjalizujemy zmienne
	m_crColor = 0;
	m_pBitmap = pBitmap;
	m_iWidth = pBitmap->GetWidth();
	m_iHeight = pBitmap->GetHeight();

}

Background::~Background()
{
}

void Background::Update()
{
}

void Background::Draw(HDC hDC)
{
	
	// Rysujemy t³o
	if (m_pBitmap != NULL)
	{
		
		m_pBitmap->Draw(hDC, 0, 0);
	
	}
	else
	{
		
		RECT rect = { 0, 0, m_iWidth, m_iHeight };
		HBRUSH hBrush = CreateSolidBrush(m_crColor);
		FillRect(hDC, &rect, hBrush);
		DeleteObject(hBrush);
	
	}

}

