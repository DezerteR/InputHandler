#pragma once
#include <map>
#include <tuple>
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
class InputHandler
{
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
    ~InputHandler();

    bool registerNewContext(const std::string &contextName);
    void deleteContext(const std::string &contextName);

    void forEachBinding(const std::string &function, std::function<void(const std::string&)>fun);

    void emplaceFromDefault(const std::string &functionName, Lambda func);
    void emplaceFromDefault(const std::string &functionName, int action, Lambda func);
    void emplaceFromDefault(const std::string &functionName, int action, const std::string &internalName, Lambda func);
    void emplaceFromDefault(const std::string &functionName, const std::string &internalName, Lambda func);

    void emplace(int k, int a, int m, const std::string &internalName, Lambda func);
    void erase(const std::string &internalName);
    static void execute(int k, int a, int m);
    /// "ctrl-alt-spacebar: jump" jak to parsować?
    static void registerKeyCombination(const std::string &binding);
private:
    static std::multimap<std::string, std::string> functionAndKeyBindings;
    static std::map<std::string, std::map<u32, Event>> contextAndEvents;
};

class InputHandlerContext
{
public:
    InputHandlerContext(std::string contextName) : contextName(contextName){
        InputHandler::registerNewContext(contextName);
    }
    ~InputHandlerContext(){
        InputHandler::deleteContext(contextName);
    }
    void setFunction(const std::string &function, Lambda onEnter, Lambda onExit={});
    void setBinding(const std::string &function, Lambda onEnter, Lambda onExit={});

private:
    std::string contextName;
};