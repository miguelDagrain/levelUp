#include <Level.h>

/**
 * Constructor for the level taking the name of the level
 * @param name Name of the level
 */
Level::Level(const std::string& name)
{
    this->setName(name);
}

/**
 * Constructor for Level
 * @param entities Entities in the Level
 * @param objects Objects in the Level
 * @param endObject Object which indicates the end of the level
 */
Level::Level(const vector<shared_ptr<Entity>>& entities, const vector<shared_ptr<Object>>& objects,
        const shared_ptr<Object>& endObject)
        :m_entities(entities), m_objects(objects), m_endObject(endObject) { }

/**
 * Constructor for Level
 * @param width Width of the level
 * @param height Height of the level
 * @param color Color of the level
 * @param entities Entities of the level
 * @param objects Objects of the level
 * @param endObject Endobject of the level
 */
Level::Level(int width, int height, const shared_ptr<Color>& color, const vector<shared_ptr<Entity>>& entities,
        const vector<shared_ptr<Object>>& objects, const shared_ptr<Object>& endObject)
        :GameObject(width,
        height,
        color),
         m_entities(
                 entities),
         m_objects(objects),
         m_endObject(
                 endObject) { }

/**
 * Getter for Entities of the Level
 * @return Vector of entities
 */
const vector<shared_ptr<Entity>>& Level::getEntities() const
{
    return m_entities;
}

/** sets the entities
 * @param   a vector of entities
 * @return  none
 */
void Level::setEntities(const vector<shared_ptr<Entity>>& entities)
{
    Level::m_entities = entities;
}

/**
 * Getter for Objects of the Level
 * @return vector of objects
 */
const vector<shared_ptr<Object>>& Level::getObjects() const
{
    return m_objects;
}

/** sets the objects
 * @param   vector of objects
 * @return  none
 */
void Level::setObjects(const vector<shared_ptr<Object>>& objects)
{
    Level::m_objects = objects;
}

/**
 * Getter of end object of the level
 * @return Object
 */
const shared_ptr<Object>& Level::getEndObject() const
{
    return m_endObject;
}

/**
 * Setter for the end object of the Level
 * @param endObject End object of the Level
 */
void Level::setEndObject(const shared_ptr<Object>& endObject)
{
    Level::m_endObject = endObject;
}

/**
 * Adds an object to the object list
 * @param obj Object to add to the list
 */
void Level::addObject(const shared_ptr<Object>& obj)
{
    this->m_objects.push_back(obj);
}

/**
 * Adds an entity to the entity list
 * @param ent Entity to add
 */
void Level::addEntity(const shared_ptr<Entity>& ent)
{
    this->m_entities.push_back(ent);
}

/**
 * Default constructor for Level
 */
Level::Level()
        :m_endObject(nullptr) { }

/**
 * Searches for the player in the list of entities
 * @return shared pointer to entity that represents the player
 */
shared_ptr<Entity> Level::getPlayer() const
{
    for (auto& entity : m_entities) {
        if (entity->isPlayer())
            return entity;
    }
    return nullptr;
}

/**
 * Default destructor for Level
 */
Level::~Level() = default;
