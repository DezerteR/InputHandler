#include <map>
#include <tuple>
#include "Includes.hpp"
#include "Timer.hpp"
#include "InputHandler.hpp"
#include "InputContextHandler.hpp"

bool debug = true;
bool running = true;
int currentMods;

void scrollCallback(GLFWwindow *window, double dx, double dy){
	// inputHandler.scrollCallback(dx, dy);
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
	// inputHandler.keyCallback(key, action, mods);
}
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
	// inputHandler.mouseButtonCallback(button, action, mods);
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

    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowCloseCallback(window, exitCallback);

    InputHandler inputHandler;
    inputHandler.setBinding("esc:exit");
    inputHandler.setBinding("alt-f4:exit");
    inputHandler.setBinding("f4:exit");
    inputHandler.setBinding("f1:help");
    inputHandler.setBinding("hold-shift:stats");
    inputHandler.setBinding("shift-c:setOrigin");
    inputHandler.setBinding("ctrl-c:setOrigin");

    InputContextHandler context(inputHandler);
    context.setAction("setOrigin", []{log("setOrigin");});
    context.setAction("help", []{log("This is help");log("-------");});
    context.setAction("stats", []{log("Score: 12300");});
    context.setAction("W", "forward", []{log("Start engine");}, []{log("Stop engine");});
    context.setAction("hold-W", "forward", []{log("brum-brum");});
    context.setAction("exit", []{std::cout<<"bye :D"; running = false;});
    // context.emplacePressRepeatRelease("shift-c", []{}, []{}, []{});
    context.activate();

    std::string tab[4] = {"aa", "bb", "cc", "dd"};
    int it=0;

    auto tankC = context.derive();
    tankC->setAction("scrollUp", "1", [&it]{ it = (it+1)%4; });
    tankC->setAction("scrollDown", "2", [&it]{ it = (it-1)%4;if(it<0) it=4+it; });
    tankC->setAction("LMB", "fire",  [&it, &tab]{ log(tab[it]); });
    tankC->activate();

    loop();
    tankC->deactivate();
    context.deactivate();
    /*
        Obsługa ruchu myszy i pada:
        input->keyboard.setAction();
        input->mouse.setAction("movement", [this]{});
        input->mouse.setAction("position", [this]{});
        input->pad.setAction("", [this]{});

    */

    return 0;
}
