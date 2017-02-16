#pragma once
#include <functional>
#include <map>
#include <string>

namespace InputHandler
{

using Lambda = std::function<void(void)>;

class IContextImpl
{
public:
    IContextImpl(const std::string &name) : name(name){}
    virtual ~IContextImpl() = default;
    virtual void emplace(int k, int a, int m, const std::string &internalName, Lambda func) = 0;
    virtual void erase(const std::string &internalName) = 0;
    virtual void unset(const std::string &str) = 0;
    virtual bool execute(int k, int a, int m) = 0;
    std::string name;
    bool active {false};
};

}
