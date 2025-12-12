#pragma once

#include "OrderInterpreter.hpp"
#include <string>

/**
 * @file SecureOrderInterpreter.hpp
 * @brief Защищенный интерпретатор с проверкой доступа через скоупы
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Защищенный интерпретатор с изоляцией через скоупы IoC
 * 
 * Каждый игрок работает в своем изолированном скоупе.
 * Объекты регистрируются в скоупе владельца.
 * При попытке управления чужим объектом - исключение.
 * 
 * Схема изоляции:
 * @code
 * ROOT SCOPE
 *   ├── Commands.StartMove, Commands.Fire и т.д. (глобальные)
 *   ├── Player.Alice Scope
 *   │   └── Objects.ship_001, Objects.ship_002
 *   └── Player.Bob Scope
 *       └── Objects.ship_003, Objects.ship_004
 * @endcode
 * 
 * Alice может управлять только ship_001 и ship_002.
 * Bob может управлять только ship_003 и ship_004.
 * Попытка управлять чужим кораблем приведет к исключению.
 */
class SecureOrderInterpreter : public OrderInterpreter {
public:
    /**
     * @brief Конструктор
     * 
     * @param playerId Идентификатор игрока
     */
    explicit SecureOrderInterpreter(const std::string& playerId);
    
    /**
     * @brief Виртуальный деструктор
     */
    ~SecureOrderInterpreter() override = default;
    
    /**
     * @brief Интерпретирует приказ с проверкой доступа
     * 
     * Перед выполнением устанавливает скоуп текущего игрока.
     * Объект будет найден только если он принадлежит игроку.
     * 
     * @param order Приказ для выполнения
     * @throws std::runtime_error если объект не принадлежит игроку
     */
    void interpret(const Order& order) override;
    
    /**
     * @brief Получить ID игрока
     * @return Идентификатор игрока
     */
    const std::string& getPlayerId() const { return playerId_; }

protected:
    /**
     * @brief Разрешает объект в скоупе игрока
     * 
     * Переопределяет базовый метод для добавления проверки доступа.
     * Объект ищется только в скоупе текущего игрока.
     * 
     * @param objectId Идентификатор объекта
     * @return Указатель на объект
     * @throws std::runtime_error если объект не принадлежит игроку
     */
    std::shared_ptr<IUObject> resolveObject(const std::string& objectId) override;

private:
    /**
     * @brief Устанавливает скоуп текущего игрока
     * 
     * Вызывает "Scopes.Current" с ID скоупа игрока.
     */
    void setPlayerScope();
    
    /**
     * @brief Формирует ID скоупа для игрока
     * @return Строка вида "Player.{playerId}"
     */
    std::string getScopeId() const;

    std::string playerId_;  ///< Идентификатор игрока
};