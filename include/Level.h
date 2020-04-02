#ifndef LEVELUP_LEVEL_H
#define LEVELUP_LEVEL_H

#include <memory>
#include "Entity.h"
#include "Object.h"

using namespace std;

/**
 * Represents a level of the game
 */
class Level: public GameObject {
private:
    vector<shared_ptr<Entity>> m_entities{};
    vector<shared_ptr<Object>> m_objects{};
    shared_ptr<Object> m_endObject;
public:
    /**
     * Default constructor for Level
     */
    Level();

    /**
     * Constructor for Level
     * @param entities Entities in the Level
     * @param objects Objects in the Level
     * @param endObject Object which indicates the end of the level
     */
    Level(const vector<shared_ptr<Entity>>& entities, const vector<shared_ptr<Object>>& objects,
            const shared_ptr<Object>& endObject);

    /**
     * Constructor for Level
     * @param width Width of the level
     * @param height Height of the level
     * @param color Color of the level
     * @param entities Entities of the level
     * @param objects Objects of the level
     * @param endObject Endobject of the level
     */
    Level(int width, int height, const shared_ptr<Color> &color, const vector<shared_ptr<Entity>> &entities,
          const vector<shared_ptr<Object>> &objects, const shared_ptr<Object> &endObject);

    /**
     * Constructor for the level taking the name of the level
     * @param name Name of the level
     */
    explicit Level(const std::string& name);

    /**
     * Default destructor for Level
     */
    ~Level() override;

    /**
     * Getter for Entities of the Level
     * @return Vector of entities
     */
    const vector<shared_ptr<Entity>>& getEntities() const;

    /**
     * Getter for Objects of the Level
     * @return vector of objects
     */
    const vector<shared_ptr<Object>>& getObjects() const;

    /**
     * Getter of end object of the level
     * @return Object
     */
    const shared_ptr<Object>& getEndObject() const;

    /**
     * Setter for Entities of the Level
     * @param entities Entities of the Level
     */
    void setEntities(const vector<shared_ptr<Entity>>& entities);

    /**
     * Setter for Objects of the Level
     * @param objects Objects of the Level
     */
    void setObjects(const vector<shared_ptr<Object>>& objects);

    /**
     * Setter for the end object of the Level
     * @param endObject End object of the Level
     */
    void setEndObject(const shared_ptr<Object>& endObject);

    /**
     * Adds an object to the object list
     * @param obj Object to add to the list
     */
    void addObject(const shared_ptr<Object> & obj);

    /**
     * Adds an entity to the entity list
     * @param ent Entity to add
     */
    void addEntity(const shared_ptr<Entity>& ent);

    /**
     * Searches for the player in the list of entities
     * @return shared pointer to entity that represents the player
     */
    shared_ptr<Entity> getPlayer() const;
};

#endif //LEVELUP_LEVEL_H
