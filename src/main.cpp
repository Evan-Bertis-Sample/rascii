//
// rascii
// Created by: Evan Bertis-Sample
// 
// A dependency free 3D rendering engine, that outputs to the terminal using ASCII characters
// Built purely for fun, and to learn more about 3D graphics
//

#include <iostream>
#include <memory>
#include "tex.hpp"
#include "raster.hpp"
#include "runtime_input.hpp"

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
    // std::cout << v.toStriaaaaaaaang() << std::endl;
}

int main() {
    std::cout << "Hello World" << std::endl;

    // test make a color
    Color c(255, 0, 0);
    std::cout << c.toString() << std::endl;

    // test make a color from a float
    Color c2 = Color::fromFloat(1.0f, 0.0f, 0.0f);
    std::cout << c2.toString() << std::endl;

    // test black
    Color c3;
    std::cout << c3.toString() << std::endl;

    // test white
    Color c4 = Color::greyscale(1.0f);
    std::cout << c4.toString() << std::endl;

    // test grey
    Color c5 = Color::greyscale(0.5f);
    std::cout << c5.toString() << std::endl;

    // test make a texture
    Texture t(20, 20, c5);

    // test output texture
    AsciiRasterizer r(20, 20);
    
    InputListener inputListener;

    PressCallback buttonCallback(
        pressCallback,
        holdCallback,
        releaseCallback
    );

    inputListener.addCallback("A", buttonCallback);

    MouseListener listener;

    VecCallback mouseCallback(
        vecCallback
    );

    listener.addCallback(mouseCallback);

    inputListener.addAxisListener(std::make_shared<MouseListener>(listener));
    // update loop
    while (true) {
        inputListener.listen();
    }

    return 0;
}
