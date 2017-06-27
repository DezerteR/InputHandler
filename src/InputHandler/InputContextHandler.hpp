#pragma once
#include "Includes.hpp"
#include <map>
#include <memory>

using Lambda = std::function<void(void)>;
using Lambda2F = std::function<void(float,float)>;

struct Action
{
    std::string name;
    std::function<void(void)> func;
    void operator () (){
        if(func) {
            // log("function", name);
            func();
        }
    }
};

struct Action2F
{
    std::string name;
    std::function<void(float, float)> func;
    void operator () (float x, float y){
        if(func) func(x, y);
    }
};

class InputHandler;

class InputContextHandler
{
private:
    std::multimap<u32, Action> actions;
    std::multimap<u32, Action2F> actions2f;
    InputHandler& inputHandler;
    InputContextHandler* lastActive {nullptr};
    InputContextHandler* parent {nullptr};
    std::vector<std::shared_ptr<InputContextHandler>> children;
public:
    InputContextHandler(InputHandler& inputHandler) : inputHandler(inputHandler){}
    bool execute(int k, int a, int m);
    bool execute(int arg, float x, float y);
    void setAction(const std::string &actionName, Lambda onEnter, Lambda onExit={});
    void setAction(const std::string &binding, const std::string &name, Lambda onEnter, Lambda onExit={});
    void setAction(const std::string &binding, const std::string &name, Lambda2F function);
    void activate();
    void deactivate();
    std::shared_ptr<InputContextHandler> derive();
};
