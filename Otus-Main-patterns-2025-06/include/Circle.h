#pragma once
#include "IShape.h"

class Circle : public IShape {
private:
    double radius;

public:
    Circle(double r);

    double area() const override;
    double perimeter() const override;
};