#pragma once
#include "IShape.h"

class Rectangle : public IShape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h);

    double area() const override;
    double perimeter() const override;
};