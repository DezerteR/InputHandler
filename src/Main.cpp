#include <map>
#include <tuple>
#include "Utils.hpp"
#include "Timer.hpp"
#include "InputHandler.hpp"

bool debug = true;
bool running = true;
int currentMods;

void scrollCallback(GLFWwindow *window, double dx, double dy){
    if(debug) log(__FUNCTION__, "dx:", dx, "dy:", dy);
    if(dy > 0) InputHandler::execute(S_UP, 0, currentMods);
    if(dy < 0) InputHandler::execute(S_DOWN, 0, currentMods);
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(debug) log(__FUNCTION__, "key:", key, "action:", action, "mods:", mods);
    currentMods = mods;
    InputHandler::execute(key, action, mods);
}
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    if(debug) log(__FUNCTION__, "button:", button, "action:", action, "mods:", mods);

    currentMods = mods;
    InputHandler::execute(button, action, mods);
}
void exitCallback(GLFWwindow *window){
    running = false;
    log("[ Exit ]");
}

void loop(){
    Timer<u32, 1000, 1> msecLoopDeltaTime;
    while(running){
        auto dt = msecLoopDeltaTime();

        glfwPollEvents();

        refresh();

        glfwSwapBuffers(window);
    }
}

int main(){
    initWindow();
    InputHandler inputHandler;

    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowCloseCallback(window, exitCallback);

    InputHandler::registerKeyCombination("esc: exit");
    InputHandler::registerKeyCombination("alt-f4: exit"); /// multimap?

    inputHandler.emplace(GLFW_KEY_ESCAPE, GLFW_PRESS, 0, "Exit-Esc", []{
        log("Exit");
        running = false;
    });
    inputHandler.emplace(GLFW_KEY_F4, GLFW_PRESS, GLFW_MOD_ALT, "Exit-Alt-F4", []{
        log("Exit");
        running = false;
    });

    inputHandler.emplace(GLFW_KEY_LEFT_ALT, GLFW_PRESS, GLFW_MOD_ALT, "GLFW_KEY_LEFT_ALT", []{
        log("GLFW_KEY_LEFT_ALT pressed");
    });
    inputHandler.emplace('C', GLFW_PRESS, GLFW_MOD_ALT, "GLFW_MOD_ALT", []{
        log("GLFW_MOD_ALT pressed");
    });

    loop();

    return 0;
}