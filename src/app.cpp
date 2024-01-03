#include <iostream>
#include <memory>
#include <stdlib.h>
#include <signal.h>

#include "app.hpp"
#include "tex.hpp"
#include "display.hpp"
#include "runtime_input.hpp"

const int OUTPUT_WIDTH = 128;
const int OUTPUT_HEIGHT = 32;

Controls App::controls = Controls();

App::App() : _display(OUTPUT_WIDTH, OUTPUT_HEIGHT)
{
    std::cout << "Initializing RASCII\n"; 
}

void App::init()
{
    this->_inputListener = App::controls.getInputListener();
}

void App::run()
{
    std::shared_ptr<Texture> texPtr = std::make_shared<Texture>(OUTPUT_WIDTH, OUTPUT_HEIGHT, Color::greyscale(0.5f));
    TextureDrawer drawer(texPtr);
    
    drawer.drawLine(Vec(0, 0, 0), Vec(OUTPUT_WIDTH, OUTPUT_HEIGHT, 0), Color::greyscale(1.0f));
    drawer.fillCircle(Vec(OUTPUT_WIDTH / 2, OUTPUT_HEIGHT / 2, 0), 10, Color::greyscale(1.0f));
    // drawer.fillTriangle(Vec(0, 0, 0), Vec(OUTPUT_WIDTH, 0, 0), Vec(OUTPUT_WIDTH / 2, OUTPUT_HEIGHT, 0), Color::greyscale(1.0f));
    // draw a more scalene, obtuse triangle
    drawer.fillTriangle(Vec(5, 5, 0), 
                        Vec(10, 25, 0), 
                        Vec(25, 10, 0), 
                        Color::greyscale(0));

    // update loop
    while (true)
    {
        this->_display.prepare();
        this->_display.draw(*texPtr);
        this->_inputListener->listen();
    }
}

void App::onExit(int exitCode)
{
    // cleanup
    this->_display.cleanup();
    exit(exitCode);
}

App::~App()
{
    // cleanup
    this->_display.cleanup();
}