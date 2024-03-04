#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button: public sf::Sprite
{
public:

	typedef std::function<void()> OnClick; //Podriem pasar per parametre quin boto es, click esquerra, dret, etc...
	OnClick onClick = [](){}; //[] -> Espai de memoria/context; () -> Parametres; {} -> Funcio com a tal;

	Button(float x, float y, std::string texturePath);

	bool CheckBounds(unsigned int x, unsigned int y);

private:

	sf::Texture texture;
};

