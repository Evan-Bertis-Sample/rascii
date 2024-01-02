#ifndef __MATRIX_H__
#define __MATRIX_H__

// Header file for all things related to 4x4 matrices
// Default implementation is 4x4 matrices

#include <string>
#include <sstream>
#include <math.h>
#include <cstring>

#include "vec.hpp"

/// @brief A compact representation of a 4x4 matrix
/// @details A matrix is represented by 16 floats, one for each element
struct Matrix
{
    float elements[16];
#pragma region Constructors
    /// @brief Default constructor
    /// @details Initializes the matrix to the identity matrix
    Matrix()
    {
        memset(this->elements, 0, sizeof(float) * 16);
        this->elements[0] = 1;
        this->elements[5] = 1;
        this->elements[10] = 1;
        this->elements[15] = 1;
    }

    /// @brief Constructor
    /// @details Initializes the matrix to the given values
    Matrix(float elements[16])
    {
        memcpy(this->elements, elements, sizeof(float) * 16);
    }

    /// @brief Copy constructor
    /// @details Initializes the matrix to the given matrix
    Matrix(const Matrix &m)
    {
        memcpy(this->elements, m.elements, sizeof(float) * 16);
    }
#pragma endregion

#pragma region Matrix Methods
    /// @brief Gets the element at the given row and column
    /// @details Returns the element at the given row and column
    /// @param row The row of the element
    /// @param col The column of the element
    float at(int row, int col) const
    {
        return this->elements[row * 4 + col];
    }

    /// @brief Sets the element at the given row and column
    /// @details Sets the element at the given row and column to the given value
    /// @param row The row of the element
    /// @param col The column of the element
    /// @param val The value to set the element to
    float set(int row, int col, float val)
    {
        this->elements[row * 4 + col] = val;
    }

    /// @brief Set all the elements of the matrix to the given value
    /// @details Sets all the elements of the matrix to the given value
    /// @param val The value to set the elements to
    void setAll(float val)
    {
        memset(this->elements, val, sizeof(float) * 16);
    }

    /// @brief Returns the transpose of this matrix
    Matrix transpose() const
    {
        Matrix result;
        for (int row = 0; row < 4; row++)
        {
            for (int col = row; col < 4; col++)
            {
                result.set(row, col, this->at(col, row));
                result.set(col, row, this->at(row, col));
            }
        }
        return result;
    }

    void transposeSelf()
    {
        Matrix result = this->transpose();
        *this = result;
    }

    /// @brief Returns the determinant of this matrix
    float determinant() const
    {
        float det = 0;
        for (int i = 0; i < 4; i++)
        {
            det += this->at(0, i) * this->cofactor(0, i);
        }
        return det;
    }

    /// @brief Returns the cofactor of the element at the given row and column
    float cofactor(int row, int col) const
    {
        return pow(-1, row + col) * this->minor(row, col);
    }

    /// @brief Returns the minor of the element at the given row and column
    float minor(int row, int col) const
    {
        Matrix m = this->submatrix(row, col);
        return m.determinant();
    }

    /// @brief Returns a submatrix of this matrix
    /// @details Returns a submatrix of this matrix with the given row and column removed
    /// @param row The row to remove
    /// @param col The column to remove
    Matrix submatrix(int row, int col) const
    {
        Matrix result;
        int resultRow = 0;
        int resultCol = 0;
        for (int i = 0; i < 4; i++)
        {
            if (i == row)
                continue;
            for (int j = 0; j < 4; j++)
            {
                if (j == col)
                    continue;
                result.set(resultRow, resultCol, this->at(i, j));
                resultCol++;
            }
            resultRow++;
            resultCol = 0;
        }
        return result;
    }

    /// @brief Returns the inverse of this matrix
    Matrix inverse() const
    {
        Matrix result;
        float det = this->determinant();
        if (det == 0)
            throw std::runtime_error("Matrix is not invertible");
        for (int row = 0; row < 4; row++)
        {
            for (int col = row; col < 4; col++)
            {
                result.set(row, col, this->cofactor(row, col) / det);
                result.set(col, row, this->cofactor(col, row) / det);
            }
        }
        return result;
    }

    void inverseSelf()
    {
        Matrix result = this->inverse();
        *this = result;
    }

