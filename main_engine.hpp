#pragma once

#include <windows.h>
#include <vector>
#include "Resource.hpp"
#include "Sprite.hpp"
#include "camera.hpp"

using namespace std;

//-----------------------------------------------------------------
// Funkcje globalne
//-----------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------
// Zmienne globalne
//-----------------------------------------------------------------

int tWidth=1200;
int tHeight=675;

//-----------------------------------------------------------------
// Funkcje silnika
//-----------------------------------------------------------------

BOOL GameInitialize(HINSTANCE hInstance);
void GameStart(HWND hWindow);
void GameEnd();
void GameActivate(HWND hWindow);
void GameDeactivate(HWND hWindow);
void GamePaint(HDC hDC, HWND hWindow);
void GameCycle();
void HandleKeys();
void MouseButtonDown(int x, int y, BOOL bLeft);
void MouseButtonUp(int x, int y, BOOL bLeft);
void MouseMove(int x, int y);
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee);
void SpriteDying (Sprite * pSprite);

//-----------------------------------------------------------------
// Definicje funkcji globalnych
//-----------------------------------------------------------------

class Tengine
{
	
	protected:
		
		// Zmienne wewnêtrzne
		HINSTANCE m_hInstance;
		HWND m_hWindow;
		TCHAR m_szWindowClass[32];
		TCHAR m_szTitle[32];
		WORD m_wIcon, m_wSmallIcon;
		int m_iWidth, m_iHeight;
		int m_iFrameDelay;
		BOOL m_bSleep;
		vector<Sprite*> m_vSprites;
		
	public:
		
		//zmienne zewnêtrzne
		static Tengine *m_pEngine;
		
		// Konstruktor i destruktor
		Tengine(HINSTANCE, LPCTSTR, LPCTSTR, WORD, WORD, int, int);
		virtual ~Tengine();
		
		// Metody g³ówne
		static Tengine* GetEngine()
		{
		
			return m_pEngine;
		
		}
		
		BOOL Initialize(int iCmdShow, HINSTANCE hInstance);
		LRESULT HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
		
		BOOL CheckSpriteCollision(Sprite* pTestSprite);
		
		void AddSprite(Sprite* pSprite);
		void DrawSprites(HDC hDC);
		void UpdateSprites();
		void CleanupSprites();
		Sprite* IsPointInSprite(int x, int y);
		
		// Metody dostêpu do silnika
		HINSTANCE GetInstance()
		{ 
			return m_hInstance; 
		}
		
		HWND GetWindow()
		{
			return m_hWindow;
		}
		
		void SetWindow(HWND hWindow)
		{
			m_hWindow = hWindow;
		}
		
		LPTSTR GetTitle()
		{
			return m_szTitle;
		}
		
		WORD GetIcon()
		{
			return m_wIcon;
		}
		
		WORD GetSmallIcon()
		{
			return m_wSmallIcon;
		}
		
		int GetWidth()
		{
			return m_iWidth;
		}
		
		int GetHeight()
		{
			return m_iHeight;
		}
		
		int GetFrameDelay()
		{
			return m_iFrameDelay;
		}
		
		void SetFrameRate(int iFrameRate)
		{
			m_iFrameDelay = 1000 / iFrameRate;
		}
		
		BOOL GetSleep()
		{
			return m_bSleep;
		}
		
		void SetSleep(BOOL bSleep)
		{
			m_bSleep = bSleep;
		}
	
};

Tengine * Tengine::m_pEngine = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	
	MSG msg;
	static int iTickTrigger = 0;
	int iTickCount;
	
	if( GameInitialize(hInstance) )
	{
		
		//Inicjacja silnika
		if (!Tengine::GetEngine()->Initialize(iCmdShow, hInstance))
		{
			
			return FALSE;
			
		}
		
		for(;;)
		{
			
			if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
			{
				
				// Przetwarzanie wiadomoœci
				if (msg.message == WM_QUIT)
				{
					
					break;
					
				}
				
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				
			}
			else
			{
				
				// Pêtla g³ówna
				if (!Tengine::GetEngine()->GetSleep())
				{
					
					// Sprawdzamy ticki
					iTickCount = GetTickCount();
					
					if (iTickCount > iTickTrigger)
					{
						iTickTrigger = iTickCount+Tengine::GetEngine()->GetFrameDelay();
						HandleKeys();
						GameCycle();
					}
					
				}
				
			}
			
		}
		
		//Zwracamy wiadomoœæ spowrotem do systemu
		return (int) msg.wParam;
		
	}
	
	GameEnd();

	return TRUE;
	
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	//Wszystkie wiadomoœci od windowsa wrzucamy do funkcji HandleEvent
	return Tengine::GetEngine()->HandleEvent(hWindow, msg, wParam, lParam);
	
}

