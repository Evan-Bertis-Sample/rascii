//
// rascii
// Created by: Evan Bertis-Sample
// 
// A dependency free 3D rendering engine, that outputs to the terminal using ASCII characters
// Built purely for fun, and to learn more about 3D graphics
//


#include <signal.h>
#include "app.hpp"

App app;

int main() {
    // register the signal handler
    signal(SIGINT, [](int signal) {
        app.onExit(signal);
    });

    // initialize the app
    app.init();
    
    // run the app
    app.run();

    return 0;
}
