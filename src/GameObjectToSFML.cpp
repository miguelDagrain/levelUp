
#include <GameObjectToSFML.h>

/**
 * Function which transforms a level to a vector of drawable objects.
 * @param level Pointer to a level of the game
 * @param window Window to draw level to
 */
void Gui::to_SFML_level(const level_ptr& level, sf::RenderWindow& window)
{
    std::shared_ptr<Color> background = level->getColor();
    window.clear(sf::Color(static_cast<sf::Uint8>(background->r), static_cast<sf::Uint8>(background->g),
            static_cast<sf::Uint8>(background->b)));
    std::vector<drawable_ptr> return_vec{};
    for (auto& object:level->getObjects()) {

        to_SFML_object(object, window);
    }

    for (auto& entity:level->getEntities()) {
        to_SFML_object(entity, window);

    }
}

/**
 * Function that converts a game object to a drawable object.
 * @param object Shared pointer to a drawable game object
 * @param window The window to draw the object to
 */
void Gui::to_SFML_object(const gameObject_ptr& object, sf::RenderWindow& window)
{
    if (std::dynamic_pointer_cast<Object>(object)) {

        // cast object
        std::shared_ptr<Object> casted_object = std::dynamic_pointer_cast<Object>(object);

        // get location
        gameLocation loc = casted_object->getLocation();

        //if there are no sub objects (otherwise we get a square block)
        if (!casted_object->getSubObjects().empty()) {
            for (const auto& sub_object:casted_object->getSubObjects()) {
                to_SFML_object(sub_object, window);
            }
        }
        else {
            // get size of object
            int height = casted_object->getHeight();
            int width = casted_object->getWidth();

            // scale the object
            casted_object->scale(width, height);

            // calculate location relative to parent object
            gameLocation object_location;
            object_location.first = loc.first;
            object_location.second = loc.second;

            // correct y-axis
            object_location.second = -object_location.second+window.getSize().y-casted_object->getHeight();
            casted_object->setPosition(object_location);

            // If the object has a texture, the location is different
            if (casted_object->hasTexture()) {
                // Correct y-axis
                loc.second = -loc.second+window.getSize().y-casted_object->getHeight();
                casted_object->setPosition(loc);
            }

            // draw the object
            casted_object->draw(window);
        }

    }
    else if (std::dynamic_pointer_cast<Entity>(object)) {

        // cast object
        entity_ptr casted_entity = std::dynamic_pointer_cast<Entity>(object);

        // get location
        gameLocation loc = casted_entity->getLocation();

        // get size of object
        int height = casted_entity->getHeight();
        int width = casted_entity->getWidth();

        // scale the object
        casted_entity->scale(width, height);

        // if the entity has a texture, the location is different
        if (casted_entity->hasTexture()) {
            loc.second = -loc.second+window.getSize().y-casted_entity->getHeight();
            casted_entity->setPosition(loc);
        }
        else {
            // correct y-axis
            loc.second = -loc.second+window.getSize().y-casted_entity->getHeight();
            casted_entity->setPosition(loc);
        }
        casted_entity->draw(window);
    }
    else {
        throw;
    }

}

/**
 * Starts the game, launches the gui
 * @param game Shared pointer to the game
 * @param check_TM Turing machines which checks for legal movement
 * @param update_loc Turing machine which updates the locations
 */