//Definicja konstruktora silnika
Tengine::Tengine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle, WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight)
{
	
	// No to ustawiamy :)
	m_pEngine = this;
	m_hInstance = hInstance;
	m_hWindow = NULL;
	
	cameraX=0;
	cameraY=0;
	
	if (lstrlen(szWindowClass) > 0) //Jak klasa naszego programu to nie NULL
	{
		
		lstrcpy(m_szWindowClass, szWindowClass); //To j¹ kopiujemy
		
	}
	
	if (lstrlen(szTitle) > 0) //Jak mamy tytu³
	{
	
		lstrcpy(m_szTitle, szTitle); //to go kopiujemy
	
	}
	
	m_wIcon = wIcon; //Bitmapa du¿ej ikonki (tej na pasku zadañ)
	m_wSmallIcon = wSmallIcon; //Bitmapa ma³ej ikonki (na pasku na górze)
	m_iWidth = iWidth; //Szerokoœæ okienka
	m_iHeight = iHeight; //Wysokoœæ okienka
	m_iFrameDelay = 50; //1000/50=20 FPS domyœlnie
	m_bSleep = TRUE; //Silnik œpi...

}

//Destruktor bêdzie pusty...
Tengine::~Tengine()
{
}

//funkcja inicjuj¹ca silnik
BOOL Tengine::Initialize(int iCmdShow, HINSTANCE hInstance)
{
	
	WNDCLASSEX wndclass;

	// Parametry okna
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInstance;
	wndclass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(GetIcon()));
	wndclass.hIconSm = LoadIcon(m_hInstance, MAKEINTRESOURCE(GetSmallIcon()));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_szWindowClass;
	
	

	// Rejestrujemy klasê okna
	if ( !RegisterClassEx(&wndclass) )
	{
		
		return FALSE;
	
	}

	// 	liczymy rozmiar okna i jego pozycjê na podstawie rozmiaru gry
	int iWindowWidth=m_iWidth+GetSystemMetrics(SM_CXFIXEDFRAME)*2, iWindowHeight=m_iHeight+GetSystemMetrics(SM_CYFIXEDFRAME)*2+GetSystemMetrics(SM_CYCAPTION);
	
	if (wndclass.lpszMenuName != NULL)
	{
		
		iWindowHeight += GetSystemMetrics(SM_CYMENU);
		
	}
	
	int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2, iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;
	
	//HMENU hMenu = LoadMenu( hInstance, MAKEINTRESOURCE( MENU_SCRIPT ) );

	// 	Tworzymy okienko
	m_hWindow = CreateWindow(m_szWindowClass, m_szTitle, WS_POPUPWINDOW | WS_CAPTION , iXWindowPos, iYWindowPos, iWindowWidth, iWindowHeight, NULL, /*hMenu*/NULL, m_hInstance, NULL);
	
	if (!m_hWindow)
	{
	
		return FALSE;
	
	}

	// Pokazujemy i odœwie¿amy okno
	ShowWindow(m_hWindow, iCmdShow);
	UpdateWindow(m_hWindow);
	
	// Maksymalizacja okna
	SendMessage(m_hWindow, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	
	RECT wSize;

	GetWindowRect( m_hWindow, & wSize );
	
	tWidth = wSize.right - wSize.left;
	tHeight = wSize.bottom - wSize.top;

	return TRUE;

}

int paints=0;

