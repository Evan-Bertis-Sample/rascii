#ifndef __RASTER_H__
#define __RASTER_H__

// Header file for all things related to rasterization
// Rendering, rasterization, etc.

// Dependencies
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
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

    /// @brief Prepares the rasterizer for rendering
    /// @details This function is called before rendering
    virtual void prepare() = 0;

    /// @brief Cleanup output
    /// @details This function is called after rendering
    virtual void cleanup() = 0;
};

/// @brief A rasterizer that renders to the terminal
/// @details This rasterizer renders the texture to the terminal
/// @details The terminal must be large enough to fit the texture
class AsciiRasterizer : public IRasterizer {
public:
    /// @brief Constructor
    /// @details Initializes the rasterizer to the given values
    /// @param width The width of the terminal
    /// @param height The height of the terminal
    AsciiRasterizer(int width, int height) : _width(width), _height(height) {
        // malloc the output buffer
        int bufferSize = this->getBufferSize();
        this->_outputBuffer = (char*)malloc(sizeof(char) * bufferSize);
        this->_outputBuffer[bufferSize - 1] = '\0';

        // create the rewind string -- brings the cursor to the top left
        sprintf(rewindStr, "\x1b[%dA", height+1);
        // create the cleanup string -- clears the terminal
        sprintf(cleanupStr, "\x1b[%dA\x1b[J", height+1);
    }

    /// @brief Prepares the rasterizer for rendering
    /// @details This function is called before rendering
    void prepare() {
        if (startedStream)
        {
            // move the cursor to the top left
            fwrite(rewindStr, sizeof(char), sizeof(rewindStr), stderr);
        }
        else
        {
            // clear the terminal
            fwrite(cleanupStr, sizeof(char), sizeof(cleanupStr), stderr);
            // hide the cursor
            this->hideCursor(true);
        }
    }

    /// @brief Renders the given texture to the terminal
    /// @details This is the main function of the rasterizer
    /// @param tex The texture to render
    void render(const Texture& tex) {
        // get the width and height of the texture
        startedStream = true;
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
                // std::cout << "luminance: " << luminance << std::endl;
                char c = this->luminanceToAscii(luminance);
                int index = y * _width + x + y;
                this->_outputBuffer[index] = c;
            }
            // add a newline
            this->_outputBuffer[y * _width + y + renderWidth] = '\n';
        }

        // write the buffer to the output
        fwrite(this->_outputBuffer, sizeof(char), this->getBufferSize(), stderr);
    }

    /// @brief Cleanup output
    /// @details This function is called after rendering
    void cleanup() {
        // print cleanup string
        if (startedStream)
        {
            fwrite(cleanupStr, sizeof(char), sizeof(cleanupStr), stderr);
        }
        this->hideCursor(false);
    }

    inline int getBufferSize() const {
        return this->_width * this->_height + this->_height + 1;
    }

    ~AsciiRasterizer() {
        free((void*)this->_outputBuffer);
    }

private:
    // the width and height of the terminal
    const int _width;
    const int _height;

    char* _outputBuffer;
    char rewindStr[20];
    char cleanupStr[20];

    bool startedStream = false;

    // used to convert luminance to ascii characters
    const char* _luminanceTable = " .:-=+*#%@";
    const int _luminanceTableSize = 10;

    /// @brief Converts the given luminance to an ascii character
    /// @details This function converts the given luminance to an ascii character
    /// @param luminance The luminance to convert
    /// @return The ascii character
    char luminanceToAscii(float luminance) const {
        // convert luminance to index
        int index = (int)(luminance * (_luminanceTableSize - 1));

        // return the character at the index
        return _luminanceTable[index];
    }

    /// @brief Hides/Shows the cursor
    /// @details This function hides or shows the cursor
    /// @param hide Whether or not to hide the cursor
    void hideCursor(bool hide) {
        if (hide) {
            // hide the cursor
            fwrite("\x1b[?25l", sizeof(char), 6, stderr);
        }
        else {
            // show the cursor
            fwrite("\x1b[?25h", sizeof(char), 6, stderr);
        }
    }
};

#endif // __RASTER_H__