void Gui::run_game(std::shared_ptr<Game> game, TuringMachine& check_TM, TuringMachine& update_lock)
{
    sf::RenderWindow window(sf::VideoMode(700, 700), "LevelUp", sf::Style::Close | sf::Style::Titlebar);
    sf::Image icon;
    icon.loadFromFile("./res/sprites/dog.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    std::vector<level_ptr> levels = game->getLevels();
    unsigned long long int currentLevel = 0;

    // This bool is needed to check whether the player is on the ground (is able to jump)
    bool allow_jump = true;

    // Needed so we dont odly move to the right without gravity
    bool not_just_moved = true;
    sf::Clock gameClock;
    gameClock.restart();
    while (window.isOpen()) {
        if (gameClock.getElapsedTime().asSeconds()>0.016) {
            gameClock.restart();

            entity_ptr player = levels.at(currentLevel)->getPlayer();

            sf::Event event{};
            if (window.pollEvent(event)) {
                switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::LostFocus:
                    window.setActive(false);
                    break;
                case sf::Event::GainedFocus:
                    window.setActive(true);
                    break;
                default:
                    break;
                }
            }
            //###################################################################
            // checking for the input
            enum key {
                none, up, right, left
            };

            key button_pressed = none;
            double move = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && allow_jump) {
                move = player->getJumpHeight();
                button_pressed = up;
                allow_jump = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && not_just_moved) {
                move = player->getMovementSpeed();
                button_pressed = right;
                not_just_moved = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && not_just_moved) {
                move = player->getMovementSpeed();
                button_pressed = left;
                not_just_moved = false;
            }
            else {
                move = 1;
            }

            for (int iter_move = 0; iter_move<move; ++iter_move) {

                // Tapes used to check for collision
                std::vector<std::string> tapes = create_tapes_level(levels.at(currentLevel));

                // Used to alter the string of the first tapes
                std::vector<std::string> tape_player{tapes.at(0)};

                if (button_pressed==up) {
                    //tape of player entity
                    tape_player.at(0) += "+S";
//                update_level_player(levels.at(currentLevel), 0, 10);
                }
                else if (button_pressed==right) {
                    tape_player.at(0) += "+F";
//                update_level_player(levels.at(currentLevel), 10, 0);
                }
                else if (button_pressed==left) {
                    tape_player.at(0) += "-F";
//                update_level_player(levels.at(currentLevel), -10, 0);
                }
                else {
                    tape_player.at(0) += "-S";
//                update_level_player(levels.at(currentLevel), 0, -10);
                    // We are falling, jumping is not allowed
                    allow_jump = false;
                    not_just_moved = true;
                }

                // Setup Turing machine with correct tape
                update_lock.setup_tapes_with_string(tape_player);

                // Perform operations
                tape_player = update_lock.calculate();

                // reset tapes which were used to alter the location
                update_lock.reset_tapes();

                // Apply to current level (to keep display up to date)
                apply_to_level(levels.at(currentLevel), tape_player.at(0));

                // Check if we reached the end, also possible with check_TM
                std::vector<std::string> end_tapes = create_tapes_end_level(levels.at(currentLevel));
                check_TM.setup_tapes_with_string(end_tapes);
                bool not_won = check_TM.evaluate_string();
                check_TM.reset_tapes();
                if (!not_won) {
                    ++currentLevel;
                    // In case this was the last level
                    if (currentLevel==levels.size()) {
                        // now we need to close the game
                        while (window.isOpen()) {
                            while (window.pollEvent(event)) {
                                if (event.type==sf::Event::Closed) {
                                    window.close();
                                }
                            }
                            window.clear(sf::Color::Black);
                            sf::Font font;
                            font.loadFromFile("slkscr.ttf");
                            sf::Text text;
                            text.setFont(font);
                            text.setString("Congratulations you won");
                            text.setCharacterSize(30);
                            text.setFillColor(sf::Color::White);
                            text.setStyle(sf::Text::Bold);
                            text.setPosition(100, 350);
                            window.draw(text);
                            window.display();
                        }
                        break;
                    }
                    // reset the booleans
                    allow_jump = true;
                    not_just_moved = true;
                    break;
                }

                // Set tapes correctly
                tapes.at(0) = tape_player.at(0);

                // Set turing machine with tapes
                check_TM.setup_tapes_with_string(tapes);

                // Evaluate if a move is possible
                bool possible = check_TM.evaluate_string();
                check_TM.reset_tapes();

                if (!possible) {
                    std::vector<std::string> tapes_correction = create_tapes_level(levels.at(currentLevel));
                    // undo changes
                    if (button_pressed==up) {
                        tapes_correction.at(0) += "-S";
//                    update_level_player(levels.at(currentLevel), 0, -10);
                    }
                    else if (button_pressed==right) {
                        tapes_correction.at(0) += "-F";
//                    update_level_player(levels.at(currentLevel), -10, 0);
                    }
                    else if (button_pressed==left) {
                        tapes_correction.at(0) += "+F";
//                    update_level_player(levels.at(currentLevel), 10, 0);
                    }
                    else {
                        tapes_correction.at(0) += "+S";
//                    update_level_player(levels.at(0), 0, 10);
                        allow_jump = true;
                    }
                    std::vector<std::string> player_correction{tapes_correction.at(0)};
                    update_lock.setup_tapes_with_string(player_correction);
                    player_correction = update_lock.calculate();
                    update_lock.reset_tapes();
                    apply_to_level(levels.at(currentLevel), player_correction.at(0));
                }

                //######################################################################

                window.clear(sf::Color::Black);

                Gui::to_SFML_level(levels.at(currentLevel), window);
                window.display();
            }
        }
    }
}