    /// @brief Returns a string representation of this matrix
    std::string toString() const
    {
        std::stringstream ss;
        ss << "Matrix(" << std::endl;
        for (int row = 0; row < 4; row++)
        {
            ss << "  ";
            for (int col = 0; col < 4; col++)
            {
                ss << this->at(row, col) << " ";
            }
            ss << std::endl;
        }
        ss << ")";
        return ss.str();
    }

#pragma endregion

#pragma region Matrix Operations
    /// @brief Assignment operator
    /// @details Assigns the matrix to the given matrix
    Matrix &operator=(const Matrix &m)
    {
        memcpy(this->elements, m.elements, sizeof(float) * 16);
        return *this;
    }

    /// @brief Addition operator
    /// @details Returns the sum of this matrix and the given matrix
    Matrix operator+(const Matrix &m) const
    {
        Matrix result;
        for (int i = 0; i < 16; i++)
        {
            result.elements[i] = this->elements[i] + m.elements[i];
        }
        return result;
    }

    /// @brief Subtraction operator
    /// @details Returns the difference of this matrix and the given matrix
    Matrix operator-(const Matrix &m) const
    {
        Matrix result;
        for (int i = 0; i < 16; i++)
        {
            result.elements[i] = this->elements[i] - m.elements[i];
        }
        return result;
    }

    /// @brief Multiplication operator
    /// @details Returns the product of this matrix and the given matrix
    Matrix operator*(const Matrix &m) const
    {
        Matrix result;
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {

                float sum = 0;
                for (int i = 0; i < 4; i++)
                {
                    sum += this->at(row, i) * m.at(i, col);
                }
                result.set(row, col, sum);
            }
        }
        return result;
    }

    /// @brief Multiplication operator
    /// @details Returns the product of this matrix and the given vector (as a column vector)
    /// @param v The vector to multiply by
    Vec operator*(const Vec &v) const
    {
        Vec result;
        for (int row = 0; row < 4; row++)
        {
            float sum = 0;
            for (int i = 0; i < 4; i++)
            {
                sum += this->at(row, i) * v[i];
            }
            result[row] = sum;
        }
        return result;
    }

    /// @brief Multiplication operator
    /// @details Returns the product of this matrix and the given scalar
    Matrix operator*(float scalar) const
    {
        Matrix result;
        for (int i = 0; i < 16; i++)
        {
            result.elements[i] = this->elements[i] * scalar;
        }
        return result;
    }

    /// @brief Division operator
    /// @details Returns the quotient of this matrix and the given scalar
    Matrix operator/(float scalar) const
    {
        Matrix result;
        for (int i = 0; i < 16; i++)
        {
            result.elements[i] = this->elements[i] / scalar;
        }
        return result;
    }

    /// @brief Equality operator
    /// @details Returns true if this matrix is equal to the given matrix
    bool operator==(const Matrix &m) const
    {
        for (int i = 0; i < 16; i++)
        {
            if (this->elements[i] != m.elements[i])
                return false;
        }
        return true;
    }

    /// @brief Inequality operator
    /// @details Returns true if this matrix is not equal to the given matrix
    bool operator!=(const Matrix &m) const
    {
        return !(*this == m);
    }

    /// @brief Addition assignment operator
    /// @details Adds the given matrix to this matrix
    Matrix &operator+=(const Matrix &m)
    {
        for (int i = 0; i < 16; i++)
        {
            this->elements[i] += m.elements[i];
        }
        return *this;
    }

    /// @brief Subtraction assignment operator
    /// @details Subtracts the given matrix from this matrix
    Matrix &operator-=(const Matrix &m)
    {
        for (int i = 0; i < 16; i++)
        {
            this->elements[i] -= m.elements[i];
        }
        return *this;
    }

    /// @brief Multiplication assignment operator
    /// @details Multiplies this matrix by the given matrix
    Matrix &operator*=(const Matrix &m)
    {
        Matrix result = *this * m;
        *this = result;
        return *this;
    }

    /// @brief Multiplication assignment operator
    /// @details Multiplies this matrix by the given scalar
    Matrix &operator*=(float scalar)
    {
        for (int i = 0; i < 16; i++)
        {
            this->elements[i] *= scalar;
        }
        return *this;
    }

    /// @brief Division assignment operator
    /// @details Divides this matrix by the given scalar
    Matrix &operator/=(float scalar)
    {
        for (int i = 0; i < 16; i++)
        {
            this->elements[i] /= scalar;
        }
        return *this;
    }

#pragma endregion
};

#endif // __MATRIX_H__