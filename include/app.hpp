#ifndef __APP_H__
#define __APP_H__

// Header file for the main application for rascii
// This is the entry point for the program

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include "raster.hpp"
#include "runtime_input.hpp"

class App {
public:
    App();
    ~App();

    /// @brief Runs the application
    /// @details This is the main function of the application
    void run();

    /// @brief Called when the application is started
    /// @details This function is called before the main loop
    void onExit(int exitCode);
private:
    AsciiRasterizer _rasterizer;
    InputListener _inputListener;
};

#endif // __APP_H__