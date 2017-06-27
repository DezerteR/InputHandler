#include <GLFW/glfw3.h>
#include "InputHandlerUtils.hpp"
#include "Includes.hpp"
#include "StringToKeyMapping.hpp"
#include "StringToModMapping.hpp"

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
