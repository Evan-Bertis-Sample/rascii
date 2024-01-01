//
// rascii
// Created by: Evan Bertis-Sample
// 
// A dependency free 3D rendering engine, that outputs to the terminal using ASCII characters
// Built purely for fun, and to learn more about 3D graphics
//

#include <iostream>
#include <memory>
#include <stdlib.h>
#include "tex.hpp"
#include "raster.hpp"
#include "runtime_input.hpp"

const int OUTPUT_WIDTH = 160;
const int OUTPUT_HEIGHT = 24;

void pressCallback () {
    std::cout << "Pressed" << std::endl;
}

void holdCallback () {
    std::cout << "Held" << std::endl;
}

void releaseCallback () {
    std::cout << "Released" << std::endl;
}

void vecCallback (Vec v) {
    std::cout << v.toString() << std::endl;
}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

int main() {
    // test make a texture
    // Texture t(OUTPUT_WIDTH, OUTPUT_HEIGHT, Color::greyscale(0.0f));
    std::shared_ptr<Texture> texPtr = std::make_shared<Texture>(OUTPUT_WIDTH, OUTPUT_HEIGHT, Color::greyscale(0.5f));
    TextureDrawer drawer(texPtr);
    drawer.drawLine(Vec(0, 0, 0), Vec(OUTPUT_WIDTH, OUTPUT_HEIGHT, 0), Color::greyscale(1.0f));
    drawer.drawCircle(Vec(OUTPUT_WIDTH / 2, OUTPUT_HEIGHT / 2, 0), 10, Color::greyscale(1.0f));
    // test output texture
    AsciiRasterizer r(OUTPUT_WIDTH, OUTPUT_HEIGHT);
    InputListener inputListener;
    PressCallback buttonCallback(
        pressCallback,
        holdCallback,
        releaseCallback
    );
    // inputListener.addCallback("A", buttonCallback);
    std::shared_ptr<WASDListener> listener = std::make_shared<WASDListener>(inputListener);
    VecCallback testVecCallback(
        vecCallback
    );
    // listener->addCallback(testVecCallback);
    
    inputListener.addAxisListener(listener);
    // update loop
    while (true) {
        r.render(*texPtr);
        inputListener.listen();
        // wait for a little biit
        Sleep(10);
        clearScreen();
    }

    return 0;
}
