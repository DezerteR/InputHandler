#include <map>
#include <tuple>
#include "Utils.hpp"
#include "Timer.hpp"

bool debug = true;
bool running = true;

class InputHandler
{
    using KeyBinding = std::tuple<int, int, int>;
    using Lambda = std::function<void(void)>;
    struct Event
    {
        std::string name;
        std::function<void(void)> func;
        void operator () (){
            if(func) func();
        }
    };
public:
    InputHandler(){}
    ~InputHandler(){
        for(auto &it : iterators){
            keyBindings.erase( it.second );
        }
    }

    void emplaceFromDefault(const std::string &functionName, Lambda func){
        emplaceFromDefault(functionName, functionName, func);
    }
    void emplaceFromDefault(const std::string &functionName, const std::string &internalName, Lambda func){
        if(iterators.count(internalName)){
            error(functionName, "is already defined in this handler");
            return;
        }
        auto binding = keysToOperator.find(functionName);
        if(binding == keysToOperator.end()){
            error("No operator("+functionName+") defined");
            return;
        }

        iterators.emplace(internalName, keyBindings.emplace(binding.second, Event{functionName, func}));
    }

    void emplace(int k, int a, int m, const std::string &internalName, Lambda func){
        if(iterators.count(internalName)){
            error(functionName, "is already defined in this handler");
            return;
        }
        auto it = keyBindings.emplace(KeyBinding(k, a, m), Event{name, func});
        iterators.emplace(internalName, it);
    }
    void erase(const std::string &internalName){
        if(not iterators.count(internalName)){
            error(functionName, "is not defined in this handler");
            return;
        }
        keyBindings.erase( iterators[internalName] );
        iterators.erase(internalName);
    }
    static void execute(int k, int a, int m){
        auto keys = keyBindings.equal_range(KeyBinding(k, a, m));
        for (auto it = keys.first; it != keys.second; ++it)
            it->second();
    }

    /// "ctrl-alt-spacebar: jump" jak to parsować?
    static void registerKeyCombination(const std::string &binding){

    }
private:
    static std::multimap<KeyBinding, Event> keysToOperator;
    static std::map<std::string, KeyBinding> defaultKeyBindings; /// zrobić to multimapą
    std::map<std::string, std::multimap<KeyBinding, Event>::iterator> iterators;
};

std::map<std::string, KeyBinding> InputHandler::keysToOperator;
std::multimap<InputHandler::KeyBinding, InputHandler::Event> InputHandler::keyBindings;

void scrollCallback(GLFWwindow *window, double dx, double dy){
    if(debug) log(__FUNCTION__, "dx:", dx, "dy:", dy);
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(debug) log(__FUNCTION__, "key:", key, "scancode:", scancode, "action:", action, "mods:", mods);

    InputHandler::execute(key, action, mods);
}
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    if(debug) log(__FUNCTION__, "button:", button, "action:", action, "mods:", mods);
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

    inputHandler.emplace('A', GLFW_PRESS, 0, "Default", []{
        log("Dupa!!");
    });
    inputHandler.emplace('A', GLFW_PRESS, 0, "Defaulta", [&inputHandler]{
        log("Wiecej dup!!");
        inputHandler.erase("Default");
    });
    {
        InputHandler ih_2;
        ih_2.emplace('A', GLFW_PRESS, 0, "A-Press", []{
            log("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        });
        loop();
    }
    running = true;
    loop();

    return 0;
}