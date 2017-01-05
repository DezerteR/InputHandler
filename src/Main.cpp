#include <map>
#include <tuple>
#include "Utils.hpp"
#include "Timer.hpp"
#include "InputHandler.hpp"

bool debug = true;
bool running = true;
int currentMods;

void scrollCallback(GLFWwindow *window, double dx, double dy){
    // if(debug) log(__FUNCTION__, "dx:", dx, "dy:", dy);
    if(dy > 0) InputHandler::execute(SCROLL_UP, GLFW_PRESS, currentMods);
    if(dy < 0) InputHandler::execute(SCROLL_DOWN, GLFW_PRESS, currentMods);
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    // if(debug) log(__FUNCTION__, "key:", key, "action:", action, "mods:", mods);
    currentMods = mods;
    InputHandler::execute(key, action, mods);
}
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    // if(debug) log(__FUNCTION__, "button:", button, "action:", action, "mods:", mods);

    currentMods = mods;
    switch(button){
        case GLFW_MOUSE_BUTTON_LEFT: { button = LMB; break; }
        case GLFW_MOUSE_BUTTON_RIGHT: { button = RMB; break; }
        case GLFW_MOUSE_BUTTON_MIDDLE: { button = MMB; break; }
    }
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

    /**
     *  Mamy konteksty, lepsze to niż trzymać lokalnie :D, łatwiej usunąć(wylatuje cały kontekst), poruszamy się tylko w jego obrębie,
     *  Nie wiem tylko czy robić stos aktywnych kontekstów, z przechwytywaniem czy też tylko jeden aktywny, nwm
     *
     *  Akcję można przypisać do funkcji(:exit) albo do sktótu klawiszowego(alt-f3), mod klawiszy nie ma dla wciśnięć
     *  Z defaultu akcja odpala się na wciśnięcie klawisza, można dodać też coś na puszczenie
     *  Jeśli ma się dziać na przytrzymaniu to trzeba użyć "hold-shift:zoom", i trzeba pamiętać o szybsze3j aktywacji holda
     *
     *  W handlerze możemy zdefiniować defaultowe skróty klawiszowe, dla jakiejś funkcji przypisujemy dowolna liczbę skrótów
     *  znając nazwę fcji można wyciągnąć przypisane jej klawisze
     *
     *  Klawisze są zdefiniowane na naciśnięcie bądź przytrymanie, puszczenie dodawane jest z defaultu
     *  Parsowanie stringa zwraca trzy inty
     *
     */
    InputHandler::Context context("Main");
    context.setFunction("setOrigin", []{log("setOrigin");});
    context.setFunction("help", []{log("This is help");log("-------");});
    context.setFunction("stats", []{log("Score: 12300");});
    context.setBinding("W", "forward", []{log("Start engine");}, []{log("Stop engine");});
    context.setBinding("hold-W", "forward", []{log("brum-brum");});
    context.setFunction("exit", []{std::cout<<"bye :D"; running = false;});
    // context.emplacePressRepeatRelease("shift-c", []{}, []{}, []{});
    context.activate();

    std::string tab[4] = {"aa", "bb", "cc", "dd"};
    int it=0;

    InputHandler::Context tankC("Tank");
    tankC.setBinding("scrollUp", "1", [&it]{ it = (it+1)%4; });
    tankC.setBinding("scrollDown", "2", [&it]{ it = (it-1)%4;if(it<0) it=4+it; });
    tankC.setBinding("LMB", "fire",  [&it, &tab]{ log(tab[it]); });
    tankC.activate();

    loop();
    context.deactivate();

    return 0;
}