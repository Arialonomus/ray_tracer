#include "vector4.hpp"

#include <ranges>
#include <stdexcept>
#include <cmath>

#include "util_functions.hpp"

namespace gfx {
    // Span Constructor
    Vector4::Vector4(std::span<const float, 4> values)
            : m_data{}
    {
        std::copy(values.begin(), values.end(), m_data.begin());
    }

    // Comparison Operator
    bool Vector4::operator==(const Vector4& rhs) const
    {
        return utils::areEqual(m_data[0], rhs.x())
               && utils::areEqual(m_data[1], rhs.y())
               && utils::areEqual(m_data[2], rhs.z())
               && utils::areEqual(m_data[3], rhs.w());
    }

    // Scalar Division Operator
    Vector4 Vector4::operator/(const float scalar) const
    {
        if (scalar == 0.0) {
            throw std::invalid_argument{ "Divide by zero." };
        }
        return Vector4{ m_data[0] / scalar, m_data[1] / scalar, m_data[2] / scalar, m_data[3] / scalar };
    }

    // Unary Negation Operator
    Vector4 Vector4::operator-() const
    {
        return Vector4{ -m_data[0], -m_data[1], -m_data[2], -m_data[3] };
    }

    // Vector Addition Shorthand Operator
    Vector4& Vector4::operator+=(const Vector4& rhs)
    {
        m_data[0] += rhs.x();
        m_data[1] += rhs.y();
        m_data[2] += rhs.z();
        m_data[3] += rhs.w();

        if (m_data[3] > 1) {
            throw std::invalid_argument{ "Cannot add two points" };
        }

        return *this;
    }

    // Vector Subtraction Shorthand Operator
    Vector4& Vector4::operator-=(const Vector4& rhs)
    {
        m_data[0] -= rhs.x();
        m_data[1] -= rhs.y();
        m_data[2] -= rhs.z();
        m_data[3] -= rhs.w();

        return *this;
    }

    // Scalar Multiplication Shorthand Operator
    Vector4& Vector4::operator*=(const float scalar)
    {
        m_data[0] *= scalar;
        m_data[1] *= scalar;
        m_data[2] *= scalar;
        m_data[3] *= scalar;

        return *this;
    }

    // Scalar Division Shorthand Operator
    Vector4& Vector4::operator/=(const float scalar)
    {
        if (scalar == 0.0) {
            throw std::invalid_argument{ "Divide by zero." };
        }

        m_data[0] /= scalar;
        m_data[1] /= scalar;
        m_data[2] /= scalar;
        m_data[3] /= scalar;

        return *this;
    }

    // Matrix-Vector Multiplication Shorthand Operator
    Vector4& Vector4::operator*=(const Matrix4& rhs)
    {
        // Populate the array with the dot product of each row with the vector
        std::array<float, 4> vector_values{};
        for (int row = 0; row < 4; ++row) {
            vector_values[row] =
                    m_data[0] * rhs[row, 0] +
                    m_data[1] * rhs[row, 1] +
                    m_data[2] * rhs[row, 2] +
                    m_data[3] * rhs[row, 3];
        }

        // Assign the new values and return
        m_data = vector_values;
        return *this;
    }

    // Vector Magnitude
    float Vector4::magnitude() const
    {
        return std::sqrt(
                std::pow(m_data[0], 2) + std::pow(m_data[1], 2) + std::pow(m_data[2], 2) + std::pow(m_data[3], 2));
    }

    // Vector Cross Product
    Vector4 Vector4::crossProduct(const Vector4& rhs) const
    {
        return Vector4{ m_data[1] * rhs.z() - m_data[2] * rhs.y(), m_data[2] * rhs.x() - m_data[0] * rhs.z(),
                        m_data[0] * rhs.y() - m_data[1] * rhs.x(), 0.0 };
    }

    // Vector Factory Function
    Vector4 createVector(const float x, const float y, const float z)
    {
        return Vector4{ x, y, z, 0.0 };
    }

    // Point Factory Function
    Vector4 createPoint(const float x, const float y, const float z)
    {
        return Vector4{ x, y, z, 1.0 };
    }

    // Addition Operator
    Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
    {
        const float w_sum = lhs.w() + rhs.w();
        if (w_sum > 1) {
            throw std::invalid_argument{ "Cannot add two points" };
        }

        return Vector4{ lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z(), w_sum };
    }

    // Subtraction Operator
    Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4{ lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z(), lhs.w() - rhs.w() };
    }

    // Scalar Multiplication Operator (Vector Left-Hand)
    Vector4 operator*(const Vector4& lhs, const float rhs)
    {
        return Vector4{ lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs, lhs.w() * rhs };
    }

    // Scalar Multiplication Operator (Vector Right-Hand)
    Vector4 operator*(const float lhs, const Vector4& rhs)
    {
        return rhs * lhs;
    }

    // Matrix-Vector Multiplication Operator
    Vector4 operator*(const Matrix4& lhs, const Vector4& rhs)
    {
        std::array<float, 4> vector_values{};
        for (int row = 0; row < 4; ++row) {
            vector_values[row] =
                    lhs[row, 0] * rhs.x() +
                    lhs[row, 1] * rhs.y() +
                    lhs[row, 2] * rhs.z() +
                    lhs[row, 3] * rhs.w();
        }

        return Vector4{ vector_values };
    }

    // Normalize Vector
    Vector4 normalize(const Vector4& src)
    {
        const float magnitude = src.magnitude();
        return Vector4{ src.x() / magnitude, src.y() / magnitude, src.z() / magnitude, src.w() / magnitude };
    }

    // Vector Dot Product
    float dotProduct(const Vector4& lhs, const Vector4& rhs)
    {
        return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z() + lhs.w() * rhs.w();
    }
}