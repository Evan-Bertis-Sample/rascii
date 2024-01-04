#ifndef __CONTROL_H__
#define __CONTROL_H__

// Header file for all things related to controls
// Sets the controls for the game

// Dependencies
#include <set>
#include <map>
#include "runtime_input.hpp"

struct Vec;

class Controls
{
public:
    // Controls
    std::string Jump = "Jump";
    std::string MoveAxis = "Move";
    std::string LookAxis = "Look";

    Controls() {
        std::cout << "Initializing controls..." << std::endl;
        this->_inputListener = std::make_shared<InputListener>();
        std::cout << "Controls initialized" << std::endl;
    }

    std::shared_ptr<InputListener> getInputListener() const {
        return this->_inputListener;
    }

    void addAxisCallback(std::string axisName, VecCallback callback) {
        std::shared_ptr<AxisListener> axisListener = this->_axisControls.at(axisName);
        if (axisListener == nullptr) {
            throw std::runtime_error("AxisListener not found");
        }
        axisListener->addCallback(callback);
    }

    void addAxisCallback(std::string axisName, std::function<void(Vec)> callbackFunc) {
        VecCallback callback(callbackFunc);
        this->addAxisCallback(axisName, callback);
    }

    void addButtonCallback(std::string buttonName, PressCallback callback) {
        std::string button = this->_buttonControls.at(buttonName);
        if (button == "") {
            throw std::runtime_error("Button not found");
        }
        // now add the callback to the InputListener
        this->_inputListener->addCallback(button, callback);
    }
    
private:
    std::shared_ptr<InputListener> _inputListener;

    std::map<std::string, std::string> _buttonControls = {
        { this->Jump, "Space"}
    };
    std::map<std::string, std::shared_ptr<AxisListener>> _axisControls = {
        { this->MoveAxis, std::make_shared<WASDListener>()},
        { this->LookAxis, std::make_shared<MouseListener>()}
    };

    void buildControls() {
        for (auto& pair : this->_axisControls) {
            std::string axisName = pair.first;
            std::shared_ptr<AxisListener> axisListener = pair.second;
            this->_inputListener->addAxisListener(axisListener);
        }
    }
};

#endif // __CONTROL_H__