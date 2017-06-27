#pragma once
#include "Includes.hpp"


struct KeyActionMode
{
    int key;
    int action;
    int modifier;
};

u32 hashInput(int k, int a, int m);
u32 hashInput(KeyActionMode keys);
std::pair<std::string, std::string> splitToFunctionAndKeys(const std::string &str);
std::vector<std::string> splitToKeys(std::string str);
KeyActionMode parseKeyBinding(const std::string &str);
