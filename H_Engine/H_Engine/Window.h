#pragma once
#include "Constant.h" // includes windows.h and defines WIN32_LEAN_AND_MEAN

#include "Input.h"
#include "graphics.h"

class Window
{
public:
	// public variables
	Input* input;
	Graphics* graphics;
public:
	// public functions
	// constructor, creates a window with the given data, also initializes graphics and input
	Window( int width, int height, const wchar_t* name, int nCmdShow, class Game* owner );
	// destructor, destroys the window
	~Window() noexcept;
	// redirects to the owner's message handler, has to be static to be used as window proc
	static LRESULT WINAPI HandleMessageSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT WINAPI HandleMessageRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	// private or utility functions
	void RegisterGameWindowClass( HINSTANCE hInstance );
	void CreateGameWindow();
private:
	// private data
	// width and height of window
	const int width;
	const int height;
	// handle to window
	HWND hwnd;
	// pointer to game / owner, used to redirect to message handler
	class Game* owner = NULL; // forward declared to prevent circular dependency wth game.h
	// handle to instance
	HINSTANCE hInstance;
};