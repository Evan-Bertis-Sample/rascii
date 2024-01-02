#include <iostream>
#include <memory>
#include <stdlib.h>
#include <signal.h>

#include "app.hpp"
#include "tex.hpp"
#include "raster.hpp"
#include "runtime_input.hpp"

const int OUTPUT_WIDTH = 128;
const int OUTPUT_HEIGHT = 32;

Controls App::controls = Controls();

App::App() : _rasterizer(OUTPUT_WIDTH, OUTPUT_HEIGHT)
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
    drawer.drawCircle(Vec(OUTPUT_WIDTH / 2, OUTPUT_HEIGHT / 2, 0), 10, Color::greyscale(1.0f));

    // update loop
    while (true)
    {
        this->_rasterizer.prepare();
        this->_rasterizer.render(*texPtr);
        this->_inputListener->listen();
    }
}

void App::onExit(int exitCode)
{
    // cleanup
    this->_rasterizer.cleanup();
    exit(exitCode);
}

App::~App()
{
    // cleanup
    this->_rasterizer.cleanup();
}