#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <Windows.h>
#include <memory>

#include "runtime_input.hpp"
#include "vec.hpp"

// CommandLineListener
std::vector<std::string> CommandLineListener::getKeysDown()
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
                // convert from keycode to string
                // we'll use the windows api for this

                UINT scanCode = MapVirtualKey(i, MAPVK_VK_TO_VSC);
                long lParamValue = (scanCode << 16);
                WCHAR lpStr[16];
                int result = GetKeyNameTextW(lParamValue, lpStr, 16);

                if (result == 0) continue;

                // convert to string
                // we'll use the windows api for this
                // we'll also convert to lowercase
                // and remove any spaces
                std::wstring wstr(lpStr);
                std::string key(wstr.begin(), wstr.end());

                // now push the key to the vector
                keysDown.push_back(key);
            }
        }

        return keysDown;
    }
}

Vec CommandLineListener::getMousePosition()
{
    POINT p;
    GetCursorPos(&p);
    return Vec(p.x, p.y, 0, 0);
}

// InputListener

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
    for (std::pair<std::string, ButtonState> pair : this->buttonStates)
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

    // now we need to call the axis listeners
    for (std::shared_ptr<AxisListener> listener : this->axisListeners)
    {
        listener->update();
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
    // first check if it exists in our this->buttonStates
    // if it doesn't, we'll assume it's up
    ButtonState state = BUTTON_UP;
    if (this->buttonStates.find(key) != this->buttonStates.end())
    {
        state = this->buttonStates[key];
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
        this->buttonStates[key] = BUTTON_DOWN;
        break;
    case BUTTON_DOWN:
        // call the onHold callback
        for (PressCallback callback : callbacks)
        {
            callback.hold();
        }
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
    // first check if it exists in our this->buttonStates
    // if it doesn't, we'll assume it's up
    ButtonState state = BUTTON_UP;
    if (this->buttonStates.find(key) != this->buttonStates.end())
    {
        state = this->buttonStates[key];
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
        this->buttonStates[key] = BUTTON_UP;
        break;
    }
}

// listeners
Vec MouseListener::getAxis()
{
    return CommandLineListener::getMousePosition();
}