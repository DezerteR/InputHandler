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

    InputHandler::registerKeyCombination("esc:exit");
    InputHandler::registerKeyCombination("alt-f4:exit");
    InputHandler::registerKeyCombination("f1:help");
    InputHandler::registerKeyCombination("hold-shift:stats");

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
     */

    InputHandlerContext context("Main");
    context.setFunction("help", []{log("This is help");log("-------");});
    context.setBinding("W", []{log("Start engine");}, []{log("Stop engine");});
    context.setBinding("hold-tab", []{std::cout<<".";});

    loop();

    return 0;
}