#include <iostream>
#include "Rectangle.h"
#include "Circle.h"

void printShapeInfo(const IShape& shape) {
    std::cout << "�������: " << shape.area()
        << ", ��������: " << shape.perimeter() << std::endl;
}

int main() {
    Rectangle rect(4.0, 5.0);
    Circle circle(3.0);

    std::cout << "�������������:\n";
    printShapeInfo(rect);

    std::cout << "\n����:\n";
    printShapeInfo(circle);

    return 0;
}