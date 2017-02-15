#pragma once
#include "IContextImpl.hpp"

class ContextImpl : public IContextImpl
{
public:
    ContextImpl(const std::string &name) : IContextImpl(name){}
    void emplace(int k, int a, int m, const std::string &internalName, Lambda func){
        auto hashed = hashInput(k, a, m);
        if(map.count(hashed)){
            error(internalName, "is already defined in this handler:", k,a,m);
            return;
        }
        map.emplace(hashed, InputEvent{internalName, func});
    }
    void erase(const std::string &internalName){
        for(auto it=map.begin(); it != map.end(); it++){
            if(it->second.name == internalName)
                map.erase(it);
        }
    }
    void unset(const std::string &str){
        auto keys = parseKeyBinding(str);
        map.erase(hashInput(keys));
        keys.action = GLFW_RELEASE;
        map.erase(hashInput(keys));
    }
    bool execute(int k, int a, int m){
        // log(k,a,m);
        auto it = map.find(hashInput(k,a,m));
        if(it != map.end()){
            it->second();
            return true;
        }
        return false;
    }
private:
    std::multimap<u32, InputEvent> map;
};
