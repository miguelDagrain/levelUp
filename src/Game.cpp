#include <Game.h>

/**
 * Adds a level to the game
 * @param level a pointer to a new level
 */
void Game::addLevel(shared_ptr<Level> level)
{
    m_levels.push_back(level);
}

/**
 * Returns a level
 * @return a shared pointer to the levels
 */
const vector<shared_ptr<Level>>& Game::getLevels() const
{
    return m_levels;
}

/**
 * Constructor of the game that takes the name of the game as inut
 * @param gameName String that is the name of the game
 */
Game::Game(const std::string& gameName)
{
    this->setName(gameName);
}

/**
 * Sets the levels of the game
 * @param levels a shared pointer to the levels
 */
void Game::setLevels(const vector<shared_ptr<Level>>& levels)
{
    Game::m_levels = levels;
}