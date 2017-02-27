#pragma once
#include <functional>
#include <string>

namespace InputHandler
{

using Lambda = std::function<void(void)>;

/**
 * Czy dałoby się to jakoś templatkowo zrobić żeby funkcja przyjmowało dowolną ilość parametrów?
 * Pewnie tak, ale na razie nie jest to jakoś paląco potrzebne(nie mam neta)
 */
struct Action
{
    std::string name;
    std::function<void(void)> func;
    void operator () (){
        if(func) {
            log("function", name);
            func();
        }
    }
};

struct KeyActionMode
{
    int key;
    int action;
    int modifier;
};

KeyActionMode parseKeyBinding(const std::string &str);
u32 hashInput(int k, int a, int m);
u32 hashInput(KeyActionMode keys);


class IContextImpl
{
public:
    IContextImpl(const std::string &name) : name(name){}
    virtual ~IContextImpl() = default;
    virtual void emplace(int k, int a, int m, const std::string &internalName, Lambda func) = 0;
    virtual void erase(const std::string &internalName) = 0;
    virtual void unset(const std::string &str) = 0;
    virtual void execute(int k, int a, int m) = 0;
    std::string name;
};

}
