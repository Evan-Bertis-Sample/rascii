#ifndef __TEX_H__
#define __TEX_H__

// Header file for all things related to textures
// Colors, textures, etc.

// Dependencies
#include <string>
#include <sstream>
#include <memory>
#include <cstring>
#include "vec.hpp"

// Forward declarations
struct Vec;

/// @brief A compact representation of a color
/// @details A color is represented by 4 bytes, one for each channel
struct Color
{
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
    Color(const Color &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}

    /// @brief Float constructor
    /// @details Initializes the color to the given values from floats (0-1)
    static Color fromFloat(float r, float g, float b, float a = 1.0f)
    {
        return Color(
            Color::toChar(r),
            Color::toChar(g),
            Color::toChar(b),
            Color::toChar(a));
    }

    /// @brief Greyscale constructor
    /// @details Creates a greyscale color from the given value (0-1)
    static Color greyscale(float v)
    {
        return Color::fromFloat(v, v, v);
    }

    /// @brief Assignment operator
    /// @details Assigns the color to the given color
    Color &operator=(const Color &c)
    {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }

    /// @brief Addition operator
    /// @details Adds the given color to this color
    Color operator+(const Color &c) const
    {
        return Color(r + c.r, g + c.g, b + c.b, a + c.a);
    }

    /// @brief Subtraction operator
    /// @details Subtracts the given color from this color
    Color operator-(const Color &c) const
    {
        return Color(r - c.r, g - c.g, b - c.b, a - c.a);
    }

    /// @brief Multiplication operator
    /// @details Multiplies the given color with this color
    Color operator*(const Color &c) const
    {
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
    Color operator*(float f) const
    {
        // Convert to float, to avoid overflow
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);
        return Color::fromFloat(fr * f, fg * f, fb * f, fa * f);
    }

    float getLuminance() const
    {
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);

        float l = 0.2126f * fr + 0.7152f * fg + 0.0722f * fb;

        return l;
    }

    /// @brief Converts the color to a string
    /// @details Converts the color to a string
    std::string toString() const
    {
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
    static inline float toFloat(unsigned char c)
    {
        return (float)c / 255.0f;
    }

    /// @brief converts 0-1 to 0-255 (float to char)
    static inline unsigned char toChar(float f)
    {
        return (unsigned char)(f * 255.0f);
    }
};

/// @brief A compact representation of a texture
/// @details A texture is represented by a 2D array of colors
struct Texture
{
public:
    /// @brief Default constructor
    /// @details Initializes the texture to a 1x1 black texture
    Texture() : _width(1), _height(1), _pixels(new Color[1])
    {
        _pixels[0] = Color();
    }

    /// @brief Constructor
    /// @details Initializes the texture to a given size
    Texture(int width, int height) : _width(width), _height(height), _pixels(new Color[width * height])
    {
        for (int i = 0; i < width * height; i++)
        {
            _pixels[i] = Color();
        }
    }

    Texture(int width, int height, const Color &c) : _width(width), _height(height), _pixels(new Color[width * height])
    {
        for (int i = 0; i < width * height; i++)
        {
            _pixels[i] = c;
        }
    }

    /// @brief Copy constructor
    /// @details Initializes the texture to a copy of the given texture -- shallow copy
    Texture(const Texture &t) : _width(t._width), _height(t._height), _pixels(t._pixels)
    {
        std::cout << "Texture copy constructor" << std::endl;
    }

    /// @brief Destructor
    /// @details Deletes the texture
    ~Texture()
    {
        delete[] _pixels;
    }

    /// @brief Assignment operator
    /// @details Assigns the texture to the given texture
    Texture &operator=(const Texture &t)
    {
        _width = t._width;
        _height = t._height;
        delete[] _pixels;
        _pixels = new Color[_width * _height];
        for (int i = 0; i < _width * _height; i++)
        {
            _pixels[i] = t._pixels[i];
        }
        return *this;
    }

    /// @brief Gets the color at the given coordinates
    /// @details Gets the color at the given coordinates
    Color get(int x, int y) const
    {
        return _pixels[y * _width + x];
    }

