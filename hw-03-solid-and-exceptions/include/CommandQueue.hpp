#pragma once

#include <queue>
#include <memory>
#include <map>
#include <typeindex>
#include <functional>
#include "ICommand.hpp"
#include "IExceptionHandler.hpp"

/**
 * @file CommandQueue.hpp
 * @brief ����� ������� ������ � ���������� ����������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ����� ��� ���������� �������� ������ � �� �����������
  *
  * ��������� ������� Command � ������������ ��������� ����������
  * ����� ��������� ��������� (Strategy pattern)
  */
class CommandQueue {
public:
    /**
     * @brief ��� ��� �������� ������������ ����������
     *
     * ���� - ���� (��� �������, ��� ����������)
     */
    using HandlerKey = std::pair<std::type_index, std::type_index>;

    /**
     * @brief ����������� �� ���������
     */
    CommandQueue();

    /**
     * @brief ����������
     */
    ~CommandQueue() = default;

    /**
     * @brief �������� ������� � �������
     * @param command ������� ��� ����������
     */
    void push(std::shared_ptr<ICommand> command);

    /**
     * @brief ���������� ���� ������� �� �������
     * @return true ���� ������� ���� ����������, false ���� ������� �����
     */
    bool processOne();

    /**
     * @brief ���������� ��� ������� � �������
     */
    void processAll();

    /**
     * @brief ���������, ����� �� �������
     * @return true ���� ������� �����
     */
    bool isEmpty() const;

    /**
     * @brief �������� ������ �������
     * @return ���������� ������ � �������
     */
    size_t size() const;

    /**
     * @brief ���������������� ���������� ����������
     * @tparam CommandType ��� �������
     * @tparam ExceptionType ��� ����������
     * @param handler ���������� ����������
     */
    template<typename CommandType, typename ExceptionType>
    void registerHandler(std::shared_ptr<IExceptionHandler> handler) {
        HandlerKey key(std::type_index(typeid(CommandType)),
            std::type_index(typeid(ExceptionType)));
        handlers_[key] = handler;
    }

    /**
     * @brief ���������� ���������� �� ���������
     * @param handler ���������� ���������� �� ���������
     */
    void setDefaultHandler(std::shared_ptr<IExceptionHandler> handler);

private:
    /**
     * @brief ������� ������
     */
    std::queue<std::shared_ptr<ICommand>> commands_;

    /**
     * @brief ����� ������������ ����������
     */
    std::map<HandlerKey, std::shared_ptr<IExceptionHandler>> handlers_;

    /**
     * @brief ���������� ���������� �� ���������
     */
    std::shared_ptr<IExceptionHandler> defaultHandler_;

    /**
     * @brief ����� ���������� ���������� ��� ����������
     * @param command �������, ����������� ����������
     * @param exception ����������
     * @return ��������� �� ���������� ��� nullptr
     */
    std::shared_ptr<IExceptionHandler> findHandler(
        std::shared_ptr<ICommand> command,
        const std::exception& exception);
};