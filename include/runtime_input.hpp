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
    BUTTON_HELD
};

typedef std::map<std::string, std::vector<PressCallback>> PressCallbackMap;

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
    virtual Vec getAxis() = 0;
    void addCallback(VecCallback callback)
    {
        this->callbacks.push_back(callback);
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

/// @brief A class that listens for input from the command line
class CommandLineListener
{
public:
    /// @brief Gets the keys that are currently down -- no state
    static std::vector<std::string> getKeysDown()
    {
        {
            // use the windows api to get the keys
            std::vector<std::string> keysDown;

            // loop through all the keys
            for (int i = 0; i < 256; i++)
            {
                // check if the key is down
                if (GetAsyncKeyState(i) & 0x8000)
                {
                    char keyName[128];
                    GetKeyNameTextA(i << 16, keyName, 128);
                    std::cout << keyName << std::endl;
                    keysDown.push_back(std::string(keyName));
                }
            }

            return keysDown;
        }
    }
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
    void listen()
    {
        std::vector<std::string> keysDown = CommandLineListener::getKeysDown();
        for (std::string key : keysDown)
        {
            this->_handleButtonDown(key);
        }

        // now we need to check if any buttons were released
        // we'll do this by checking if any buttons were down last frame
        // and if they're not down this frame, we'll assume they were released
        std::map<std::string, ButtonState> stateMap = this->buttonStates;
        for (std::pair<std::string, ButtonState> pair : stateMap)
        {
            std::string key = pair.first;
            ButtonState state = pair.second;

            if (state == BUTTON_DOWN)
            {
                // check if the key is not included in the keysDown vector
                // if it isn't, we'll assume it was released
                if (std::find(keysDown.begin(), keysDown.end(), key) == keysDown.end())
                {
                    this->_handleButtonUp(key);
                }
            }
        }
    }

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
    void _handleButtonDown(std::string key)
    {
        // check if there are any callbacks for this button
        // if there aren't, we can just ignore it
        PressCallbackMap callbackMap = this->buttonCallbacks;
        if (callbackMap.find(key) == callbackMap.end())
        {
            return;
        }

        // get the callbacks for this button
        std::vector<PressCallback> callbacks = callbackMap[key];

        // get the state of the button
        // first check if it exists in our statemap
        // if it doesn't, we'll assume it's up
        ButtonState state = BUTTON_UP;
        std::map<std::string, ButtonState> stateMap = this->buttonStates;
        if (stateMap.find(key) != stateMap.end())
        {
            state = stateMap[key];
        }

        switch (state)
        {
        case BUTTON_UP:
            // call the onPress callback
            for (PressCallback callback : callbacks)
            {
                callback.press();
            }

            // update the state
            stateMap[key] = BUTTON_DOWN;
            break;
        case BUTTON_DOWN:
            // call the onHold callback
            for (PressCallback callback : callbacks)
            {
                callback.hold();
            }

            // update the state
            stateMap[key] = BUTTON_HELD;
            break;
        case BUTTON_HELD:
            // do nothing
            break;
        }
    }

    void _handleButtonUp(std::string key)
    {
        // check if there are any callbacks for this button
        // if there aren't, we can just ignore it
        PressCallbackMap callbackMap = this->buttonCallbacks;
        if (callbackMap.find(key) == callbackMap.end())
        {
            return;
        }

        // get the callbacks for this button
        std::vector<PressCallback> callbacks = callbackMap[key];

        // get the state of the button
        // first check if it exists in our statemap
        // if it doesn't, we'll assume it's up
        ButtonState state = BUTTON_UP;
        std::map<std::string, ButtonState> stateMap = this->buttonStates;
        if (stateMap.find(key) != stateMap.end())
        {
            state = stateMap[key];
        }

        switch (state)
        {
        case BUTTON_UP:
            // do nothing
            break;
        default:
            // call the onRelease callback
            for (PressCallback callback : callbacks)
            {
                callback.release();
            }

            // update the state
            stateMap[key] = BUTTON_UP;
            break;
        }
    }
};

#endif // __RUNTIME_INPUT_H__