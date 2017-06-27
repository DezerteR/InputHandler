#pragma once
#include <GLFW/glfw3.h>
#include <map>

const int LMB = 501;
const int MMB = 502;
const int RMB = 503;
const int SCROLL_UP = 504;
const int SCROLL_DOWN = 505;
const int MousePosition = 507;
const int MouseMove = 508;
const int HOLD_KEY = 666;
const int LT = 666;
const int RT = 666;

/// zrobic z tego tablicę
const std::map<std::string, int> stringToKey = {
    { "space", GLFW_KEY_SPACE },
    { "esc", GLFW_KEY_ESCAPE },
    { "enter", GLFW_KEY_ENTER },
    { "tab", GLFW_KEY_TAB },
    { "backspace", GLFW_KEY_BACKSPACE },
    { "insert", GLFW_KEY_INSERT },
    { "delete", GLFW_KEY_DELETE },
    { "right", GLFW_KEY_RIGHT },
    { "left", GLFW_KEY_LEFT },
    { "down", GLFW_KEY_DOWN },
    { "up", GLFW_KEY_UP },
    { "pageUp", GLFW_KEY_PAGE_UP },
    { "pageDown", GLFW_KEY_PAGE_DOWN },
    { "home", GLFW_KEY_HOME },
    { "end", GLFW_KEY_END },
    { "capsLock", GLFW_KEY_CAPS_LOCK },
    { "scrollLock", GLFW_KEY_SCROLL_LOCK },
    { "printScreen", GLFW_KEY_PRINT_SCREEN },
    { "pause", GLFW_KEY_PAUSE },
    { "f1", GLFW_KEY_F1 },
    { "f2", GLFW_KEY_F2 },
    { "f3", GLFW_KEY_F3 },
    { "f4", GLFW_KEY_F4 },
    { "f5", GLFW_KEY_F5 },
    { "f6", GLFW_KEY_F6 },
    { "f7", GLFW_KEY_F7 },
    { "f8", GLFW_KEY_F8 },
    { "f9", GLFW_KEY_F9 },
    { "f10", GLFW_KEY_F10 },
    { "f11", GLFW_KEY_F11 },
    { "f12", GLFW_KEY_F12 },
    { "menu", GLFW_KEY_MENU },
    { "shift", GLFW_KEY_LEFT_SHIFT },
    { "ctrl", GLFW_KEY_LEFT_CONTROL },
    { "alt", GLFW_KEY_LEFT_ALT },
    { "super", GLFW_KEY_LEFT_SUPER },
    { "hold", HOLD_KEY},
    { "scrollUp", SCROLL_UP},
    { "scrollDown", SCROLL_DOWN},
    { "LMB", LMB},
    { "MMB", MMB},
    { "RMB", RMB},
    { "minus", int('-')},
    { "LT", LT},
    { "RT", RT},
    { "MousePosition", MousePosition},
    { "MouseMove", MouseMove},
};
