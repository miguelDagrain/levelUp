#include <utility>
#include <GameObject.h>
#include <iostream>

#include "GameObject.h"

/**
 * Constructor of game object
 * @param width The width of the game object
 * @param height The height of the game object
 */
GameObject::GameObject(int width, int height)
        :m_width(width), m_height(height), m_hasTexture(false) { }

/**
 * Constructor of game object with color
 * @param width Width of the game object
 * @param height Height of the game object
 * @param color Color of the game object
 */
GameObject::GameObject(int width, int height, std::shared_ptr<Color> color)
        :m_width(width), m_height(height), m_color(
        std::move(color)), m_hasTexture(false) {
    ENSURE(height>=0,"height may not be below 0 after initializing");
    ENSURE(height>=0,"width may not be below 0 after initializing");
}

/**
 * Default constructor
 */
GameObject::GameObject()
        :m_width(0), m_height(0), m_color(new Color), m_hasTexture(false) { }

/**
 * Getter of the width
 * @return int, width
 */
int GameObject::getWidth() const
{
    return m_width;
}

/**
 * Setter of the width
 * @param width The int that is supposed to be the width
 */
void GameObject::setWidth(int width)
{
    REQUIRE(width>=0,"width may not be below 0");
    GameObject::m_width = width;
}

/**
 * Getter of the height
 * @return int, height
 */
int GameObject::getHeight() const
{
    return m_height;
}

/**
 * sets the height
 * @param height An int that is supposed to be the height
 */
void GameObject::setHeight(int height)
{
    REQUIRE(height>=0,"height may not be below 0");
    GameObject::m_height = height;
}

/**
 * Getter of the color
 * @return Shared pointer that is the color
 */
const std::shared_ptr<Color>& GameObject::getColor() const
{
    return m_color;
}

/**
 * Setter of the color
 * @param color A shared pointer that is the color
 */
void GameObject::setColor(const std::shared_ptr<Color>& color)
{
    GameObject::m_color = color;
}

/**
 * Returns the gamename
 * @return The name of the game
 */
const std::string& GameObject::getName() const
{
    return m_name;
}

/**
 * Sets the name of the game object
 * @param name The string that is the name
 */
void GameObject::setName(const std::string& name)
{
    GameObject::m_name = name;
}

/**
 * Constructor of Color taking the RGB values
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 */
Color::Color(int r, int g, int b)
        :r(r), g(g), b(b) {
    ENSURE(r>=0&&g>=0&&b>=0,"after initialisation of a color all color values must be above 0");
    ENSURE(r<=255&&g<=255&&b<=255,"after initialisation of a color all color values must be below 256");
}

/**
 * Default destructor
 */
Color::~Color() = default;

/**
 * Default constructor of Color
 */
Color::Color() = default;

/**
 * Getter for the Texture path of the object
 * @return String which represents texture path
 */
const string& GameObject::getTexturePath() const
{
    return m_texturePath;
}

/**
 * Setter for texture path of the object
 * @param texturePath Texture path of the object
 */
void GameObject::setTexturePath(const string& texturePath)
{
    if(m_texture.loadFromFile(texturePath)){
        m_texture.loadFromFile(texturePath);
        m_hasTexture = true;
        sf::Sprite sprite(m_texture);
        m_sprite = move(sprite);
    }
    GameObject::m_texturePath = texturePath;
}

/**
 * Determine whether a GameObject has a texture or not.
 * @return True if the GameObject has a texture.
 */
bool GameObject::hasTexture() const
{
    return m_hasTexture;
}

void GameObject::scale(float scaleX, float scaleY)
{
    if(hasTexture()){

        scaleX = scaleX / m_sprite.getGlobalBounds().width;
        scaleY = scaleY / m_sprite.getGlobalBounds().height;

        m_sprite.scale(scaleX, scaleY);
    }
    else{
        sf::RectangleShape rect(sf::Vector2f(scaleX, scaleY));
        m_shape = std::move(rect);
    }
}

void GameObject::setPosition(const gameLocation& location)
{
    if(hasTexture()){
        m_sprite.setPosition(static_cast<float>(location.first),
                static_cast<float>(location.second));
    }else{
        m_shape.setPosition(static_cast<float>(location.first),
                static_cast<float>(location.second));
    }
}

void GameObject::draw(sf::RenderWindow& window)
{
    if(hasTexture()) {
        window.draw(m_sprite);
    }
    else {
        sf::Color color_of_shape(static_cast<sf::Uint8>(getColor()->r),
                static_cast<sf::Uint8>(getColor()->g),
                static_cast<sf::Uint8>(getColor()->b));
        m_shape.setFillColor(color_of_shape);
        window.draw(m_shape);
    }
}
