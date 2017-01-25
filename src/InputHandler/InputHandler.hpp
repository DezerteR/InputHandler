#pragma once
#include <map>
#include <deque>
#include <memory>
#include "Utils.hpp"
#include "Timer.hpp"

namespace InputHandler
{
class InputHandlerContextBindingContainer;
using Lambda = std::function<void(void)>;

enum ConsumeInput
{
    DEFAULT,
    CONSUME_ALL,
    PASS_ALL
};

void scrollCallback(double dx, double dy);
void keyCallback(int key, int action, int mods);
void mouseButtonCallback(int button, int action, int mods);
void execute(int k, int a, int m);
void registerKeyCombination(const std::string &str);

class Context
{
public:
    Context(std::string contextName, ConsumeInput consumeInput = DEFAULT);
    ~Context();
    void setBinding(const std::string &function, Lambda onEnter, Lambda onExit={});
    void setBinding(const std::string &function, const std::string &name, Lambda onEnter, Lambda onExit={});
    void execute(int k, int a, int m);
    void activate();
    void deactivate();
    std::string contextName;
    std::shared_ptr<InputHandlerContextBindingContainer> contextImpl;
    ConsumeInput consumeInput;
};

}
