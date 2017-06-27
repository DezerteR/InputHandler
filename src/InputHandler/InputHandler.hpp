#pragma once
#include "Includes.hpp"
#include <list>
#include <map>
#include <memory>

class InputContextHandler;

class InputHandler
{
private:
    std::multimap<std::string, std::string> configuredActions;
    std::list<int> currentlyPressedKeys;
    int currentModifierKey;

    void execute(int k, int a, int m);

public:
    std::shared_ptr<InputContextHandler> root;
    InputContextHandler *active;

    void setBinding(const std::string& combination);

    auto getBinding(const std::string& function) -> decltype(configuredActions.equal_range(function)){
        return configuredActions.equal_range(function);
    }

    void refresh();
    void scrollCallback(double dx, double dy);
    void keyCallback(int key, int action, int mods);
    void mouseButtonCallback(int button, int action, int mods);
    void mousePosition(float x, float y);
    void mouseMovement(float x, float y);
    void joyPadDispatch();
};
