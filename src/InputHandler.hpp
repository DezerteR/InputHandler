#pragma once
#include <map>
#include <deque>
#include "Utils.hpp"
#include "Timer.hpp"

using Lambda = std::function<void(void)>;

const int LMB = 501;
const int MMB = 502;
const int RMB = 503;
const int SCROLL_UP = 504;
const int SCROLL_DOWN = 505;
const int HOLD_KEY = 666;

namespace InputHandler
{

enum ConsumeInput
{
    DEFAULT,
    CONSUME_ALL,
    PASS_ALL
};

void execute(int k, int a, int m);
void registerKeyCombination(const std::string &str);

class Context
{
public:
    Context(std::string contextName, ConsumeInput consumeInput = DEFAULT);
    ~Context();
    void setFunction(const std::string &function, Lambda onEnter, Lambda onExit={});
    void setBinding(const std::string &function, const std::string &name, Lambda onEnter, Lambda onExit={});
    void activate();
    void deactivate();
private:
    std::string contextName;
    ConsumeInput consumeInput;
};

}
