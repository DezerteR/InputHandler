#include "InputContextHandler.hpp"
#include "InputHandler.hpp"
#include "InputHandlerUtils.hpp"
#include "StringToKeyMapping.hpp"

bool InputContextHandler::execute(int k, int a, int m){
    auto range = actions.equal_range(hashInput(k, a, m)); /// first with mod keys
    if(range.second == range.first) range = actions.equal_range(hashInput(k, a, 0)); /// because sometimes mod key can be pressed unintentionally
    bool anyExecuted = false;
    for(auto it = range.first; it != range.second; ++it){
        it->second();
        anyExecuted = true;
    }
    if(parent) parent->execute(k, a, m);
    return anyExecuted;
}
bool InputContextHandler::execute(int arg, float x, float y){
    auto range = actions2f.equal_range(arg);
    bool anyExecuted = false;
    for(auto it = range.first; it != range.second; ++it){
        it->second(x, y);
        anyExecuted = true;
    }
    if(parent) parent->execute(arg, x, y);
    return anyExecuted;
}

void InputContextHandler::setAction(const std::string &actionName, Lambda onEnter, Lambda onExit){
    auto keys = inputHandler.getBinding(actionName);
    for(auto it = keys.first; it != keys.second; ++it){
        setAction(actionName, it->second, onEnter, onExit);
    }
}
void InputContextHandler::setAction(const std::string &binding, const std::string &name, Lambda onEnter, Lambda onExit){
    auto keys = parseKeyBinding(binding);
    if(onEnter){
        auto hashed = hashInput(keys.key, keys.action, keys.modifier);
        actions.emplace(hashed, Action{name, onEnter});
    }
    keys.action = GLFW_RELEASE;
    if(onExit){
        auto hashed = hashInput(keys.key, keys.action, keys.modifier);
        actions.emplace(hashed, Action{name, onExit});
    }
}
void InputContextHandler::setAction(const std::string &binding, const std::string &name, Lambda2F function){
    auto keys = parseKeyBinding(binding);
        auto hashed = hashInput(keys.key, keys.action, keys.modifier);
        actions2f.emplace(hashed, Action2F{name, function});
}
void InputContextHandler::activate(){
    lastActive = inputHandler.active;
    inputHandler.active = this;
}
void InputContextHandler::deactivate(){
    if(inputHandler.active == this) inputHandler.active = lastActive;
}
std::shared_ptr<InputContextHandler> InputContextHandler::derive(const std::string& newName){
    auto child = std::make_shared<InputContextHandler>(inputHandler, newName);

    child->parent = this;
    children.push_back(child);

    return child;
}
