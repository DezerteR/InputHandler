#pragma once
#include <GLFW/glfw3.h>

const std::map<std::string, int> stringToMod = {
    { "shift", GLFW_MOD_SHIFT },
    { "ctrl", GLFW_MOD_CONTROL },
    { "alt", GLFW_MOD_ALT },
    { "super", GLFW_MOD_SUPER },
};
