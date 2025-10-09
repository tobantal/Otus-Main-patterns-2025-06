#include "CommandGetter.hpp"

void CommandGetter::execute()
{
    try
    {
        result_ = m_object->getProperty(m_property);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("CommandGetter execute failed: ") + e.what());
    }
}