    /// @brief Sets the color at the given coordinates
    /// @details Sets the color at the given coordinates
    void set(int x, int y, const Color &c)
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
        {
            return;
        }
        _pixels[y * _width + x] = c;
    }

    /// @brief Blanks the texture
    /// @details Blanks the texture
    void blank(const Color &c = Color())
    {
        // Set the memory to the color
        // Colors are the same size as ints (4 bytes)
        int colorAsInt = *((int *)&c);
        memset(_pixels, colorAsInt, _width * _height * sizeof(Color));
    }

    /// @brief Gets the width of the texture
    /// @details Gets the width of the texture
    int getWidth() const
    {
        return _width;
    }

    /// @brief Gets the height of the texture
    /// @details Gets the height of the texture
    int getHeight() const
    {
        return _height;
    }

    Vec getTopLeft() const
    {
        return Vec(0, 0, 0, 0);
    }

    Vec getTopRight() const
    {
        return Vec(_width, 0, 0, 0);
    }

    Vec getBottomLeft() const
    {
        return Vec(0, _height, 0, 0);
    }

    Vec getBottomRight() const
    {
        return Vec(_width, _height, 0, 0);
    }

    /// @brief Converts the texture to a string
    /// @details Converts the texture to a string
    std::string toString() const
    {
        std::stringstream ss;
        ss << "Texture(" << _width << ", " << _height << ")";
        for (int y = 0; y < _height; y++)
        {
            ss << std::endl;
            for (int x = 0; x < _width; x++)
            {
                ss << get(x, y).toString() << " ";
            }
        }

        return ss.str();
    }

private:
    int _width, _height;
    Color *_pixels;
};

/// @brief A class that is responsible for drawing on a texture
class TextureDrawer
{
public:
    /// @brief Default constructors
    TextureDrawer() : _texture(std::shared_ptr<Texture>()) {}
    TextureDrawer(std::shared_ptr<Texture> texture) : _texture(texture) {}

