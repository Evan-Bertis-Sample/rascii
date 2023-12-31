#ifndef __RUNTIME_INPUT_H__
#define __RUNTIME_INPUT_H__

// Header file for all things related to runtime input
// Listening for input during the update loop, etc.

// Dependencies
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <Windows.h>
#include <memory>
#include "vec.hpp"

#pragma region ButtonHandling
struct PressCallback
{
public:
    PressCallback() : onPress(nullptr), onHold(nullptr), onRelease(nullptr) {}
    PressCallback(std::function<void()> onPress = nullptr,
                  std::function<void()> onHold = nullptr,
                  std::function<void()> onRelease = nullptr)
        : onPress(onPress), onHold(onHold), onRelease(onRelease) {}

    void press()
    {
        if (this->onPress != nullptr)
        {
            this->onPress();
        }
    }

    void hold()
    {
        if (this->onHold != nullptr)
        {
            this->onHold();
        }
    }

    void release()
    {
        if (this->onRelease != nullptr)
        {
            this->onRelease();
        }
    }

private:
    std::function<void()> onPress;   // the frame the button is pressed
    std::function<void()> onHold;    // every frame the button is held
    std::function<void()> onRelease; // the frame the button is released
};

enum ButtonState
{
    BUTTON_UP,
    BUTTON_DOWN,
};

typedef std::map<std::string, std::vector<PressCallback>> PressCallbackMap;

#pragma endregion

#pragma region AxisHandling
struct VecCallback
{
public:
    VecCallback() : onUpdate(nullptr) {}
    VecCallback(std::function<void(Vec)> onUpdate) : onUpdate(onUpdate) {}

    void update(Vec v)
    {
        if (this->onUpdate != nullptr)
        {
            this->onUpdate(v);
        }
    }

private:
    std::function<void(Vec)> onUpdate; // every frame
};

/// @brief A class that listens for vector input during the update loop
/// @details This class is responsible for listening for vector input during the update loop
/// @note Example: WASD keys
class AxisListener
{
public:
    /// @brief Gets the axis
    /// @details Gets the axis -- this is abstract, and must be implemented by the child class
    virtual Vec getAxis() { return Vec(); }
    void addCallback(VecCallback callback)
    {
        this->callbacks.push_back(callback);
    }

    void update()
    {
        Vec v = this->getAxis();
        for (VecCallback callback : this->callbacks)
        {
            callback.update(v);
        }
    }

private:
    std::vector<VecCallback> callbacks;
};

/// @brief A class that listens to the WASD keys for input during the update loop
/// @details This class is responsible for listening to the WASD keys for input during the update loop
class WASDListner : public AxisListener
{
public:
    /// @brief Gets the axis
    /// @details Gets the axis -- constructs a vector from the WASD keys
    Vec getAxis();
};

/// @brief A class that listens to the mouse for input during the update loop
/// @details This class is responsible for listening to the mouse for input during the update loop
class MouseListener : public AxisListener
{
public:
    /// @brief Gets the axis
    /// @details Gets the axis -- constructs a vector from the mouse position
    Vec getAxis();
};

#pragma endregion

/// @brief A class that listens for input from the command line
class CommandLineListener
{
public:
    /// @brief Gets the keys that are currently down -- no state
    static std::vector<std::string> getKeysDown();
    static Vec getMousePosition();
};

/// @brief A class that listens for input during the update loop
/// @details This class is responsible for listening for input during the update loop
/// @details It will call the appropriate callbacks when the input is received
class InputListener
{
public:
    /// @brief Default constructor
    /// @details Initializes the input listener to the default values
    InputListener() {}

    /// @brief Listens for input during the update loop
    void listen();

    // Button callbacks
    void addCallback(std::string key, PressCallback callback)
    {
        this->buttonCallbacks[key].push_back(callback);
    }

    // Axis callbacks
    void addAxisListener(std::shared_ptr<AxisListener> listener)
    {
        this->axisListeners.push_back(listener);
    }

private:
    PressCallbackMap buttonCallbacks;
    std::map<std::string, ButtonState> buttonStates;
    std::vector<std::shared_ptr<AxisListener>> axisListeners;

    // responsible for calling the appropriate callbacks, and updating the button states
    void _handleButtonDown(std::string &key);
    void _handleButtonUp(std::string &key);
};

#endif // __RUNTIME_INPUT_H__