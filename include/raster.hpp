#ifndef __RASTER_H__
#define __RASTER_H__


// Header file for all things related to rasterization
// Rendering, rasterization, etc.

// Dependencies
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include "tex.hpp"

/// @brief An interface that all rasterizers must implement
/// @details A rasterizer is responsible for taking a texture and rendering it into some output
/// @details The output could be a terminal, a file, a window, etc.
/// @note This is an interface, and should not be instantiated
class IRasterizer {
public:
    /// @brief Default constructor
    /// @details Initializes the rasterizer to the default values
    IRasterizer() {}

    /// @brief Renders the given texture to the output
    /// @details This is the main function of the rasterizer
    /// @param tex The texture to render
    /// @param output The output to render to
    virtual void render(const Texture& tex) = 0;
};

/// @brief A rasterizer that renders to the terminal
/// @details This rasterizer renders the texture to the terminal
/// @details The terminal must be large enough to fit the texture
class AsciiRasterizer : public IRasterizer {
public:
    /// @brief Default constructor
    /// @details Initializes the rasterizer to the default values
    AsciiRasterizer() : _width(80), _height(24) {
        this->setOutput(std::cout);
    }

    /// @brief Constructor
    /// @details Initializes the rasterizer to the given values
    /// @param width The width of the terminal
    /// @param height The height of the terminal
    AsciiRasterizer(int width, int height) : _width(width), _height(height) {
        this->setOutput(std::cout);
    }

    /// @brief Sets the ouptut stream
    /// @details Sets the output stream to the given stream
    /// @param out The output stream to set
    void setOutput(std::ostream& out) {
        this->out = &out;
    }

    /// @brief Renders the given texture to the terminal
    /// @details This is the main function of the rasterizer
    /// @param tex The texture to render
    void render(const Texture& tex) {
        // get the width and height of the texture
        int texWidth = tex.getWidth();
        int texHeight = tex.getHeight();

        // calculate the width and height of the render
        // the minimum of the terminal size and the texture size
        int renderWidth = std::min(_width, texWidth);
        int renderHeight = std::min(_height, texHeight);

        // loop through each pixel in the render
        for (int y = 0; y < renderHeight; y++) {
            for (int x = 0; x < renderWidth; x++) {
                float luminance = tex.get(x, y).getLuminance();
                char c = this->luminanceToAscii(luminance);
                *(this->out) << c;
            }
            // ouptput a newline
            *(this->out) << std::endl;
        }
    }

private:
    // the width and height of the terminal
    const int _width;
    const int _height;

    // ouptut channel
    std::ostream* out;

    // used to convert luminance to ascii characters
    const char* const _luminanceTable = " .:-=+*#%@";
    const int _luminanceTableSize = 11;

    /// @brief Converts the given luminance to an ascii character
    /// @details This function converts the given luminance to an ascii character
    /// @param luminance The luminance to convert
    /// @return The ascii character
    char luminanceToAscii(float luminance) {
        // convert luminance to index
        int index = (int)(luminance * (_luminanceTableSize - 1));

        // return the character at the index
        return _luminanceTable[index];
    }
};

#endif // __RASTER_H__