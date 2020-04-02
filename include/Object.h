 #ifndef LEVELUP_OBJECT_H
#define LEVELUP_OBJECT_H

#include "GameObject.h"
#include <vector>

using namespace std;

class Object : public GameObject{
public:

    /**
     * Default constructor of object
     */
    Object();

    /**
     * Default Destructor
     */
    ~Object() override = default;

    /**
     * Setter for collision of Object
     * @param canCollide true if it can collide with other objects
     */
    void setCollision(bool canCollide);

    /**
     * Setter for the location of an object
     * @param location The new location
     */
    void setLocation(const gameLocation &location);

    /**
     * Setter for the sub objects of an object
     * @param subObjects A vector with objects
     */
    void setSubObjects(const vector<shared_ptr<Object>>& subObjects);

    /**
     * Getter for the sub objects
     * @return Vector with all the sub objects of an object
     */
    const vector<shared_ptr<Object>> &getSubObjects() const;

    /**
     * Decides whether an object can be collided with
     * @return True if the object can be collided with
     */
    bool canCollide() const;

    /**
     * Getter for object location
     * @return the gamelocation
     */
    const gameLocation &getLocation() const;

    /**
     * Adds a subobject to an object
     * @param newObject The subobject to add
     */
    void addSubObject(std::shared_ptr<Object> newObject);

private:
    bool m_canCollide;
    vector<shared_ptr<Object>> m_subObjects;
    gameLocation m_location;

};

#endif //LEVELUP_OBJECT_H
