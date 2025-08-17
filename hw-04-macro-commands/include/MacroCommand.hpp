#pragma once

#include "ICommand.hpp"
#include "CommandException.hpp"
#include <memory>
#include <vector>

/**
 * @file MacroCommand.hpp
 * @brief ћакрокоманда дл€ последовательного выполнени€ команд
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ћакрокоманда дл€ последовательного выполнени€ команд
  *
  * –еализует паттерн Composite дл€ команд.
  * ¬ыполн€ет последовательность команд, прерыва€ выполнение при первой ошибке.
  * —ледует принципу Liskov Substitution - может использоватьс€ везде,
  * где используетс€ ICommand.
  */
class MacroCommand : public ICommand {
private:
    std::vector<std::shared_ptr<ICommand>> m_commands; ///< ѕоследовательность команд

public:
    /**
     * @brief  онструктор макрокоманды
     * @param commands ¬ектор команд дл€ последовательного выполнени€
     * @throws std::invalid_argument если вектор пуст или содержит nullptr
     */
    MacroCommand(const std::vector<std::shared_ptr<ICommand>>& commands);

    /**
     * @brief ¬ыполнить все команды последовательно
     *
     * ¬ыполн€ет команды в пор€дке их следовани€ в векторе.
     * ѕри возникновении исключени€ прерывает выполнение и пробрасывает исключение.
     *
     * @throws CommandException если одна из команд выбросила CommandException
     * @throws std::runtime_error если одна из команд не может быть выполнена
     */
    void execute() override;
};