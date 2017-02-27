#include <map>
#include <tuple>
#include "Utils.hpp"
#include "Timer.hpp"
#include "InputHandler.hpp"

bool debug = true;
bool running = true;
int currentMods;

void scrollCallback(GLFWwindow *window, double dx, double dy){
	InputHandler::scrollCallback(dx, dy);
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
	InputHandler::keyCallback(key, action, mods);
}
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
	InputHandler::mouseButtonCallback(button, action, mods);
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
        InputHandler::refresh();

        glfwSwapBuffers(window);
    }
}

int main(){
    initWindow();

    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowCloseCallback(window, exitCallback);

    InputHandler::registerKeyCombination("esc:exit");
    InputHandler::registerKeyCombination("alt-f4:exit");
    InputHandler::registerKeyCombination("f4:exit");
    InputHandler::registerKeyCombination("f1:help");
    InputHandler::registerKeyCombination("hold-shift:stats");
    InputHandler::registerKeyCombination("shift-c:setOrigin");
    InputHandler::registerKeyCombination("ctrl-c:setOrigin");

    InputHandler::Context context("Main");
    context.setAction("setOrigin", []{log("setOrigin");});
    context.setAction("help", []{log("This is help");log("-------");});
    context.setAction("stats", []{log("Score: 12300");});
    context.setAction("exit", []{std::cout<<"bye :D"; running = false;});
    // context.emplacePressRepeatRelease("shift-c", []{}, []{}, []{});
    context.activate();

    std::string tab[4] = {"aa", "bb", "cc", "dd"};
    int it=0;

    bool tankSpecialModifier(false);
    InputHandler::Context tankC("Tank", "Main");
    tankC.setAction("exit", []{std::cout<<"bye :D"; running = false;});
    tankC.setAction("scrollUp", "1", [&it]{ it = (it+1)%4; });
    tankC.setAction("shift", "", [&tankSpecialModifier]{ tankSpecialModifier = true; log("Shift ON"); }, [&tankSpecialModifier]{ tankSpecialModifier = false; log("Shift OFF"); });
    tankC.setAction("hold-W", "Go Forward", [&it, &tankSpecialModifier]{ 
        if(tankSpecialModifier) log("5m/s");
        else log("100m/s");
     });
    tankC.setAction("LMB", "fire",  [&it, &tab]{ log(tab[it]); });
    tankC.activate();

    loop();
    tankC.deactivate();
    context.deactivate();

    return 0;
}
