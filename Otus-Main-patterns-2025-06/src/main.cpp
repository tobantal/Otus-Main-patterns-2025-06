#include <iostream>
#include "Rectangle.h"
#include "Circle.h"

void printShapeInfo(const IShape& shape) {
    std::cout << "Площадь: " << shape.area()
        << ", Периметр: " << shape.perimeter() << std::endl;
}

int main() {
    Rectangle rect(4.0, 5.0);
    Circle circle(3.0);

    std::cout << "Прямоугольник:\n";
    printShapeInfo(rect);

    std::cout << "\nКруг:\n";
    printShapeInfo(circle);

    return 0;
}