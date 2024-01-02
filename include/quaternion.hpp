#ifndef __QUATERNION_H__
#define __QUATERNION_H__

// Header file for all things related to quaternions

#include <iostream>
#include <string>
#include <sstream>

#include "vec.hpp"
#include "matrix.hpp"

/// @brief A compact representation of a quaternion
/// @details A quaternion is represented by 4 floats, one for each dimension
struct Quaternion
{
    float x, y, z, w;

#pragma region Constructors
    /// @brief Default constructor
    /// @details Initializes the quaternion to the identity quaternion
    Quaternion() : x(0), y(0), z(0), w(1) {}

    /// @brief Constructor
    /// @details Initializes the quaternion to the given values
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    /// @brief Copy constructor
    /// @details Initializes the quaternion to the given quaternion
    Quaternion(const Quaternion &q) : x(q.x), y(q.y), z(q.z), w(q.w) {}

    /// @brief Constructor
    /// @details Initializes the quaternion from an axis angle representation
    /// @param axis The axis of rotation
    /// @param angle The angle of rotation
    Quaternion(const Vec &axis, float angle)
    {
        float halfAngle = angle / 2;
        float sinHalfAngle = sin(halfAngle);
        this->x = axis.x * sinHalfAngle;
        this->y = axis.y * sinHalfAngle;
        this->z = axis.z * sinHalfAngle;
        this->w = cos(halfAngle);
    }

    /// @brief Constructor
    /// @details Initializes the quaternion from an Euler angle representation
    /// @param pitch The pitch of the rotation
    /// @param yaw The yaw of the rotation
    /// @param roll The roll of the rotation
    Quaternion(float pitch, float yaw, float roll)
    {
        float halfPitch = pitch / 2;
        float halfYaw = yaw / 2;
        float halfRoll = roll / 2;
        float sinHalfPitch = sin(halfPitch);
        float sinHalfYaw = sin(halfYaw);
        float sinHalfRoll = sin(halfRoll);
        float cosHalfPitch = cos(halfPitch);
        float cosHalfYaw = cos(halfYaw);
        float cosHalfRoll = cos(halfRoll);
        this->x = sinHalfRoll * cosHalfPitch * cosHalfYaw - cosHalfRoll * sinHalfPitch * sinHalfYaw;
        this->y = cosHalfRoll * sinHalfPitch * cosHalfYaw + sinHalfRoll * cosHalfPitch * sinHalfYaw;
        this->z = cosHalfRoll * cosHalfPitch * sinHalfYaw - sinHalfRoll * sinHalfPitch * cosHalfYaw;
        this->w = cosHalfRoll * cosHalfPitch * cosHalfYaw + sinHalfRoll * sinHalfPitch * sinHalfYaw;
    }

#pragma endregion

#pragma region Static Quaternion Methods
    // Direction Functions

    static Quaternion identity()
    {
        return Quaternion(0, 0, 0, 1);
    }

    // Conversion Functions
    static Quaternion fromAxisAngle(const Vec &axis, float angle)
    {
        return Quaternion(axis, angle);
    }

    static Quaternion fromEuler(float pitch, float yaw, float roll)
    {
        return Quaternion(pitch, yaw, roll);
    }
#pragma endregion

#pragma region Quaternion Methods
    /// @brief Constructs a rotation matrix from the quaternion
    /// @return A 4x4 Rotation matrix
    Matrix toRotationMatrix() const
    {
    }
#pragma endregion
};

#endif // __QUATERNION_H__