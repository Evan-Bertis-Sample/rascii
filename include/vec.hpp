#ifndef __VEC_H__
#define __VEC_H__

// Header file for all things related to vectors
// Default implementation is 4D vectors

#include <string>
#include <sstream>
#include <math.h>

/// @brief A compact representation of a vector
/// @details A vector is represented by 4 floats, one for each dimension
struct Vec
{
    float x, y, z, w;

#pragma region Constructors
    /// @brief Default constructor
    /// @details Initializes the vector to the origin
    Vec() : x(0), y(0), z(0), w(0) {}

    /// @brief Constructor
    /// @details Initializes the vector to the given values
    Vec(float x, float y, float z, float w = 1.0f) : x(x), y(y), z(z), w(w) {}

    /// @brief Copy constructor
    /// @details Initializes the vector to the given vector
    Vec(const Vec &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
#pragma endregion

#pragma region Static Vec Methods
    // Direction Functions

    static Vec zero()
    {
        return Vec(0, 0, 0, 0);
    }

    static Vec one()
    {
        return Vec(1, 1, 1, 1);
    }

    static Vec up()
    {
        return Vec(0, 1, 0, 0);
    }

    static Vec down()
    {
        return Vec(0, -1, 0, 0);
    }

    static Vec left()
    {
        return Vec(-1, 0, 0, 0);
    }

    static Vec right()
    {
        return Vec(1, 0, 0, 0);
    }

    static Vec forward()
    {
        return Vec(0, 0, 1, 0);
    }

    static Vec backward()
    {
        return Vec(0, 0, -1, 0);
    }

#pragma endregion

#pragma region Vec Methods
    /// @brief Converts a sequence of floats into a vector
    /// @details Converts a sequence of floats into a vector -- the default values are 0
    static Vec toVec(float x = 0, float y = 0, float z = 0, float w = 0)
    {
        return Vec(x, y, z, w);
    }

    static Vec interpolate(Vec v1, Vec v2, float t)
    {
        return v1 + (v2 - v1) * t;
    }

    /// @brief Returns the element at the given index
    /// @details Returns the element at the given index
    /// @param index The index of the element
    float at(int index) const
    {
        if (index < 0 || index > 3)
        {
            throw std::runtime_error("Index out of bounds");
        }
        return (&this->x)[index];
    }

    /// @brief Returns the length of this vector
    float length() const
    {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
    }

    /// @brief Returns the squared length of this vector
    float lengthSquared() const
    {
        return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
    }

    /// @brief Returns the dot product of this vector and the given vector
    float dot(const Vec &v) const
    {
        return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
    }

    /// @brief Returns the cross product of this vector and the given vector
    Vec cross(const Vec &v) const
    {
        return Vec(
            this->y * v.z - this->z * v.y,
            this->z * v.x - this->x * v.z,
            this->x * v.y - this->y * v.x,
            0.0f);
    }

    /// @brief Returns the normalized version of this vector
    Vec normalized() const
    {
        return *this / this->length();
    }

    /// @brief Normalizes this vector
    void normalizeSelf()
    {
        *this /= this->length();
    }

    /// @brief Returns a 2D vector with the x and y components of this vector (the other components are zero)
    Vec xy() const
    {
        return Vec(this->x, this->y, 0, 0);
    }

    /// @brief Returns a 3D vector with the x, y, and z components of this vector (the other component is zero)
    Vec xyz() const
    {
        return Vec(this->x, this->y, this->z, 0);
    }

    /// @brief Returns a string representation of this vector
    std::string toString() const
    {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
        return ss.str();
    }

#pragma endregion

#pragma region Operation Overloads
    /// @brief Assignment operator
    /// @details Assigns the vector to the given vector
    Vec &operator=(const Vec &v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
        return *this;
    }

    /// @brief Index operator
    /// @details Returns the element at the given index
    float operator[](int index) const
    {
        return this->at(index);
    }

    /// @brief Index operator
    /// @details Returns the element at the given index
    float &operator[](int index)
    {
        if (index < 0 || index > 3)
        {
            throw std::runtime_error("Index out of bounds");
        }
        return (&this->x)[index];
    }

    /// @brief Addition operator
    /// @details Adds the given vector to this vector
    Vec operator+(const Vec &v) const
    {
        return Vec(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
    }

    /// @brief Subtraction operator
    /// @details Subtracts the given vector from this vector
    Vec operator-(const Vec &v) const
    {
        return Vec(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
    }

    /// @brief Multiplication operator
    /// @details Multiplies this vector by the given scalar
    Vec operator*(float scalar) const
    {
        return Vec(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
    }

    /// @brief Multiplication operator
    /// @details Multiplies this vector by the given vector element-wise
    Vec operator*(Vec v) const
    {
        return Vec(this->x * v.x, this->y * v.y, this->z * v.z, this->w * v.w);
    }

    /// @brief Division operator
    /// @details Divides this vector by the given scalar
    Vec operator/(float scalar) const
    {
        return Vec(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar);
    }

    /// @brief Division operator
    /// @details Divides this vector by the given vector element-wise
    Vec operator/(Vec v) const
    {
        return Vec(this->x / v.x, this->y / v.y, this->z / v.z, this->w / v.w);
    }

    /// @brief Addition assignment operator
    /// @details Adds the given vector to this vector
    Vec &operator+=(const Vec &v)
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        this->w += v.w;
        return *this;
    }

    /// @brief Subtraction assignment operator
    /// @details Subtracts the given vector from this vector
    Vec &operator-=(const Vec &v)
    {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        this->w -= v.w;
        return *this;
    }

    /// @brief Multiplication assignment operator
    /// @details Multiplies this vector by the given scalar
    Vec &operator*=(float scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        this->w *= scalar;
        return *this;
    }

    /// @brief Multiplication assignment operator
    /// @details Multiplies this vector by the given vector element-wise
    Vec &operator*=(Vec v)
    {
        this->x *= v.x;
        this->y *= v.y;
        this->z *= v.z;
        this->w *= v.w;
        return *this;
    }

    /// @brief Division assignment operator
    /// @details Divides this vector by the given scalar
    Vec &operator/=(float scalar)
    {
        this->x /= scalar;
        this->y /= scalar;
        this->z /= scalar;
        this->w /= scalar;
        return *this;
    }

    /// @brief Division assignment operator
    /// @details Divides this vector by the given vector element-wise
    Vec &operator/=(Vec v)
    {
        this->x /= v.x;
        this->y /= v.y;
        this->z /= v.z;
        this->w /= v.w;
        return *this;
    }

    /// @brief Equality operator
    /// @details Checks if this vector is equal to the given vector
    bool operator==(const Vec &v) const
    {
        return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
    }

    /// @brief Inequality operator
    /// @details Checks if this vector is not equal to the given vector
    bool operator!=(const Vec &v) const
    {
        return !(*this == v);
    }

    /// @brief Negation operator
    /// @details Negates this vector
    Vec operator-() const
    {
        return Vec(-this->x, -this->y, -this->z, -this->w);
    }
#pragma endregion
};

#endif // __VEC_H__