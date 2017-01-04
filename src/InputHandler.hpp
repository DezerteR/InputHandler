#pragma once
#include <map>
#include <deque>
#include "Utils.hpp"
#include "Timer.hpp"

const int LMB = 501;
const int MMB = 502;
const int RMB = 503;
const int S_UP = 504;
const int S_DOWN = 505;
const int HOLD_KEY = 666;

u32 hashKeys(int, int, int);

using Lambda = std::function<void(void)>;

struct InputEvent
{
    std::string name;
    std::function<void(void)> func;
    void operator () (){
        if(func) func();
    }
};

class InputHandlerContextBindingContainer;

class InputHandler
{
public:
    static bool registerNewContext(const std::string &contextName);
    static void deleteContext(const std::string &contextName);
    static InputHandlerContextBindingContainer& getContext(const std::string &contextName);
    static void execute(int k, int a, int m);

    static void forEachBinding(const std::string &function, std::function<void(const std::string&)>fun);
    static void registerKeyCombination(const std::string &binding);
    static void activate(const std::string &contextName);
    static void deactivate(const std::string &contextName);
private:
    static std::multimap<std::string, std::string> functionAndKeyBindings;
    static std::map<std::string, InputHandlerContextBindingContainer> contexts;
    static std::deque<std::reference_wrapper<InputHandlerContextBindingContainer>> stackOfContext;
};

class InputHandlerContext
{
public:
    InputHandlerContext(std::string contextName) : contextName(contextName){
        InputHandler::registerNewContext(contextName);
    }
    ~InputHandlerContext(){
        deactivate();
        InputHandler::deleteContext(contextName);
    }
    void setFunction(const std::string &function, Lambda onEnter, Lambda onExit={});
    void setBinding(const std::string &function, const std::string &name, Lambda onEnter, Lambda onExit={});
    void activate();
    void deactivate();
private:
    std::string contextName;
};