/**
 * Creates the turing machine tapes for a level
 * @param level Level to create tapes for
 * @return vector of strings
 */
std::vector<std::string> Gui::create_tapes_level(const level_ptr& level)
{

    entity_ptr player = level->getPlayer();
    std::string player_string = create_string_player(player);

    //#####################################################################################

    std::string object_string;
    std::vector<object_ptr> leafs;
    for (auto& parent_object:level->getObjects()) {
        if (parent_object->canCollide()) {
            for (auto& leaf:get_leafs(parent_object)) {
                leafs.push_back(leaf);
            }
        }
    }
    for (auto& object:leafs) {
        object_string += "1";
        gameLocation loc_object = object->getLocation();
        for (int iter_loc_x = 0; iter_loc_x<loc_object.first; iter_loc_x += 10) {
            object_string += "x";
        }
        for (int iter_width_x = 0; iter_width_x<object->getWidth(); iter_width_x += 10) {
            object_string += "X";
        }
        object_string += "2";
        for (int iter_loc_y = 0; iter_loc_y<loc_object.second; iter_loc_y += 10) {
            object_string += "y";
        }
        for (int iter_height_y = 0; iter_height_y<object->getHeight(); iter_height_y += 10) {
            object_string += "Y";
        }
        object_string += "3";
    }

    //#####################################################################################

    return {player_string, object_string};
}

/**
 * Creates tapes for the end of the level
 * @param level Level to generate tapes for
 * @return Tape (aka vector of strings)
 */
std::vector<std::string> Gui::create_tapes_end_level(level_ptr level)
{
    entity_ptr player = level->getPlayer();
    std::string player_string = create_string_player(player);

    //#####################################################################################

    object_ptr end = level->getEndObject();
    std::string end_string = "1";
    gameLocation loc_object = end->getLocation();

    for (int iter_loc_x = 0; iter_loc_x<loc_object.first; iter_loc_x += 10) {
        end_string += "x";
    }
    for (int iter_width_x = 0; iter_width_x<end->getWidth(); iter_width_x += 10) {
        end_string += "X";
    }
    end_string += "2";
    for (int iter_loc_y = 0; iter_loc_y<loc_object.second; iter_loc_y += 10) {
        end_string += "y";
    }
    for (int iter_height_y = 0; iter_height_y<end->getHeight(); iter_height_y += 10) {
        end_string += "Y";
    }
    end_string += "3";
    return {player_string, end_string};

}

/**
 * Applies the status to the level
 * @param level Level to apply status to
 * @param status Status to apply
 */
void Gui::apply_to_level(level_ptr level, std::string& status)
{

    //status is status of player
    int location_x_status = 0;
    int location_y_status = 0;

    for (auto character:status) {
        if (character=='x') {
            location_x_status += 10;
        }
        else if (character=='y') {
            location_y_status += 10;
        }
    }
    gameLocation location_status{location_x_status, location_y_status};
    level->getEntities().at(0)->setLocation(location_status);
}

/**
 * Creates a string for the player
 * @param player Player to create string for
 * @return String for player
 */
std::string Gui::create_string_player(entity_ptr player)
{
    // 1 needed for TM
    std::string player_string = "1";
    gameLocation loc_player = player->getLocation();

    // The game runs per 10 pixels (to avoid excessive string length)
    for (int iter_loc_x = 0; iter_loc_x<loc_player.first; iter_loc_x += 10) {
        player_string += "x";
    }
    for (int iter_width_x = 0; iter_width_x<player->getWidth(); iter_width_x += 10) {
        player_string += "X";
    }

    // Symbols needed for the Turing machine
    player_string += "U";
    player_string += "2";

    for (int iter_loc_y = 0; iter_loc_y<loc_player.second; iter_loc_y += 10) {
        player_string += "y";
    }
    for (int iter_height_y = 0; iter_height_y<player->getHeight(); iter_height_y += 10) {
        player_string += "Y";
    }
    // Symbol needed for the Turing machine
    player_string += "3";
    return player_string;
}

/**
 * Returns the sub objects of a parent object
 * @param parent_object Parent object to get sub objects from
 * @return Vector of sub objects
 */
std::vector<object_ptr> Gui::get_leafs(const object_ptr& parent_object)
{
    if (parent_object->getSubObjects().empty()) {
        return {parent_object};
    }
    else {
        std::vector<object_ptr> leafs;
        for (auto& sub_object:parent_object->getSubObjects()) {
            for (auto& leaf:get_leafs(sub_object)) {
                leafs.push_back(leaf);
            }
        }
        return leafs;
    }
}