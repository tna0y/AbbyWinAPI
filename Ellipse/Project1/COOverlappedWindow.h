#include <Windows.h>

class COverlappedWindow {
public:

	
	COverlappedWindow(HINSTANCE inst) : hInstance(inst) {};
	~COverlappedWindow() {};
	
	// Зарегистрировать класс окна
	bool RegisterTheClass();

	// Создать экземпляр окна
	bool Create();
	
	// Показать окно
	void Show(int cmdShow);
	

protected:

	void OnDestroy();
	void OnCreate();
	static void OnNCCreate(const HWND handle);
	static void OnTimer(HWND handle);
	static void OnPaint(HWND handle);
	static void OnSize(HWND handle);
	void OnQuit();
private:
	HWND handle; // хэндл окна
	HINSTANCE hInstance;
	static RECT ellipseDefault;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static bool resize;
	static HDC bltHDC;
};
