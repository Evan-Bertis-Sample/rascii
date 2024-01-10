#include <iostream>
#include <memory>
#include <stdlib.h>
#include <signal.h>
#include <Windows.h>

#include "app.hpp"
#include "tex.hpp"
#include "display.hpp"
#include "runtime_input.hpp"
#include "render.hpp"


Controls App::controls = Controls();

App::App()
{
    // get the height and width of the console
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    this->OUTPUT_HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top;
    this->OUTPUT_WIDTH = csbi.srWindow.Right - csbi.srWindow.Left;

    // initialize the display
    this->_display = AsciiDisplay(this->OUTPUT_WIDTH, this->OUTPUT_HEIGHT);
}

void App::init()
{
    this->_inputListener = App::controls.getInputListener();
}

void App::run()
{
    RenderSettings settings(OUTPUT_WIDTH, OUTPUT_HEIGHT, 120.0f, 0.1f, 100.1f);
    std::cout << settings.toString() << "\n";
    RasciiRenderer renderer = RasciiRenderer(settings);

    // create the scene graph
    SceneGraph sceneGraph = SceneGraph();
    Mesh mesh = Mesh::centeredQuad();
    Mesh mesh2 = Mesh::centeredQuad();

    // create the transform node
    std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>(mesh);
    RenderInfo renderInfo = RenderInfo(meshPtr);
    std::shared_ptr<TransformNode> transformNode = std::make_shared<TransformNode>(Transform(), renderInfo);
    // translate the transform node
    transformNode->transform.move(Vec(3.0f, 0.0f, -25.0f));
    // add the transform node to the scene graph
    sceneGraph.addChild(transformNode);

    std::shared_ptr<TransformNode> childNode = std::make_shared<TransformNode>(Transform(), renderInfo);
    childNode->transform.move(Vec(0.0f, 0.0f, -5.0f));
    childNode->transform.scaleBy(0.5f);
    transformNode->addChild(childNode);


    // create the transform node
    std::shared_ptr<Mesh> meshPtr2 = std::make_shared<Mesh>(mesh2);
    RenderInfo renderInfo2 = RenderInfo(meshPtr2);
    std::shared_ptr<TransformNode> transformNode2 = std::make_shared<TransformNode>(Transform(), renderInfo2);
    // translate the transform node
    transformNode2->transform.move(Vec(-3.0f, 0.0f, -15.0f));
    // add the transform node to the scene graph
    sceneGraph.addChild(transformNode2);

    // std::cout << "Created scene graph\n";
    Quaternion rotationQuaternion = Quaternion::fromAxisAngle(Vec(0.0f, 1.0f, 0.0f), 0.002f);
    Quaternion childQuaternion = Quaternion::fromAxisAngle(Vec(1.0f, 0.0f, 0.0f), -0.002f);
    Quaternion rotationQuaternion2 = Quaternion::fromAxisAngle(Vec(1.0f, 1.0f, 1.0f), -0.0002f);


    // update loop
    while (true)
    {
        _display.prepare();
        renderer.prepare();
        // create the displayer
        this->_inputListener->listen();
        // render the scene graph
        renderer.render(sceneGraph);

        // draw the output
        this->_display.draw(*renderer.getOutput());

        transformNode->transform.rotate(rotationQuaternion);
        childNode->transform.rotate(childQuaternion);
        transformNode2->transform.rotate(rotationQuaternion2);
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