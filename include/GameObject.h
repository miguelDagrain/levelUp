#ifndef LEVELUP_GAMEOBJECT_H
#define LEVELUP_GAMEOBJECT_H

#include "DesignByContract.h"
#include <SFML/Graphics.hpp>
#include <utility>
#include <memory>
#include <string>


using namespace std;
using gameLocation =  std::pair<double, double>;

/**
 * This class represents the Color in our logic system
 */
struct Color{
    int r;
    int g;
    int b;

    /**
     * Constructor of Color taking the RGB values
     * @param r Red value
     * @param g Green value
     * @param b Blue value
     */
    Color(int r, int g, int b);

    /**
     * Default constructor of Color
     */
    Color();

    /**
     * Default destructor
     */
    virtual ~Color();
};

/**
 * Abstract class for each game object
 */
class GameObject {
public:
    /**
     * Default constructor
     */
    GameObject();

    /**
     * Constructor of game object
     * @param width The width of the game object
     * @param height The height of the game object
     */
    GameObject(int width, int height);

    /**
     * Constructor of game object with color
     * @param width Width of the game object
     * @param height Height of the game object
     * @param color Color of the game object
     */
    GameObject(int width, int height, std::shared_ptr<Color> color);

    /**
     * Getter of the width
     * @return int, width
     */
    int getWidth() const;

    /**
     * Setter of the width
     * @param width The int that is supposed to be the width
     */
    void setWidth(int width);

    /**
     * Getter of the height
     * @return int, height
     */
    int getHeight() const;

    /**
     * sets the height
     * @param height An int that is supposed to be the height
     */
    void setHeight(int height);

    /**
     * Getter of the color
     * @return Shared pointer that is the color
     */
    const std::shared_ptr<Color>& getColor() const;

    /**
     * Setter of the color
     * @param color A shared pointer that is the color
     */
    void setColor(const std::shared_ptr<Color>& color);

    /**
     * Sets the name of the game object
     * @param name The string that is the name
     */
    void setName(const std::string &name);

    /**
     * Returns the gamename
     * @return The name of the game
     */
    const std::string &getName() const;

    /**
     * default destructor
     */
    virtual ~GameObject() = default;

    /**
     * Getter for the Texture path of the object
     * @return String which represents texture path
     */
    const string& getTexturePath() const;

    /**
     * Setter for texture path of the object.
     * Also check if the texture can be loaded, and loads it into m_texture if possible.
     * If the texture is loaded, the sprite will be initialized.
     * @param texturePath Texture path of the object
     */
    void setTexturePath(const string& texturePath);

    /**
     * Determine whether a GameObject has a texture or not.
     * @return True if the GameObject has a texture.
     */
    bool hasTexture() const;

    /**
     * Scales the GameObject that must be drawn correctly.
     * If the GameObject has a texture, the sprite will be scaled, else the rectangle will be scaled.
     * @param scaleX Scale factor of X.
     * @param scaleY Scale factor of Y.
     */
    void scale(float scaleX, float scaleY);

    void setPosition(const gameLocation& location);

    void draw(sf::RenderWindow& window);

private:
    int m_width;
    int m_height;
    std::shared_ptr<Color> m_color;
    std::string m_name;
    string m_texturePath;
    bool m_hasTexture;
    sf::Sprite m_sprite;
    sf::RectangleShape m_shape;
    sf::Texture m_texture;
};


#endif //LEVELUP_GAMEOBJECT_H
