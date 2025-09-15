/**
 * @file MovableImplementations.hpp
 * @brief Кастомные реализации методов для IMovable
 * @author Anton Tobolkin
 * @version 1.0
 */

#pragma once

#include "IGameObject.hpp"
#include "Vector2D.hpp"
#include <memory>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Класс с кастомными реализациями методов для интерфейса IMovable
 * 
 * Содержит сложную логику, которая не может быть выражена простыми
 * геттерами/сеттерами и требует специальных вычислений.
 */
class MovableImplementations {
public:
    /**
     * @brief Кастомная реализация получения вектора скорости
     * 
     * Вычисляет вектор скорости на основе модуля скорости, направления
     * и общего количества направлений. Выполняет тригонометрические
     * расчеты для преобразования полярных координат в декартовы.
     * 
     * @param gameObject Игровой объект с необходимыми свойствами
     * @return Вектор скорости в декартовых координатах
     * @throws std::runtime_error при ошибках получения свойств или вычислений
     */
    static Vector2D getVelocity(std::shared_ptr<IGameObject> gameObject) {
        try {
            std::any velocityAny = gameObject->getProperty("Velocity");
            std::any directionAny = gameObject->getProperty("Direction");
            std::any directionsNumberAny = gameObject->getProperty("DirectionsNumber");

            int velocity = std::any_cast<int>(velocityAny);
            int direction = std::any_cast<int>(directionAny);
            int directionsNumber = std::any_cast<int>(directionsNumberAny);

            // Валидация входных данных
            if (directionsNumber <= 0) {
                throw std::runtime_error("DirectionsNumber must be positive");
            }

            if (direction < 0 || direction >= directionsNumber) {
                throw std::runtime_error("Direction must be between 0 and " +
                    std::to_string(directionsNumber - 1));
            }

            // Преобразуем дискретное направление в радианы
            // Направление 0 соответствует углу 0 радиан (направление вправо)
            double angleRadians = (2.0 * M_PI * direction) / directionsNumber;

            // Вычисляем компоненты скорости
            double vx_double = velocity * std::cos(angleRadians);
            double vy_double = velocity * std::sin(angleRadians);

            // Округляем до ближайшего целого числа для избежания ошибок округления
            int vx = static_cast<int>(std::round(vx_double));
            int vy = static_cast<int>(std::round(vy_double));

            return Vector2D(vx, vy);

        } catch (const std::exception& e) {
            throw std::runtime_error("Cannot calculate velocity: " + std::string(e.what()));
        }
    }

    /**
     * @brief Кастомная реализация для сложного позиционирования
     * 
     * Пример более сложной кастомной реализации, которая может
     * учитывать дополнительные факторы при установке позиции.
     * 
     * @param gameObject Игровой объект
     * @param position Желаемая позиция
     * @throws std::runtime_error при ошибках установки позиции
     */
    static void setPositionWithValidation(std::shared_ptr<IGameObject> gameObject, 
                                         const Vector2D& position) {
        try {
            // Пример валидации границ игрового поля
            if (position.x < -1000 || position.x > 1000 ||
                position.y < -1000 || position.y > 1000) {
                throw std::runtime_error("Position is out of game field bounds");
            }

            // Устанавливаем позицию
            gameObject->setProperty("Position", position);
            
            // Дополнительная логика: обновляем время последнего перемещения
            auto now = std::chrono::system_clock::now();
            auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                now.time_since_epoch()).count();
            gameObject->setProperty("LastMoveTime", timestamp);

        } catch (const std::exception& e) {
            throw std::runtime_error("Cannot set position: " + std::string(e.what()));
        }
    }
};