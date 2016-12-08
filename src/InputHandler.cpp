#include "InputHandler.hpp"

static bool debug = true;
static bool running = true;

const std::map<std::string, int> stringToKey = {
    { "space", GLFW_KEY_SPACE },
    { "esc", GLFW_KEY_ESCAPE },
    { "enter", GLFW_KEY_ENTER },
    { "tab", GLFW_KEY_TAB },
    { "backspace", GLFW_KEY_BACKSPACE },
    { "insert", GLFW_KEY_INSERT },
    { "delete", GLFW_KEY_DELETE },
    { "right", GLFW_KEY_RIGHT },
    { "left", GLFW_KEY_LEFT },
    { "down", GLFW_KEY_DOWN },
    { "up", GLFW_KEY_UP },
    { "page_up", GLFW_KEY_PAGE_UP },
    { "page_down", GLFW_KEY_PAGE_DOWN },
    { "home", GLFW_KEY_HOME },
    { "end", GLFW_KEY_END },
    { "caps_lock", GLFW_KEY_CAPS_LOCK },
    { "scroll_lock", GLFW_KEY_SCROLL_LOCK },
    { "print_screen", GLFW_KEY_PRINT_SCREEN },
    { "pause", GLFW_KEY_PAUSE },
    { "f1", GLFW_KEY_F1 },
    { "f2", GLFW_KEY_F2 },
    { "f3", GLFW_KEY_F3 },
    { "f4", GLFW_KEY_F4 },
    { "f5", GLFW_KEY_F5 },
    { "f6", GLFW_KEY_F6 },
    { "f7", GLFW_KEY_F7 },
    { "f8", GLFW_KEY_F8 },
    { "f9", GLFW_KEY_F9 },
    { "f10", GLFW_KEY_F10 },
    { "f11", GLFW_KEY_F11 },
    { "f12", GLFW_KEY_F12 },
    { "menu", GLFW_KEY_MENU },
    { "shift", GLFW_MOD_SHIFT },
    { "ctrl", GLFW_MOD_CONTROL },
    { "alt", GLFW_MOD_ALT },
    { "super", GLFW_MOD_SUPER },
};


inline u32 hashInput(int k, int a, int m){
    if(k > 256){
        switch(k){
            case GLFW_MOUSE_BUTTON_LEFT: { k = LMB; break; }
            case GLFW_MOUSE_BUTTON_RIGHT: { k = RMB; break; }
            case GLFW_MOUSE_BUTTON_MIDDLE: { k = MMB; break; }
            case GLFW_KEY_KP_ENTER: { k = GLFW_KEY_ENTER ; break; }
            case GLFW_KEY_KP_DIVIDE : { k = '/'; break; }
            case GLFW_KEY_KP_MULTIPLY: { k = '*'; break; }
            case GLFW_KEY_KP_SUBTRACT : { k = '-'; break; }
            case GLFW_KEY_KP_ADD : { k = '+'; break; }
            case GLFW_KEY_KP_EQUAL  : { k = '='; break; }
            default:{
                if(k >= GLFW_KEY_KP_0 and k <= GLFW_KEY_KP_9) k -= GLFW_KEY_KP_0 + '0';
            }
        }
    }

    // std::cout<<std::hex << "composed: " << composed << ", k: " << k << ", a: " << a << ", m: " << m << std::endl;
    /// m is 4bits, a is 2bits, k is at least 9bits
    return u32( k<<6 | a <<4 | m );
}

InputHandler::~InputHandler(){
    for(auto &it : iterators){
        keysToOperator.erase( it.second );
    }
}

void InputHandler::emplaceFromDefault(const std::string &functionName, Lambda func){
    emplaceFromDefault(functionName, GLFW_PRESS, functionName, func);
}

void InputHandler::emplaceFromDefault(const std::string &functionName, int action, Lambda func){
    emplaceFromDefault(functionName, action, functionName, func);
}
void InputHandler::emplaceFromDefault(const std::string &functionName,const std::string &internalName, Lambda func){
    emplaceFromDefault(functionName, GLFW_PRESS, internalName, func);
}
void InputHandler::emplaceFromDefault(const std::string &functionName,  int action, const std::string &internalName, Lambda func){
    if(iterators.count(internalName)){
        error(functionName, "is already defined in this handler");
        return;
    }
    auto binding = defaultKeyBindings.find(functionName);
    if(binding == defaultKeyBindings.end()){
        error("No operator("+functionName+") defined");
        return;
    }

    iterators.emplace(internalName, keysToOperator.emplace(hashInput(binding->second.first, action, binding->second.second), Event{internalName, func}));
}

void InputHandler::emplace(int k, int a, int m, const std::string &internalName, Lambda func){
    if(iterators.count(internalName)){
        error(internalName, "is already defined in this handler");
        return;
    }
    auto it = keysToOperator.emplace(hashInput(k, a, m), Event{internalName, func});
    iterators.emplace(internalName, it);
}
void InputHandler::erase(const std::string &internalName){
    if(not iterators.count(internalName)){
        error(internalName, "is not defined in this handler");
        return;
    }
    keysToOperator.erase( iterators[internalName] );
    iterators.erase(internalName);
}

/// static section
void InputHandler::execute(int k, int a, int m){
    auto keys = keysToOperator.equal_range(hashInput(k, a, m));
    for (auto it = keys.first; it != keys.second; ++it)
        it->second();
}

/// "ctrl-alt-spacebar: jump" jak to parsować?
/// "ctrl-alt--: jump" jak to parsować?
void InputHandler::registerKeyCombination(const std::string &binding){
    int a=0;
    std::vector<std::string> values;
    for(int i=0; i<binding.size(); i++){
        char c = binding[i];
        if(c == ':' and binding[i-1]=='-'){
            values.push_back("-");
            a=i+1;
        }
        else if(c == ':'){
            a=i+1;
        }
        else if(c == '-'){
            values.push_back(binding.substr(a, i-a));
            a = i+1;
        }
    }

    int mods = 0;
    int key = 0;
    for(int i=0; i<values.size()-1; i++){
        mods |= stringToKey[values[i]];
    }
    key = stringToKey[values.back()];
    auto function = s.substr(a);
    defaultKeyBindings.emplace(function, make_pair(mods, key));
}

std::map<std::string, std::pair<int, int>> InputHandler::defaultKeyBindings;
std::multimap<u32, InputHandler::Event> InputHandler::keysToOperator;

