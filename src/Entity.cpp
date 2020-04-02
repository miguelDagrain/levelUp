#include <Entity.h>

/**
 * Constructor for Entity
 * @param width Width of the entity
 * @param height Height of the entity
 * @param moveSpeed Movement speed of the entity
 * @param jumpHeight Jump height of the entity
 * @param location Location of the Entity
 * @param m_texturePath Path to texture of entity
 */
Entity::Entity(int width, int height, double moveSpeed, double jumpHeight, const gameLocation& location,
        const string& m_texturePath)
        :GameObject(width, height), m_movementSpeed(moveSpeed), m_jumpHeight(jumpHeight),
         m_location(location) {
    setTexturePath(m_texturePath);
    ENSURE(m_jumpHeight>=0,"jumpheight may not be bellow 0 after initialisation");
}

/**
 * Getter for movement speed
 * @return Movement speed (double)
 */
double Entity::getMovementSpeed() const
{
    return m_movementSpeed;
}

/**
 * Setter for movement speed of entity
 * @param speed Movement speed of entity
 */
void Entity::setMovementSpeed(double speed)
{
    Entity::m_movementSpeed = speed;
}

/**
 * Getter for jump height
 * @return Jump height (double)
 */
double Entity::getJumpHeight() const
{
    return m_jumpHeight;
}

/**
 * Setter for jump height
 * @param jumpHeight Jump height of the entity
 */
void Entity::setJumpHeight(double jumpHeight)
{
    REQUIRE(jumpHeight>=0,"jumheight may not be below 0")
    Entity::m_jumpHeight = jumpHeight;
}

/**
 * Getter for the location of the entity
 * @return Location of the entity
 */
const gameLocation& Entity::getLocation() const
{
    return m_location;
}

/**
 * Setter for the location
 * @param location Location of the entity
 */
void Entity::setLocation(const gameLocation& location)
{
    Entity::m_location = location;
}



/**
 * Determine if the entity is a player or not
 * @return True if the entity is a player
 */
bool Entity::isPlayer() const
{
    return m_player;
}

/**
 * Setter for bool to determine if entity is player or not
 * @param isPlayer Bool which identifies the player
 */
void Entity::setPlayerBool(bool isPlayer)
{
    m_player = isPlayer;
}

/**
 * Default constructor for Entity
 */
Entity::Entity() = default;
