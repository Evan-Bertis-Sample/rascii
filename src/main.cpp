//
// rascii
// Created by: Evan Bertis-Sample
// 
// A dependency free 3D rendering engine, that outputs to the terminal using ASCII characters
// Built purely for fun, and to learn more about 3D graphics
//

#include <iostream>
#include "tex.hpp"
#include "raster.hpp"
#include "runtime_input.hpp"

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
    
    // update loop
    while (true) {
        // listen to input
        InputListener::listen();
    }

    return 0;
}
