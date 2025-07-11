#pragma once

class IShape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    virtual ~IShape() = default;
};