    /// @brief Draws a line on the texture
    /// @details Draws a line on the texture
    /// @param x1 The x coordinate of the first point
    /// @param y1 The y coordinate of the first point
    /// @param x2 The x coordinate of the second point
    /// @param y2 The y coordinate of the second point
    /// @param c The color of the line
    void drawLine(int x1, int y1, int x2, int y2, const Color &c)
    {
        // Bresenham's line algorithm
        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;
        while (true)
        {
            _texture->set(x1, y1, c);
            if (x1 == x2 && y1 == y2)
            {
                break;
            }
            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y1 += sy;
            }
        }
    }

    /// @brief Draws a line on the texture
    /// @details Draws a line on the texture
    /// @param p1 The first point
    /// @param p2 The second point
    /// @param c The color of the line
    void drawLine(const Vec &p1, const Vec &p2, const Color &c)
    {
        drawLine(p1.x, p1.y, p2.x, p2.y, c);
    }

    /// @brief Draws a triangle outline on the texture
    /// @details Draws a triangle outline on the texture
    /// @param p1 The first point
    /// @param p2 The second point
    /// @param p3 The third point
    /// @param c The color of the triangle outline
    void drawTriangle(const Vec &p1, const Vec &p2, const Vec &p3, const Color &c)
    {
        drawLine(p1, p2, c);
        drawLine(p2, p3, c);
        drawLine(p3, p1, c);
    }

    /// @brief Draws a filled triangle on the texture
    /// @details Draws a filled triangle on the texture
    /// @param p1 The first point
    /// @param p2 The second point
    /// @param p3 The third point
    /// @param c The color of the triangle
    void fillTriangle(const Vec &p1, const Vec &p2, const Vec &p3, const Color &c)
    {
        // order the points by y
        Vec orderedPoints[3] = {p1, p2, p3};
        // bubble sort -- good enough for 3 elements
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 2 - i; j++)
            {
                if (orderedPoints[j].y > orderedPoints[j + 1].y)
                {
                    Vec temp = orderedPoints[j];
                    orderedPoints[j] = orderedPoints[j + 1];
                    orderedPoints[j + 1] = temp;
                }
            }
        }

        // now draw the horizontal lines that comprise this triangle, from top to bottom
        Vec top = orderedPoints[0];
        Vec middle = orderedPoints[1];
        Vec bottom = orderedPoints[2];

        // calculate the slopes of the lines
        float topToMiddleSlope = (middle.x - top.x) / (middle.y - top.y);
        float topToBottomSlope = (bottom.x - top.x) / (bottom.y - top.y);
        float middleToBottomSlope = (bottom.x - middle.x) / (bottom.y - middle.y);

        // draw the top half of the triangle
        for (int y = top.y; y < middle.y; y++)
        {
            int x1 = top.x + (y - top.y) * topToMiddleSlope;
            int x2 = top.x + (y - top.y) * topToBottomSlope;
            drawLine(x1, y, x2, y, c);
        }

        // draw the bottom half of the triangle
        for (int y = middle.y; y < bottom.y; y++)
        {
            int x1 = middle.x + (y - middle.y) * middleToBottomSlope;
            int x2 = top.x + (y - top.y) * topToBottomSlope;
            drawLine(x1, y, x2, y, c);
        }
    }

    /// @brief Draws a circle outline on the Texture
    /// @details Draws a circle outline on the Texture
    /// @param x The x coordinate of the center of the circle
    /// @param y The y coordinate of the center of the circle
    /// @param r The radius of the circle
    /// @param c The color of the circle outline
    void drawCircle(int x, int y, float r, const Color &c)
    {
        // Bresenham's circle algorithm
        // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
        int f = 1 - r;
        int ddF_x = 1;
        int ddF_y = -2 * r;
        int cx = 0;
        int cy = r;

        _texture->set(x, y + r, c);
        _texture->set(x, y - r, c);
        _texture->set(x + r, y, c);
        _texture->set(x - r, y, c);

        while (cx < cy)
        {
            if (f >= 0)
            {
                cy--;
                ddF_y += 2;
                f += ddF_y;
            }
            cx++;
            ddF_x += 2;
            f += ddF_x;

            _texture->set(x + cx, y + cy, c);
            _texture->set(x - cx, y + cy, c);
            _texture->set(x + cx, y - cy, c);
            _texture->set(x - cx, y - cy, c);
            _texture->set(x + cy, y + cx, c);
            _texture->set(x - cy, y + cx, c);
            _texture->set(x + cy, y - cx, c);
            _texture->set(x - cy, y - cx, c);
        }
    }

    /// @brief Draws a circle outline on the Texture
    /// @details Draws a circle outline on the Texture
    /// @param p The center of the circle
    /// @param r The radius of the circle
    /// @param c The color of the circle outline
    void drawCircle(const Vec &p, float r, const Color &c)
    {
        drawCircle(p.x, p.y, r, c);
    }

    /// @brief Draws a filled circle on the Texture
    /// @details Draws a filled circle on the Texture
    /// @param x The x coordinate of the center of the circle
    /// @param y The y coordinate of the center of the circle
    /// @param r The radius of the circle
    /// @param c The color of the circle
    void fillCircle(int x, int y, float r, const Color &c)
    {
        // Bresenham's circle algorithm
        // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
        int f = 1 - r;
        int ddF_x = 1;
        int ddF_y = -2 * r;
        int cx = 0;
        int cy = r;

        // draw the horizontal lines
        for (int i = y - r; i <= y + r; i++)
        {
            _texture->set(x, i, c);
        }

        while (cx < cy)
        {
            if (f >= 0)
            {
                cy--;
                ddF_y += 2;
                f += ddF_y;
            }
            cx++;
            ddF_x += 2;
            f += ddF_x;

            // draw the horizontal lines
            for (int i = y - cy; i <= y + cy; i++)
            {
                _texture->set(x + cx, i, c);
                _texture->set(x - cx, i, c);
            }
            for (int i = y - cx; i <= y + cx; i++)
            {
                _texture->set(x + cy, i, c);
                _texture->set(x - cy, i, c);
            }
        }
    }

    /// @brief Draws a filled circle on the Texture
    /// @details Draws a filled circle on the Texture
    /// @param p The center of the circle
    /// @param r The radius of the circle
    /// @param c The color of the circle
    void fillCircle(const Vec &p, float r, const Color &c)
    {
        fillCircle(p.x, p.y, r, c);
    }

    /// @brief Fills a texture with a color
    /// @details Fills a texture with a color
    /// @param c The color to fill the texture with
    void fill(const Color &c)
    {
        this->_texture->blank(c);
    }

private:
    std::shared_ptr<Texture> _texture;
};

#endif // __TEX_H__