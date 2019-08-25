#include "Vector2D.h"

#include <cmath>

using namespace uw;

Vector2D::Vector2D(const double& x, const double& y) :
    _x(x),
    _y(y)
{}

double& Vector2D::x()
{
    return _x;
}

const double uw::Vector2D::x() const
{
    return _x;
}

void Vector2D::x(const double& newX)
{
    _x = newX;
}

double& Vector2D::y()
{
    return _y;
}

const double uw::Vector2D::y() const
{
    return _y;
}

void Vector2D::y(const double& newY)
{
    _y = newY;
}

const double Vector2D::distance(const Vector2D& other) const
{
    return sqrt(distanceSq(other));
}

const double Vector2D::distanceSq(const Vector2D& other) const
{
    return (_x - other._x) * (_x - other._x) + (_y - other._y) * (_y - other._y);
}

void Vector2D::operator+=(const Vector2D& other)
{
    _x += other._x;
    _y += other._y;
}

Vector2D Vector2D::operator+(const Vector2D& other) const
{
    return Vector2D(_x + other._x, _y + other._y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const
{
    return Vector2D(_x - other._x, _y - other._y);
}

bool Vector2D::operator==(const Vector2D& other) const
{
    return _x == other._x && _y == other._y;
}

const double uw::Vector2D::module() const
{
    return sqrt(moduleSq());
}

const double uw::Vector2D::moduleSq() const
{
    return _x * _x + _y * _y;
}

Vector2D& uw::Vector2D::maxAt(const double & maxModule)
{
    if (_x * _x + _y * _y > maxModule * maxModule)
    {
        atModule(maxModule);
    }
    return *this;
}

Vector2D& uw::Vector2D::atModule(const double & module)
{
    if (module <= 0)
    {
        _x = 0;
        _y = 0;
    }
    else
    {
        const auto modifier = sqrt((_x * _x + _y * _y) / (module * module));
        _x /= modifier;
        _y /= modifier;
    }
    return *this;
}

Vector2D Vector2D::divide(const double& divided, const double& max)
{
    auto xValue = _x == 0 ? max : divided / _x;
    if (abs(xValue) > max)
    {
        xValue *= max / abs(xValue);
    }
    auto yValue = _y == 0 ? max : divided / _y;
    if (abs(xValue) > max)
    {
        xValue *= max / abs(xValue);
    }
    return Vector2D(xValue, yValue);
}