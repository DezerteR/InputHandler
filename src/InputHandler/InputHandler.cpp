#include "InputHandler.hpp"
#include "Logging.hpp"
#include "StringToKeyMapping.hpp"
#include "StringToModMapping.hpp"
#include "ContextImplForKeyboard.hpp"
#include <deque>
#include <list>
#include <map>
#include <memory>

static bool debug = true;
static bool running = true;

namespace InputHandler {

int currentMods;

void scrollCallback(double dx, double dy){
    // if(debug) log(__FUNCTION__, "dx:", dx, "dy:", dy);
    if(dy > 0) execute(SCROLL_UP, GLFW_PRESS, currentMods);
    if(dy < 0) execute(SCROLL_DOWN, GLFW_PRESS, currentMods);
}
void keyCallback(int key, int action, int mods){
    // if(debug) log(__FUNCTION__, "key:", key, "action:", action, "mods:", mods);
    currentMods = mods;
    execute(key, action, mods);
}
void mouseButtonCallback(int button, int action, int mods){
    // if(debug) log(__FUNCTION__, "button:", button, "action:", action, "mods:", mods);

    currentMods = mods;
    switch(button){
        case GLFW_MOUSE_BUTTON_LEFT: { button = LMB; break; }
        case GLFW_MOUSE_BUTTON_RIGHT: { button = RMB; break; }
        case GLFW_MOUSE_BUTTON_MIDDLE: { button = MMB; break; }
    }
    execute(button, action, mods);
}
/// padCallback

std::deque<Context*> stackOfContext;
std::multimap<std::string, std::string> functionAndKeyBindings;

u32 hashInput(int k, int a, int m){
    if(k > 256){
        switch(k){
            case GLFW_KEY_KP_ENTER: { k = GLFW_KEY_ENTER ; break; }
            case GLFW_KEY_KP_DIVIDE : { k = '/'; break; }
            case GLFW_KEY_KP_MULTIPLY: { k = '*'; break; }
            case GLFW_KEY_KP_SUBTRACT : { k = '-'; break; }
            case GLFW_KEY_KP_ADD : { k = '+'; break; }
            case GLFW_KEY_KP_EQUAL : { k = '='; break; }
            case GLFW_KEY_RIGHT_SHIFT : { k = GLFW_KEY_LEFT_SHIFT; break; }
            case GLFW_KEY_RIGHT_CONTROL : { k = GLFW_KEY_LEFT_CONTROL; break; }
            case GLFW_KEY_RIGHT_ALT : { k = GLFW_KEY_LEFT_ALT; break; }
            case GLFW_KEY_RIGHT_SUPER : { k = GLFW_KEY_LEFT_SUPER; break; }
            default:{
                if(k >= GLFW_KEY_KP_0 and k <= GLFW_KEY_KP_9) k -= GLFW_KEY_KP_0 + '0';
            }
        }
        /// in case we want only shift pressed and behave as key not mod
        if(k == GLFW_KEY_LEFT_SHIFT and m & GLFW_MOD_SHIFT) m = 0;
        if(k == GLFW_KEY_LEFT_CONTROL and m & GLFW_MOD_CONTROL) m = 0;
        if(k == GLFW_KEY_LEFT_ALT and m & GLFW_MOD_ALT) m = 0;
        if(k == GLFW_KEY_LEFT_SUPER and m & GLFW_MOD_SUPER) m = 0;
    }

    /// m is 4bits, a is 2bits, k is at least 9bits
    return u32( k<<6 | a <<4 | m );
}
u32 hashInput(KeyActionMode keys){
    return hashInput(keys.key, keys.action, keys.modifier);
}
/// keys:function
std::pair<std::string, std::string> splitToFunctionAndKeys(const std::string &str){
    // str.erase( remove(str.begin(), str.end(),' '), str.end() );
    int a=0;
    for(int i=0; i<str.size(); i++){
        if(str[i] == ':'){
            return std::make_pair(str.substr(i+1), str.substr(0, i));
        }
    }
    return {};
}
std::vector<std::string> splitToKeys(std::string str){
    if(str.size() == 1) return {str};
    if(str.back() == '-') str.replace(str.size()-1, 1, "minus");

    int a=0;
    std::vector<std::string> values;
    for(int i=0; i<str.size(); i++){
        if(str[i] == '-'){
            values.push_back(str.substr(a, i-a));
            a = i+1;
        }
    }
    values.push_back(str.substr(a));
    return values;
}
KeyActionMode parseKeyBinding(const std::string &str){
    KeyActionMode out {};
    std::vector<std::string> values = splitToKeys(str);
    // log(str);
    // for(auto &it : values) log("\t", it);
    if(values.front() == "hold"){
        out.action = GLFW_REPEAT;
        values = std::vector<std::string>(values.begin()+1, values.end());
    }
    else out.action = GLFW_PRESS;

    for(int i=0; i<values.size()-1; i++){
        if(stringToMod.count(values[i])){
            out.modifier |= stringToMod.at(values[i]);
        }
        else {
            error("No modifier key:", values[i]);
        }
    }
    if(stringToKey.count(values.back()))
        out.key = stringToKey.at(values.back());
    else {
        out.key = int(values.back()[0]);
        // log("[Warning] unknown key value:", values.back());
    }
    if(out.key >= 'a' and out.key <= 'z') out.key -= 'a' - 'A';
    // log("\t", out.key);
    return out;
}

void forEachBinding(const std::string &functionName, std::function<void(const std::string&)> fun){
    auto keys = functionAndKeyBindings.equal_range(functionName);
    for(auto it = keys.first; it != keys.second; ++it)
        fun(it->second);
}
void registerKeyCombination(const std::string &str){
    auto funcAndKeys = splitToFunctionAndKeys(str);
    functionAndKeyBindings.emplace(funcAndKeys.first, funcAndKeys.second);
}
void activate(Context* context){
    stackOfContext.push_front(context);
}
void deactivate(Context* context){
    std::remove_if(std::begin(stackOfContext), std::end(stackOfContext), [&context](const Context* it){ return it==context; });
}
void executeAction(int k, int a, int m){
    stackOfContext.front()->contextImpl->execute(k, a, m);
}

std::list<int> currentlyPressedKeys;
int currentModifierKey;
void execute(int k, int a, int m){
    if(a != GLFW_PRESS and a != GLFW_RELEASE) return; /// GLFW_REPEAT is handled internally in funcion refresh as default behaviour for repeating keys sucks

    if(a == GLFW_PRESS){
        currentlyPressedKeys.push_back(k);
    }
    else if(a == GLFW_RELEASE){
        currentlyPressedKeys.remove(k);
    }

    currentModifierKey = m;
    executeAction(k, a, m);
}

void refresh(){
    for(auto &it : currentlyPressedKeys){
        executeAction(it, 2, currentModifierKey);
    }
}

Context::Context(std::string contextName, std::vector<std::string>contextsToInheritFrom, int behaviour) : contextName(contextName), behaviour(behaviour), contextImpl(std::make_unique<ContextImplForKeyboard>(contextName)){

}
Context::Context(std::string contextName, int behaviour) : Context(contextName, {}, behaviour) {}
Context::~Context(){
    deactivate();
}
void Context::setAction(const std::string &functionName, Lambda onEnter, Lambda onExit){
    forEachBinding(functionName, [&](const std::string &binding){
        setAction(binding, functionName, onEnter, onExit);
    });
}
void Context::setAction(const std::string &binding, const std::string &name, Lambda onEnter, Lambda onExit){
    auto keys = parseKeyBinding(binding);
    if(onEnter){
        // log(binding, keys.key, keys.action, keys.modifier);
        contextImpl->emplace(keys.key, keys.action, keys.modifier, name, onEnter);
    }
    keys.action = GLFW_RELEASE;
    if(onExit){
        contextImpl->emplace(keys.key, keys.action, keys.modifier, name, onExit);
    }
}

void Context::execute(int k, int a, int m){
    contextImpl->execute(k, a, m);
}
void Context::activate(){
    InputHandler::activate(this);
}
void Context::deactivate(){
    InputHandler::deactivate(this);
}

}
