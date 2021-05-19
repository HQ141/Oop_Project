#include "Window.h"
#include "game.h"
#include <exception>

Window::Window(int width, int height, const wchar_t* name, int nCmdShow, Game* owner)
    :
    width(width),
    height(height),
    owner(owner),
    hInstance(GetModuleHandle(NULL)) // returns the HINSTANCE of the calling program
{
    if (owner == NULL)
        throw std::exception("Error in creating window, owner ptr is null");

    // register a custom window class
    RegisterGameWindowClass(hInstance);
    // creat a window for the game
    CreateGameWindow();
    // display the window
    ShowWindow(hwnd, nCmdShow);

    // create and initialize Graphics instance
    graphics = new Graphics();
    graphics->initialize(hwnd, width, height, FULLSCREEN);
    // create and initialize Input instance
    input = new Input();
    input->initialize(hwnd, false);
}

Window::~Window() noexcept
{
    // delete graphics and input before ending
    SAFE_DELETE(graphics);
    SAFE_DELETE(input);
    // destroy window
    DestroyWindow( hwnd );
    UnregisterClass(L"Window", hInstance);
}

LRESULT WINAPI Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // check if the window was just created
    if (msg == WM_CREATE) {
        // extract the 'this' ptr sent in CreateGameWindow
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* pWnd = reinterpret_cast<Window*>(pCreate->lpCreateParams);

        // store pointer to window in userdata
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // change the window proc fom HandleMessageSetup to HandleMessageRedirect
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::HandleMessageRedirect));
        // call game's message handler
        pWnd->owner->messageHandler(hWnd, msg, wParam, lParam);
    }
    // if any other message is recieved before WM_CREATE, it'll be sent to DefWndProc
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMessageRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // get pointer to window previously stored into user data
    Window* pWnd = reinterpret_cast<Window*>( GetWindowLongPtr(hWnd, GWLP_USERDATA) );
    // use it to call the message handler of game
    return pWnd->owner->messageHandler(hWnd, msg, wParam, lParam);
}

void Window::RegisterGameWindowClass( HINSTANCE hInstance )
{
    WNDCLASSEX wcx;
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = HandleMessageSetup;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = NULL;
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.lpszMenuName = NULL;
    wcx.lpszClassName = L"Window";
    wcx.hIconSm = NULL;
    if (RegisterClassEx(&wcx) == 0)    // if error
        throw std::exception("Failed to register Window Class");
}

void Window::CreateGameWindow()
{
    // define the window style
    UINT style;
    if (FULLSCREEN)
        style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
    else
        style = WS_OVERLAPPEDWINDOW;
    
    // create window
    hwnd = CreateWindow(L"Window", L"game", 
        style, CW_USEDEFAULT, CW_USEDEFAULT, 
        GAME_WIDTH, GAME_HEIGHT, (HWND)NULL, 
        (HMENU)NULL, hInstance, (LPVOID)this // this will be used to redirect messages to a different wndproc
    );
    
    // check if window creation was successful
    if (!hwnd) {
        throw std::exception("Failed to create Window");
    }
    
    // resize window if not fullscreen... i think
    if (!FULLSCREEN) {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        MoveWindow(hwnd, 0, 0, GAME_WIDTH + (GAME_WIDTH - clientRect.right), GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), TRUE);
    }
}
