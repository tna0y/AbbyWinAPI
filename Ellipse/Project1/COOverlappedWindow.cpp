#include "COOverlappedWindow.h"


#define _USE_MATH_DEFINES
#include <math.h>

bool COverlappedWindow::resize = false;

HDC COverlappedWindow::bltHDC = NULL;

RECT COverlappedWindow::ellipseDefault = { 0, 0, 400, 250 };

bool COverlappedWindow::RegisterTheClass() {
	WNDCLASS wc = {};

	wc.lpfnWndProc = COverlappedWindow::windowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "COverlappedWindow";

	RegisterClass(&wc);
	return 0;
}


bool COverlappedWindow::Create() { 

	resize = false;
	// Пример из MSDN
	handle = CreateWindowEx(
		0,                              // Optional window styles.
		"COverlappedWindow",                     // Window class
		"YAN_Ellips",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		this        // Additional application data
	);
	return (handle != NULL);
}

void COverlappedWindow::Show(int cmdShow) {
	SetTimer(handle, 1, 20, NULL);
	ShowWindow(handle, cmdShow);
}

LRESULT __stdcall  COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {

	COverlappedWindow *window = (COverlappedWindow*)lParam;

	switch (message)
	{
	case WM_CREATE:
		window->OnCreate();
		return 0;
	case WM_QUIT:
		window->OnQuit();
		return 0;
	case WM_DESTROY:
		window->OnDestroy();
		return 0;

	case WM_PAINT:
		OnPaint(handle);
		return 0;
	case WM_TIMER:
		OnTimer(handle);
		return 0;
	case WM_SIZE:
		OnSize(handle);
		return 0;
	}
	return DefWindowProc(handle, message, wParam, lParam);
}


void COverlappedWindow::OnCreate() {
	bltHDC = CreateCompatibleDC(GetDC(handle));
	HBITMAP memBM = CreateCompatibleBitmap(bltHDC, ellipseDefault.right, ellipseDefault.bottom);
	SelectObject(bltHDC, memBM);
	FillRect(bltHDC, &ellipseDefault, (HBRUSH)(COLOR_WINDOWFRAME + 1));
	Ellipse(bltHDC, 5, 5, ellipseDefault.right-5, ellipseDefault.bottom-5);
}

void COverlappedWindow::OnQuit() {
	KillTimer(handle, 0xdeadbeef);
	DeleteDC(bltHDC);
	DestroyWindow(handle);
}
void COverlappedWindow::OnDestroy() {
	PostQuitMessage(0);
}

void COverlappedWindow::OnSize(HWND handle) {
	resize = true;

}

void COverlappedWindow::OnTimer(HWND handle) {
	RECT rect;
	const int d = 3;
	static float direction = -1;
	static LPRECT ellipse = NULL;
	if (!ellipse) {
		ellipse = new RECT();
		ellipse->left = 0;
		ellipse->top = 0;
		ellipse->bottom = ellipseDefault.bottom;
		ellipse->right = ellipseDefault.right;

	}

	GetClientRect(handle, &rect); // отталкиваемся от границ
	if (ellipse->right > rect.right || ellipse->left < rect.left) {
		direction = M_PI - direction;
	}
	else if (ellipse->top < rect.top || ellipse->bottom > rect.bottom) {
		direction = -direction;
	}
	// геометрия
	float dx, dy;
	dx = d * cos(direction);
	dy = d * sin(direction);
	ellipse->bottom -= (LONG)dy;
	ellipse->top -= (LONG)dy;
	ellipse->left += (LONG)dx;
	ellipse->right += (LONG)dx;

	// вызываем перерисовку
	InvalidateRect(handle, ellipse, false);
}

void COverlappedWindow::OnPaint(HWND handle) {

	RECT ellipse;
	static bool timer_hit = false;
	if (!timer_hit || resize) { // Рисование фона в начале или пр ресайзе окна
		timer_hit = true;
		resize = false;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(handle, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
		EndPaint(handle, &ps);
	}

	if (GetUpdateRect(handle, &ellipse, true)) { // прорисовка эллипса
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(handle, &ps);
		BitBlt(hdc, ellipse.left, ellipse.top, ellipse.right - ellipse.left, ellipse.bottom - ellipse.top, bltHDC, 0, 0, SRCCOPY);
		EndPaint(handle, &ps);
	}
}