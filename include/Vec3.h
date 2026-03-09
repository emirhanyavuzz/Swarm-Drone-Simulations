#pragma once
#include <cmath>
#include <iostream>

struct Vec3 {
    double x, y, z;

    Vec3() : x(0.0), y(0.0), z(0.0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    // --- Arithmetic operators ---
    Vec3 operator+(const Vec3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vec3 operator-(const Vec3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vec3 operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vec3 operator/(double scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    Vec3& operator*=(double scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    // --- Length & normalization ---
    double length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    double lengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vec3 normalized() const {
        double len = length();
        if (len < 1e-9) return {0, 0, 0};
        return *this / len;
    }

    // --- Dot & cross products ---
    double dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 cross(const Vec3& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    // --- Utilities ---
    double distanceTo(const Vec3& other) const {
        return (*this - other).length();
    }

    static Vec3 lerp(const Vec3& a, const Vec3& b, double t) {
        return a + (b - a) * t;
    }

    static Vec3 zero() { return {0, 0, 0}; }

    // --- Stream output ---
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
};

// Scalar * Vec3
inline Vec3 operator*(double scalar, const Vec3& v) {
    return v * scalar;
}
