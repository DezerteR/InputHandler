#include <GLFW/glfw3.h>
#include "InputHandler.hpp"
#include "InputHandlerUtils.hpp"
#include "InputContextHandler.hpp"
#include "StringToKeyMapping.hpp"

void InputHandler::execute(int k, int a, int m){
    if(a != GLFW_PRESS and a != GLFW_RELEASE) return; /// GLFW_REPEAT is handled internally in funcion refresh as default behaviour for repeating keys sucks

    if(a == GLFW_PRESS){
        currentlyPressedKeys.push_back(k);
    }
    else if(a == GLFW_RELEASE){
        currentlyPressedKeys.remove(k);
    }

    currentModifierKey = m;
    active->execute(k, a, m);
}

void InputHandler::setBinding(const std::string& combination){
    auto funcAndKeys = splitToFunctionAndKeys(combination);
    configuredActions.emplace(funcAndKeys.first, funcAndKeys.second);
}

void InputHandler::refresh(){
    for(auto &it : currentlyPressedKeys){
        active->execute(it, 2, currentModifierKey);
    }
}
void InputHandler::scrollCallback(double dx, double dy){
    if(dy > 0) execute(SCROLL_UP, GLFW_PRESS, currentModifierKey);
    if(dy < 0) execute(SCROLL_DOWN, GLFW_PRESS, currentModifierKey);
}
void InputHandler::keyCallback(int key, int action, int mods){
    execute(key, action, mods);
}
void InputHandler::mouseButtonCallback(int button, int action, int mods){
    switch(button){
        case GLFW_MOUSE_BUTTON_LEFT: { button = LMB; break; }
        case GLFW_MOUSE_BUTTON_RIGHT: { button = RMB; break; }
        case GLFW_MOUSE_BUTTON_MIDDLE: { button = MMB; break; }
    }
    execute(button, action, mods);
}
void InputHandler::mousePosition(float x, float y){
    active->execute(MousePosition, x, y);
}
void InputHandler::mouseMovement(float x, float y){
    active->execute(MouseMove, x, y);
}
void InputHandler::joyPadDispatch(){}
