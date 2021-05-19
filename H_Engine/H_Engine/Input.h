#ifndef INPUT_H
#define INPUT_H
#define WIN32_LEAN_AND_MEAN

class Input;

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include <XInput.h>
#include "Constant.h"
#include "gameError.h"
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif

class Input {
private:
    bool keysDown[inputNS::KEYS_ARRAY_LEN];
    bool keysPressed[inputNS::KEYS_ARRAY_LEN];
    std::string textIn;
    char charIn;
    bool newLine;
    int  mouseX, mouseY;
    int  mouseRawX, mouseRawY;
    RAWINPUTDEVICE Rid[1];
    bool mouseCaptured;
    bool mouseLButton;
    bool mouseRButton;
public:
    Input();
    virtual ~Input();
    void initialize(HWND hwnd, bool capture);
    void keyDown(WPARAM);
    void keyUp(WPARAM);
    void keyIn(WPARAM);
    bool isKeyDown(UCHAR vkey) const;
    bool wasKeyPressed(UCHAR vkey) const;
    bool anyKeyPressed() const;
    void clear(UCHAR what);
    void clearKeyPress(UCHAR vkey);
    void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }
    void clearTextIn() { textIn.clear(); }
    std::string getTextIn() { return textIn; }
    char getCharIn() { return charIn; }
    void mouseIn(LPARAM);
    void mouseRawIn(LPARAM);
    void setMouseLButton(bool b) { mouseLButton = b; }
    void setMouseRButton(bool b) { mouseRButton = b; }
    int  getMouseX()        const { return mouseX; }
    int  getMouseY()        const { return mouseY; }
    int  getMouseRawX()     const { return mouseRawX; }
    int  getMouseRawY()     const { return mouseRawY; }
    bool getMouseLButton()  const { return mouseLButton; }
    bool getMouseRButton()  const { return mouseRButton; }
};
#endif

