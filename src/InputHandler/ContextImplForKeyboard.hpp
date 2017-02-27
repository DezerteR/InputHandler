#pragma once
#include "IContextImpl.hpp"
#include <map>
/**
 * In this implementation there are no 'modifier keys', they should be handled by user via if statements
 * Also there can be many action under one key, as context can derive actions from other contexts
 *
 */


namespace InputHandler
{

class ContextImplForKeyboard : public IContextImpl
{
public:
    ContextImplForKeyboard(const std::string &name) : IContextImpl(name){}
    void emplace(int k, int a, int m, const std::string &internalName, Lambda func){
        auto hashed = hashInput(k, a, 0);
        actions.emplace(hashed, Action{internalName, func});
    }
    void erase(const std::string &internalName){
        for(auto it=actions.begin(); it != actions.end(); it++){
            if(it->second.name == internalName)
                actions.erase(it);
        }
    }
    void unset(const std::string &str){
        auto keys = parseKeyBinding(str);
        keys.modifier = 0;
        actions.erase(hashInput(keys));
        keys.action = GLFW_RELEASE;
        actions.erase(hashInput(keys));
    }
    void execute(int k, int a, int m){
        // log(k,a,m);
        auto range = actions.equal_range(hashInput(k, a, 0));
        for (auto it = range.first; it != range.second; ++it){
            it->second();
        }
    }
private:
    std::multimap<u32, Action> actions;
};
}
