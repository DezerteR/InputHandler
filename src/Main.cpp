#include <map>
#include <tuple>
#include "Utils.hpp"
#include "Timer.hpp"

bool debug = true;
bool running = true;

class InputHandler
{
    using KeyBinding = std::tuple<int, int, int>;
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

    void emplace(int k, int a, int m, const std::string &name, std::function<void(void)> func){
        iterators.emplace(
            name,
            keyBindings.emplace(KeyBinding(k, a, m), Event{name, func}));
    }
    void erase(const std::string &name){
        if(not iterators.count(name)) return;
        keyBindings.erase( iterators[name] );
        iterators.erase(name);
    }
    static void execute(int k, int a, int m){
        auto keys = keyBindings.equal_range(KeyBinding(k, a, m));
        for (auto it = keys.first; it != keys.second; ++it)
            it->second();
    }
private:
    static std::multimap<KeyBinding, Event> keyBindings;
    std::map<std::string, std::multimap<KeyBinding, Event>::iterator> iterators;
};
std::multimap<InputHandler::KeyBinding, InputHandler::Event> InputHandler::keyBindings;

void scrollCallback(GLFWwindow *window, double dx, double dy){
    if(debug) log(__FUNCTION__, "dx:", dx, "dy:", dy);
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(debug) log(__FUNCTION__, "key:", key, "scancode:", scancode, "action:", action, "mods:", mods);

    if(((mods == GLFW_MOD_ALT && key == GLFW_KEY_F4) or key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS){
        running = false;
    }

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