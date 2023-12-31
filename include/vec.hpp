#ifndef __VEC_H__
#define __VEC_H__

// Header files for all things related to vectors
// Default implementation is 4D vectors

/// @brief A compact representation of a vector
/// @details A vector is represented by 4 floats, one for each dimension
struct Vec {
    float x, y, z, w;

    /// @brief Default constructor
    /// @details Initializes the vector to the origin
    Vec() : x(0), y(0), z(0), w(0) {}

    /// @brief Constructor
    /// @details Initializes the vector to the given values
    Vec(float x, float y, float z, float w = 1.0f) : x(x), y(y), z(z), w(w) {}
};

#endif // __VEC_H__