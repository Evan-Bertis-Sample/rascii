#include <iostream>
#include <memory>
#include <stdlib.h>
#include <signal.h>

#include "app.hpp"
#include "tex.hpp"
#include "display.hpp"
#include "runtime_input.hpp"
#include "render.hpp"

const int OUTPUT_WIDTH = 128;
const int OUTPUT_HEIGHT = 32;

Controls App::controls = Controls();

App::App() : _display(OUTPUT_WIDTH, OUTPUT_HEIGHT)
{
    std::cout << "Creating RASCII\n";
}

void App::init()
{
    std::cout << "Initializing RASCII\n";
    this->_inputListener = App::controls.getInputListener();
    std::cout << "Initialized RASCII\n";
}

void App::run()
{
    RenderSettings settings(OUTPUT_WIDTH, OUTPUT_HEIGHT, 90.0f, 0.1f, 100.0f);
    std::cout << settings.toString() << "\n";
    RasciiRenderer renderer = RasciiRenderer(settings);
    renderer.prepare();

    // create the scene graph
    SceneGraph sceneGraph = SceneGraph();
    Mesh mesh = Mesh::centeredQuad();
    std::cout << mesh.toString() << "\n";

    // create the transform node
    std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>(mesh);
    std::cout << meshPtr->toString() << "\n";
    RenderInfo renderInfo = RenderInfo(meshPtr);
    std::shared_ptr<TransformNode> transformNode = std::make_shared<TransformNode>(Transform(), renderInfo);
    // add the transform node to the scene graph
    sceneGraph.addChild(transformNode);

    std::cout << "Created scene graph\n";
    // create the displayer
    this->_inputListener->listen();
    // render the scene graph
    renderer.render(sceneGraph);
    // draw the output
    this->_display.draw(*renderer.getOutput());

    // update loop
    // while (true)
    // {
    // }
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