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
    context.setBinding("setOrigin", []{log("setOrigin");});
    context.setBinding("help", []{log("This is help");log("-------");});
    context.setBinding("stats", []{log("Score: 12300");});
    context.setBinding("W", "forward", []{log("Start engine");}, []{log("Stop engine");});
    context.setBinding("hold-W", "forward", []{log("brum-brum");});
    context.setBinding("exit", []{std::cout<<"bye :D"; running = false;});
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
    tankC.deactivate();
    context.deactivate();

    return 0;
}
