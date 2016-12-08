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

class InputHandler
{
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
    ~InputHandler();

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
    static std::multimap<u32, Event> keysToOperator;
    static std::map<std::string, std::pair<int, int>> defaultKeyBindings; /// zrobić to multimapą
    std::map<std::string, std::multimap<u32, Event>::iterator> iterators;
};
