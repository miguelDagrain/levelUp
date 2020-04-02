#ifndef LEVELUP_ENDOBJECT_H
#define LEVELUP_ENDOBJECT_H

#include "Object.h"

class EndObject: public Object {
private:
    std::string m_goto;
public:
    /**
     * Default constructor of the end object class
     */
    EndObject();

    /**
     * Constructor that takes the name of the End Object as string
     * @param name Name of the End Object
     */
    explicit EndObject(const std::string& name);

    /**
     * returns where the endobject goes to
     * @return the string that is where the endobject goes to
     */
    const string &getGoto() const;

    /**
     * Sets the object where the end object goes to
     * @param gotoObj  a string where the object is supposed to be going to
     */
    void setGoto(const string& gotoObj);

};




#endif //LEVELUP_ENDOBJECT_H
