#include "Image.h"

Image::Image(float x, float y, std::string texturePath)
{
    if (!texture.loadFromFile(texturePath)) //Check if the texture size is enough
    {
        //Manage Error
    }

    this->setPosition(x, y);
    this->setTexture(texture);
}

bool Image::CheckBounds(unsigned int x, unsigned int y)
{
    return this->getGlobalBounds().contains(x, y);
}
