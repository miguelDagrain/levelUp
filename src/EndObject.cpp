#include "EndObject.h"

/**
 * Constructor that takes the name of the End Object as string
 * @param name Name of the End Object
 */
EndObject::EndObject(const std::string& name)
{
    this->setName(name);
}

/**
 * Default constructor of the end object class
 */
EndObject::EndObject() = default;

/**
 * returns where the endobject goes to
 * @return   the string that is where the endobject goes to
 */
const string& EndObject::getGoto() const
{
    return m_goto;
}

/**
 * Sets the object where the end object goes to
 * @param gotoObj  a string where the object is supposed to be going to
 */
void EndObject::setGoto(const string& gotoObj)
{
    EndObject::m_goto = gotoObj;
}