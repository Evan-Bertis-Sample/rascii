#ifndef __TEX_H__
#define __TEX_H__

// Header file for all things related to textures
// Colors, textures, etc.

// Dependencies
#include <string>
#include <sstream>
#include "vec.hpp"

// Forward declarations
struct Vec;

/// @brief A compact representation of a color
/// @details A color is represented by 4 bytes, one for each channel
struct Color {
public:
    unsigned char r, g, b, a;

    /// @brief Default constructor
    /// @details Initializes the color to black
    Color() : r(0), g(0), b(0), a(0) {}

    /// @brief Constructor
    /// @details Initializes the color to the given values from bytes (0-255)
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}

    /// @brief Copy constructor
    /// @details Initializes the color to the given color
    Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}

    /// @brief Float constructor
    /// @details Initializes the color to the given values from floats (0-1)
    static Color fromFloat(float r, float g, float b, float a = 1.0f) {
        return Color(
            Color::toChar(r),
            Color::toChar(g),
            Color::toChar(b),
            Color::toChar(a)
        );
    }

    /// @brief Greyscale constructor
    /// @details Creates a greyscale color from the given value (0-1)
    static Color greyscale(float v) {
        return Color::fromFloat(v, v, v);
    }

    /// @brief Assignment operator
    /// @details Assigns the color to the given color
    Color& operator=(const Color& c) {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }

    /// @brief Addition operator
    /// @details Adds the given color to this color
    Color operator+(const Color& c) const {
        return Color(r + c.r, g + c.g, b + c.b, a + c.a);
    }

    /// @brief Subtraction operator
    /// @details Subtracts the given color from this color
    Color operator-(const Color& c) const {
        return Color(r - c.r, g - c.g, b - c.b, a - c.a);
    }

    /// @brief Multiplication operator
    /// @details Multiplies the given color with this color
    Color operator*(const Color& c) const {
        // Convert to float, to avoid overflow
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);

        float cr = c.toFloat(c.r);
        float cg = c.toFloat(c.g);
        float cb = c.toFloat(c.b);
        float ca = c.toFloat(c.a);

        return Color::fromFloat(fr * cr, fg * cg, fb * cb, fa * ca);
    }

    /// @brief Scalar multiplication operator
    /// @details Multiplies the given color with a float (0-1)
    Color operator*(float f) const {
        // Convert to float, to avoid overflow
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);
        return Color::fromFloat(fr * f, fg * f, fb * f, fa * f);
    }

    float getLuminance() const {
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);

        float l = 0.2126f * fr + 0.7152f * fg + 0.0722f * fb;

        return l;
    }

    /// @brief Converts the color to a string
    /// @details Converts the color to a string
    std::string toString() const {
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);

        std::stringstream ss;
        ss << "(" << fr << ", " << fg << ", " << fb << ", " << fa << ")";
        return ss.str();
    }

private:
    /// @brief converts 0-255 to 0-1 (char to float)
    static inline float toFloat(unsigned char c) {
        return (float)c / 255.0f;
    }

    /// @brief converts 0-1 to 0-255 (float to char)
    static inline unsigned char toChar(float f) {
        return (unsigned char)(f * 255.0f);
    }
};

/// @brief A compact representation of a texture
/// @details A texture is represented by a 2D array of colors
struct Texture {
public:
    /// @brief Default constructor
    /// @details Initializes the texture to a 1x1 black texture
    Texture() : _width(1), _height(1), _pixels(new Color[1]) {
        _pixels[0] = Color();
    }

    /// @brief Constructor
    /// @details Initializes the texture to a given size
    Texture(int width, int height) : _width(width), _height(height), _pixels(new Color[width * height]) {
        for (int i = 0; i < width * height; i++) {
            _pixels[i] = Color();
        }
    }

    /// @brief Copy constructor
    /// @details Initializes the texture to a copy of the given texture
    Texture(const Texture& t) : _width(t._width), _height(t._height), _pixels(new Color[t._width * t._height]) {
        for (int i = 0; i < _width * _height; i++) {
            _pixels[i] = t._pixels[i];
        }
    }

    /// @brief Destructor
    /// @details Deletes the texture
    ~Texture() {
        delete[] _pixels;
    }

    /// @brief Assignment operator
    /// @details Assigns the texture to the given texture
    Texture& operator=(const Texture& t) {
        _width = t._width;
        _height = t._height;
        delete[] _pixels;
        _pixels = new Color[_width * _height];
        for (int i = 0; i < _width * _height; i++) {
            _pixels[i] = t._pixels[i];
        }
        return *this;
    }

    /// @brief Gets the color at the given coordinates
    /// @details Gets the color at the given coordinates
    Color get(int x, int y) const {
        return _pixels[y * _width + x];
    }

    /// @brief Sets the color at the given coordinates
    /// @details Sets the color at the given coordinates
    void set(int x, int y, const Color& c) {
        _pixels[y * _width + x] = c;
    }

    /// @brief Gets the width of the texture
    /// @details Gets the width of the texture
    int getWidth() const {
        return _width;
    }

    /// @brief Gets the height of the texture
    /// @details Gets the height of the texture
    int getHeight() const {
        return _height;
    }

private:
    int _width, _height;
    Color* _pixels;
};

#endif // __TEX_H__