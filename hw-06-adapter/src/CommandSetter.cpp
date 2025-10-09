#include "CommandSetter.hpp"

void CommandSetter::execute()
{
    try
    {
        m_object->setProperty(m_property, m_value);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("CommandSetter execute failed: ") + e.what());
    }
}