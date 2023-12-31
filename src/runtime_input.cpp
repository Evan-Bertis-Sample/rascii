#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <Windows.h>
#include <memory>

#include "runtime_input.hpp"
#include "vec.hpp"

void InputListener::listen()
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

void InputListener::_handleButtonDown(std::string &key)
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

void InputListener::_handleButtonUp(std::string &key)
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