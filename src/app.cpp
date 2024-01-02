#include <iostream>
#include <memory>
#include <stdlib.h>
#include <signal.h>
#include "app.hpp"
#include "tex.hpp"
#include "raster.hpp"
#include "runtime_input.hpp"

const int OUTPUT_WIDTH = 32;
const int OUTPUT_HEIGHT = 16;

App::App() : _rasterizer(OUTPUT_WIDTH, OUTPUT_HEIGHT), _inputListener()
{
    // idk
}

void App::run()
{
    std::shared_ptr<Texture> texPtr = std::make_shared<Texture>(OUTPUT_WIDTH, OUTPUT_HEIGHT, Color::greyscale(0.5f));
    TextureDrawer drawer(texPtr);
    drawer.drawLine(Vec(0, 0, 0), Vec(OUTPUT_WIDTH, OUTPUT_HEIGHT, 0), Color::greyscale(1.0f));
    drawer.drawCircle(Vec(OUTPUT_WIDTH / 2, OUTPUT_HEIGHT / 2, 0), 10, Color::greyscale(1.0f));

    // test output texture
    std::shared_ptr<WASDListener> listener = std::make_shared<WASDListener>(this->_inputListener);
    this->_inputListener.addAxisListener(listener);

    // update loop
    while (true)
    {
        this->_rasterizer.prepare();
        this->_rasterizer.render(*texPtr);
        this->_inputListener.listen();
        // wait for a little bit
        // this will limit the framerate, but keep the cpu usage lower
        // the upper bound for framerate will be 144 fps
        Sleep(1000 / 144);
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