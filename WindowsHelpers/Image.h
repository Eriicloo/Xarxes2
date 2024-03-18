#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Image : public sf::Sprite
{
public:

	Image(float x, float y, std::string texturePath);

	bool CheckBounds(unsigned int x, unsigned int y);

private:

	sf::Texture texture;
};

