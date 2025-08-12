#include "CommandQueue.hpp"
#include <iostream>

/**
 * @brief ����������� �� ���������
 */
CommandQueue::CommandQueue() : defaultHandler_(nullptr) {
}

/**
 * @brief �������� ������� � �������
 */
void CommandQueue::push(std::shared_ptr<ICommand> command) {
    if (command) {
        commands_.push(command);
    }
}

/**
 * @brief ���������� ���� ������� �� �������
 */
bool CommandQueue::processOne() {
    if (commands_.empty()) {
        return false;
    }

	// ��������� ������ ������� �� �������
    auto command = commands_.front();
    // ������� ������� �� �������
	commands_.pop();

    try {
        // ��������� ������� � ����� try-catch
        command->execute();
    }
    catch (const std::exception& e) {
        // ������������� ������ ������� ����������
        // ������� ���������� ����������
        auto handler = findHandler(command, e);

        if (handler) {
            // �������� ���������� ����������
            handler->handle(command, e);
        }
        else if (defaultHandler_) {
            // ���������� ���������� �� ���������
            defaultHandler_->handle(command, e);
        }
        else {
            // ���� ���������� �� ������, ������������� ����������
            std::cerr << "No handler found for exception: " << e.what() << std::endl;
            throw;
        }
    }

    return true;
}

/**
 * @brief ���������� ��� ������� � �������
 */
void CommandQueue::processAll() {
    while (processOne()) {
        // ���������� ��������� ���� ���� �������
    }
}

/**
 * @brief ���������, ����� �� �������
 */
bool CommandQueue::isEmpty() const {
    return commands_.empty();
}

/**
 * @brief �������� ������ �������
 */
size_t CommandQueue::size() const {
    return commands_.size();
}

/**
 * @brief ���������� ���������� �� ���������
 */
void CommandQueue::setDefaultHandler(std::shared_ptr<IExceptionHandler> handler) {
    defaultHandler_ = handler;
}

/**
 * @brief ����� ���������� ���������� ��� ����������
 */
std::shared_ptr<IExceptionHandler> CommandQueue::findHandler(
    std::shared_ptr<ICommand> command,
    const std::exception& exception) {

    // ������� ���� ��� ������ �����������
    HandlerKey key(std::type_index(typeid(*command)),
        std::type_index(typeid(exception)));

    // ���� ������ ����������
    auto it = handlers_.find(key);
    if (it != handlers_.end()) {
        return it->second;
    }

    // ���� ������� ���������� ���, ���������� nullptr
    return nullptr;
}