//Tu przekierowywujemy wszystko z pêtli komunikatów
LRESULT Tengine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	// Sortujemy pocztê :)
	switch (msg)
	{
		
		case WM_CREATE:
			
		// Ustawiamy okno i zaczynamy grê
		SetWindow(hWindow);
		GameStart(hWindow);
		
		return 0;
		
		case WM_ACTIVATE:
			
			// Aktywujemy/Dysaktywujemy grê
			if (wParam != WA_INACTIVE)
			{
				GameActivate(hWindow);
				SetSleep(FALSE);
			}
			else
			{
				GameDeactivate(hWindow);
				SetSleep(TRUE);
			}
		
		return 0;
		
		case WM_PAINT:
			
			paints++;
			
			{
				
				PAINTSTRUCT ps;
				HDC hDC = BeginPaint(hWindow, &ps);
			
				static HBITMAP nBitmap;
			
				if(paints==1)
				{
					
					nBitmap = CreateCompatibleBitmap(hDC, m_iWidth, m_iHeight);
					
				}
		
				HDC hDCn_ = CreateCompatibleDC( hDC );
				SelectObject( hDCn_, nBitmap );
			
				// Rysujemy grê
				GamePaint(hDCn_, hWindow);
			
				BitBlt( hDC, (tWidth-m_iWidth)/2, (tHeight-m_iHeight)/2, m_iWidth, m_iHeight, hDCn_, cameraX, cameraY, SRCCOPY );
			
				DeleteDC(hDCn_);	
				EndPaint(hWindow, &ps);
		
			}
		
		return 0;

		case WM_DESTROY:
			
			// Koñczymy grê i zamykamy program
			GameEnd();
			PostQuitMessage(0);
		
		return 0;
		
		case WM_LBUTTONDOWN:
			
			// Lewy myszy klikniêty
			MouseButtonDown(LOWORD(lParam)-(tWidth-m_iWidth)/2, HIWORD(lParam)-(tHeight-m_iHeight)/2, TRUE);
			
		return 0;
		
		case WM_LBUTTONUP:
			
			// Lewy myszy wypuszczony
			MouseButtonUp(LOWORD(lParam)-(tWidth-m_iWidth)/2, HIWORD(lParam)-(tHeight-m_iHeight)/2, TRUE);
			
		return 0;
		
		case WM_RBUTTONDOWN:
			
			// Prawy myszy klikniêty
			MouseButtonDown(LOWORD(lParam)-(tWidth-m_iWidth)/2, HIWORD(lParam)-(tHeight-m_iHeight)/2, FALSE);
			
		return 0;
		
		case WM_RBUTTONUP:
			
			// Prawy myszy wypuszczony
			MouseButtonUp(LOWORD(lParam)-(tWidth-m_iWidth)/2, HIWORD(lParam)-(tHeight-m_iHeight)/2, FALSE);
		
		return 0;
		
		case WM_MOUSEMOVE:
			
			// Ruch myszy
			MouseMove(LOWORD(lParam)-(tWidth-m_iWidth)/2, HIWORD(lParam)-(tHeight-m_iHeight)/2);
			
		return 0;
		
		default:
		return DefWindowProc(hWindow, msg, wParam, lParam);
		
	}

}

BOOL Tengine::CheckSpriteCollision(Sprite* pTestSprite)
{
	
	for(int i=0;i<m_vSprites.size();i++)
	{
		
		if( m_vSprites[i]!=pTestSprite && m_vSprites[i]!=NULL )
		{
			
			if( pTestSprite->TestCollision(m_vSprites[i]) )
			{
				
				return SpriteCollision(pTestSprite, m_vSprites[i]);
				
			}
			
		}
		
	}
	
	return FALSE;
	
}

void Tengine::AddSprite(Sprite* pSprite)
{
	
	if( pSprite!=NULL )
	{
		
		m_vSprites.push_back(pSprite);
		
		sort( m_vSprites.begin(), m_vSprites.end(), [](Sprite* a, Sprite* b){ return a->GetZOrder()<b->GetZOrder(); } );
		
	}
	
}

void Tengine::DrawSprites(HDC hDC)
{
	
	for(int i=0;i<m_vSprites.size();i++)
	{
		
		if( m_vSprites[i]!=NULL )
		{
			
			m_vSprites[i]->Draw(hDC);
			
		}
		
	}
	
}

void Tengine::UpdateSprites()
{
	
	for(int i=0;i<m_vSprites.size();i++)
	{
		
		if( m_vSprites[i]!=NULL )
		{
			
			RECT& pre=m_vSprites[i]->GetPosition();
			
			if( m_vSprites[i]->Update() == SA_KILL )
			{
				
				SpriteDying(m_vSprites[i]);
				m_vSprites.erase(m_vSprites.begin()+i);
				
			}
			else
			{
				
				if( CheckSpriteCollision(m_vSprites[i]) )
				{
			
					m_vSprites[i]->SetPosition(pre);
					
				}
				
			}
			
		}
		
	}
	
}

void Tengine::CleanupSprites()
{
	
	for(int i=0;i<m_vSprites.size();i++)
	{
		
		if( m_vSprites[i]!=NULL )
		{
			
			m_vSprites.erase(m_vSprites.begin()+i);
			
		}
		
	}
	
}

Sprite* Tengine::IsPointInSprite(int x, int y)
{
	
	for(int i=m_vSprites.size()-1;i>=0;i++)
	{
		
		if( m_vSprites[i]!=NULL )
		{
			
			if( m_vSprites[i]->IsPointInside(x, y) )
			{
				
				return m_vSprites[i];
				
			}
			
		}
		
	}
	
	return NULL;
	
}
