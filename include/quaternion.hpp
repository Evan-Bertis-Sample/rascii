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

    /// @brief Slerps between two quaternions
    /// @details Slerps between two quaternions
    
#pragma endregion

#pragma region Quaternion Methods
    /// @brief Constructs a 4x4 rotation matrix from the quaternion
    /// @return A 4x4 Rotation matrix
    Matrix toRotationMatrix() const
    {
        // starts off with the identity matrix
        Matrix rotationMatrix = Matrix();

        // precompute the values
        float x2 = this->x * this->x;
        float y2 = this->y * this->y;
        float z2 = this->z * this->z;
        float xy = this->x * this->y;
        float xz = this->x * this->z;
        float yz = this->y * this->z;
        float wx = this->w * this->x;
        float wy = this->w * this->y;
        float wz = this->w * this->z;

        // set the values
        rotationMatrix.set(0, 0, 1 - 2 * (y2 + z2));
        rotationMatrix.set(0, 1, 2 * (xy - wz));
        rotationMatrix.set(0, 2, 2 * (xz + wy));

        rotationMatrix.set(1, 0, 2 * (xy + wz));
        rotationMatrix.set(1, 1, 1 - 2 * (x2 + z2));
        rotationMatrix.set(1, 2, 2 * (yz - wx));

        rotationMatrix.set(2, 0, 2 * (xz - wy));
        rotationMatrix.set(2, 1, 2 * (yz + wx));
        rotationMatrix.set(2, 2, 1 - 2 * (x2 + y2));

        return rotationMatrix;
    }

    /// @brief Returns a string representation of this quaternion
    /// @details Returns a string representation of this quaternion
    std::string toString() const
    {
        std::stringstream ss;
        ss << "Quaternion(" << this->x << ", " << this->y << ", " << this->z << ", " << this->w << ")";
        return ss.str();
    }

    /// @brief Returns the magnitude of this quaternion
    /// @details Returns the magnitude of this quaternion
    float magnitude() const
    {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
    }

    /// @brief Returns the normalized version of this quaternion
    /// @details Returns the normalized version of this quaternion
    Quaternion normalized() const
    {
        float magnitude = this->magnitude();
        return Quaternion(this->x / magnitude, this->y / magnitude, this->z / magnitude, this->w / magnitude);
    }

    /// @brief Normalizes this Quaternion
    /// @details Normalizes this Quaternion
    void normalize()
    {
        float magnitude = this->magnitude();
        this->x /= magnitude;
        this->y /= magnitude;
        this->z /= magnitude;
        this->w /= magnitude;
    }    
    // TODO: Implement slerp, inverse, and overloads

    /// @brief Returns the inverse of this quaternion
    /// @details Returns the inverse of this quaternion
    Quaternion inverse() const
    {
        return Quaternion(-this->x, -this->y, -this->z, this->w);
    }    
#pragma endregion

#pragma region Quaternion overloads

    /// @brief Overload for the multiplication operator
    /// @param q The quaternion to multiply by
    /// @return The result of the multiplication
    Quaternion operator*(const Quaternion &q) const {
        return Quaternion(
            this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y,
            this->w * q.y + this->y * q.w + this->z * q.x - this->x * q.z,
            this->w * q.z + this->z * q.w + this->x * q.y - this->y * q.x,
            this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z
        );
    }

#pragma endregion
};

#endif // __QUATERNION_H__