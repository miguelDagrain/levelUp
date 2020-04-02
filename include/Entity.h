#ifndef LEVELUP_ENTITY_H
#define LEVELUP_ENTITY_H

#include "GameObject.h"
#include <iostream>

using namespace std;

/**
 * Class which represents an Entity of the Game
 * Derived from abstract GameObject
 */
class Entity : public GameObject {
private:
    double m_movementSpeed;
    double m_jumpHeight;
    gameLocation m_location;
    bool m_player;
public:
    /**
     * Constructor for Entity
     * @param width Width of the entity
     * @param height Height of the entity
     * @param moveSpeed Movement speed of the entity
     * @param jumpHeight Jump height of the entity
     * @param location Location of the Entity
     * @param m_texturePath Path to texture of entity
     */
    Entity(int width, int height, double moveSpeed, double jumpHeight, const gameLocation& location,
            const string& m_texturePath);

    /**
     * Default constructor for Entity
     */
    Entity();

    /**
     * Getter for movement speed
     * @return Movement speed (double)
     */
    double getMovementSpeed() const;

    /**
     * Getter for jump height
     * @return Jump height (double)
     */
    double getJumpHeight() const;

    /**
     * Getter for the location of the entity
     * @return Location of the entity
     */
    const gameLocation& getLocation() const;

    /**
     * Setter for jump height
     * @param jumpHeight Jump height of the entity
     */
    void setJumpHeight(double jumpHeight);

    /**
     * Setter for the location
     * @param location Location of the entity
     */
    void setLocation(const gameLocation& location);

    /**
     * Setter for movement speed of entity
     * @param speed Movement speed of entity
     */
    void setMovementSpeed(double speed);

    /**
     * Default destructor for Entity
     */
    ~Entity() override = default;

    /**
     * Determine if the entity is a player or not
     * @return True if the entity is a player
     */
    virtual bool isPlayer() const;

    /**
     * Setter for bool to determine if entity is player or not
     * @param isPlayer Bool which identifies the player
     */
    void setPlayerBool(bool isPlayer);
};

#endif //LEVELUP_ENTITY_H
