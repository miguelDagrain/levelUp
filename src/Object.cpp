#include <Object.h>

/**
 * Decides whether an object can be collided with
 * @return True if the object can be collided with
 */
bool Object::canCollide() const
{
    return m_canCollide;
}

/**
 * Setter for collision of Object
 * @param canCollide true if it can collide with other objects
 */
void Object::setCollision(bool canCollide)
{
    Object::m_canCollide = canCollide;
}

/**
 * Getter for the sub objects
 * @return Vector with all the sub objects of an object
 */
const vector<shared_ptr<Object>>& Object::getSubObjects() const
{
    return m_subObjects;
}

/**
 * Getter for object location
 * @return the gamelocation
 */
const gameLocation& Object::getLocation() const
{
    return m_location;
}

/**
 * Setter for the location of an object
 * @param location The new location
 */
void Object::setLocation(const gameLocation& location)
{
    Object::m_location = location;
}

/**
 * Adds a subobject to an object
 * @param newObject The subobject to add
 */
void Object::addSubObject(std::shared_ptr<Object> newObject)
{
    m_subObjects.push_back(newObject);
}

/**
 * Setter for the sub objects of an object
 * @param subObjects A vector with objects
 */
void Object::setSubObjects(const vector<shared_ptr<Object>>& subObjects)
{
    Object::m_subObjects = subObjects;
}

/**
 * Default constructor of object
 */
Object::Object()
        :m_canCollide(false), m_location({0, 0})
{

}
