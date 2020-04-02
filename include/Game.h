#ifndef LEVELUP_GAME_H
#define LEVELUP_GAME_H

#include "GameObject.h"
#include "Level.h"
#include <vector>

using namespace std;

/**
 * This class is the container of the game
 */
class Game : public GameObject{
private:
    vector<shared_ptr<Level>> m_levels;
public:
    /**
     * Default constructor for the game
     */
    Game() = default;

    /**
     * Constructor of the game that takes the name of the game as inut
     * @param gameName String that is the name of the game
     */
    explicit Game(const std::string& gameName);

    /**
     * Adds a level to the game
     * @param level a pointer to a new level
     */
    void addLevel(shared_ptr<Level> level);

    /**
     * Sets the levels of the game
     * @param levels a shared pointer to the levels
     */
    void setLevels(const vector<shared_ptr<Level>>& levels);

    /**
     * Returns a level
     * @return a shared pointer to the levels
     */
    const vector<shared_ptr<Level>>& getLevels() const;

    /**
     * default destructor
     */
    ~Game() override = default;
};

#endif //LEVELUP_GAME_H
