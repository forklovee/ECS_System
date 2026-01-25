#pragma once

#include <cmath>
#include <concepts>

namespace Math
{

template<typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template<Arithmetic T>
struct Vector3
{
    T x{}, y{}, z{};

    Vector3() {};
    Vector3(T x, T y, T z)
        :x(x), y(y), z(z)
    {}

    template<Arithmetic U>
    Vector3(Vector3<U> other)
        :x(other.x), y(other.y), z(other.z) {}

    template<Arithmetic U>
    Vector3<T> operator+(const Vector3<U>& rhs) const{
        return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    template<Arithmetic U>
    void operator+=(const Vector3<U>& rhs){
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }

    template<Arithmetic U>
    Vector3<T> operator-(const Vector3<U>& rhs) const{
        return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    template<Arithmetic U>
    void operator-=(const Vector3<U>& rhs){
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
    }

    template<Arithmetic U>
    Vector3<T> operator*(const Vector3<U>& rhs) const{
        return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    template<Arithmetic U>
    void operator*=(const Vector3<U>& rhs){
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
    }

    template<Arithmetic U>
    Vector3<T> operator*(const U& rhs) const{
        return Vector3(x * rhs, y * rhs, z * rhs);
    }

    template<Arithmetic U>
    Vector3<T> operator/(const Vector3<U>& rhs) const{
        return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    template<Arithmetic U>
    void operator/=(const Vector3<U>& rhs){
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
    }

    template<Arithmetic U>
    Vector3<T> operator/(const U& rhs) const{
        return Vector3(x / rhs, y / rhs, z / rhs);
    }

    float Length() const
    {
        return std::sqrt(x*x + y*y + z*z);
    }

    void Normalize()
    {
        const float length = Length();
        x /= length;
        y /= length;
        z /= length;
    }

    Vector3<T> GetNormalized() const
    {
        const float length = Length();
        return Vector3{x, y, z}/length;
    }

};

template<Arithmetic T>
struct Vector2
{
    T x{};
    T y{};

    Vector2() {};
    Vector2(T x, T y)
        :x(x), y(y)
    {}

    template<Arithmetic U>
    Vector2(Vector2<U> other)
        :x(other.x), y(other.y) {}

    template<Arithmetic U>
    Vector2<T> operator+(const Vector2<U>& rhs) const{
        return Vector2(x + rhs.x, y + rhs.y);
    }

    template<Arithmetic U>
    void operator+=(const Vector2<U>& rhs){
        x += rhs.x;
        y += rhs.y;
    }

    template<Arithmetic U>
    Vector2<T> operator-(const Vector2<U>& rhs) const{
        return Vector2(x - rhs.x, y - rhs.y);
    }

    template<Arithmetic U>
    void operator-=(const Vector2<U>& rhs){
        x -= rhs.x;
        y -= rhs.y;
    }

    template<Arithmetic U>
    Vector2<T> operator*(const Vector2<U>& rhs) const{
        return Vector2(x * rhs.x, y * rhs.y);
    }

    template<Arithmetic U>
    void operator*=(const Vector2<U>& rhs){
        x *= rhs.x;
        y *= rhs.y;
    }

    template<Arithmetic U>
    Vector2<T> operator*(const U& rhs) const{
        return Vector2(x * rhs, y * rhs);
    }

    template<Arithmetic U>
    Vector2<T> operator/(const Vector2<U>& rhs) const{
        return Vector2(x / rhs.x, y / rhs.y);
    }

    template<Arithmetic U>
    void operator/=(const Vector2<U>& rhs){
        x /= rhs.x;
        y /= rhs.y;
    }

    template<Arithmetic U>
    Vector2<T> operator/(const U& rhs) const{
        return Vector2(x / rhs, y / rhs);
    }

    float Length() const
    {
        return std::sqrt(x*x + y*y);
    }

    void Normalize()
    {
        const float length = Length();
        x /= length;
        y /= length;
    }

    Vector2<T> GetNormalized() const
    {
        const float length = Length();
        return Vector2{x, y}/length;
    }

};


}