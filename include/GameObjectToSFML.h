
#ifndef LEVELUP_GAMEOBJECTTOSFML_H
#define LEVELUP_GAMEOBJECTTOSFML_H


#include <vector>
#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>

#include <TuringMachine.h>
#include <Game.h>
#include <GameObject.h>
#include <Level.h>
#include <Object.h>
#include <Entity.h>

using level_ptr  = shared_ptr<Level>;
using gameObject_ptr = shared_ptr<GameObject>;
using object_ptr = shared_ptr<Object>;
using entity_ptr = shared_ptr<Entity>;
using drawable_ptr = shared_ptr<sf::Drawable>;


namespace Gui {
    const float original_Height = 500;
    const float original_Width = 500;

    /**
     * Function which transforms a level to a vector of drawable objects.
     * @param level Pointer to a level of the game
     * @param window Window to draw level to
     */
    void to_SFML_level(const level_ptr& level, sf::RenderWindow& window);

    /**
     * Function that converts a game object to a drawable object.
     * @param object Shared pointer to a drawable game object
     * @param window The window to draw the object to
     */
    void to_SFML_object(const gameObject_ptr& object, sf::RenderWindow& window);

    /**
     * Function that re-sizes the view to avoid stretched images
     * @param window Window to where we draw on
     * @param view View of the window which needs to be re-sized
     */
    void resize_view(sf::RenderWindow& window, sf::View view);

    /**
     * Starts the game, launches the gui
     * @param game Shared pointer to the game
     * @param check_TM Turing machines which checks for legal movement
     * @param update_loc Turing machine which updates the locations
     */
    void run_game(std::shared_ptr<Game> game, TuringMachine& check_TM, TuringMachine& update_loc);

    /**
     * Creates the turing machine tapes for a level
     * @param level Level to create tapes for
     * @return vector of strings
     */
    std::vector<std::string> create_tapes_level(const level_ptr& level);

    /**
     * Creates a string for the player
     * @param player Player to create string for
     * @return String for player
     */
    std::string create_string_player(entity_ptr player);

    /**
     * Creates tapes for the end of the level
     * @param level Level to generate tapes for
     * @return Tape (aka vector of strings)
     */
    std::vector<std::string> create_tapes_end_level(level_ptr level);

    /**
     * Applies the status to the level
     * @param level Level to apply status to
     * @param status Status to apply
     */
    void apply_to_level(level_ptr level, std::string& status);

    /**
     * Returns the sub objects of a parent object
     * @param parent_object Parent object to get sub objects from
     * @return Vector of sub objects
     */
    std::vector<object_ptr> get_leafs(const object_ptr& parent_object);
}

#endif //LEVELUP_GAMEOBJECTTOSFML_H
