#pragma once
#include "IContextImpl.hpp"
#include "Utils.hpp"
#include "Timer.hpp"

namespace InputHandler
{

#define BIT(x) (1<<(x))

enum ConsumeInput
{
    DEFAULT = BIT(0),
    CONSUME_ALL = BIT(1),
    PASS_ALL = BIT(2),
};

enum ModificatorKeyPolicy
{
    INCLUDE_IN_MAPPING = BIT(5),
    EXCLUDE_FROM_MAPPING = BIT(6),
};

void scrollCallback(double dx, double dy);
void keyCallback(int key, int action, int mods);
void mouseButtonCallback(int button, int action, int mods);
void execute(int k, int a, int m);
void registerKeyCombination(const std::string &str);

class Context
{
public:
    Context(std::string contextName, std::vector<std::string>contextsToInheritFrom, int behavour = DEFAULT);
    Context(std::string contextName, int behavour = DEFAULT);
    ~Context();
    void setBinding(const std::string &function, Lambda onEnter, Lambda onExit={});
    void setBinding(const std::string &function, const std::string &name, Lambda onEnter, Lambda onExit={});
    void execute(int k, int a, int m);
    void activate();
    void deactivate();
    std::string contextName;
    std::unique_ptr<IContextImpl> contextImpl;
    int behavour;
};

}
