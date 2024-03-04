#include "Window.h"

Window::Window(unsigned int width, unsigned int height, std::string title)
{
	_window.create(sf::VideoMode(width, height), title);
	//Afegir el q fagi falta
}

void Window::AddButton(Button* button)
{
	_mutex.lock();


	//Aquesta llista, Z buffer haura danar cambiant. Quan movem un boto ha d'estar a la part de dalt,
	//com a minim no a sota de un altre boto
	//Si tinc 3 botons, i faig bringtofront del primer, l'he de pujar en les 2 llistes
	_buttons.push_back(button);
	_objectsToDraw.push_back(button);


	_mutex.unlock();
}

void Window::RunWindowsLoop()
{
	//Hauria de ser thread safe i no ho es ara mateix!!!!!
	//un update normal i corrent

	while (_window.isOpen()) 
	{
		_window.clear(sf::Color::Black);
		for (sf::Drawable* drawable : _objectsToDraw) 
		{
			if (drawable != nullptr) 
			{
				_window.draw(*drawable);

			}
		}
		_window.display();

		sf::Event event;

		while (_window.pollEvent(event)) //Per cada frame, la pantalla pot rebre multiples inputs.
		{
			switch (event.type)
			{
				case sf::Event::Closed:
				{
					_window.close();
					break;
				}
				case sf::Event::MouseButtonPressed: 
				{
					if (event.mouseButton.button == sf::Mouse::Left) 
					{
						sf::Vector2i clickPixelPos = { event.mouseButton.x, event.mouseButton.y };
						sf::Vector2f worldPos = _window.mapPixelToCoords(clickPixelPos);

						for (auto it = _buttons.rbegin(); it != _buttons.rend(); it++) 
						{
							Button* button = *it;
							if (button->CheckBounds(worldPos.x, worldPos.y)) 
							{
								_lastClickedDownButton = button;
								break;
							}
						}
					}
					break;
				}
				case sf::Event::MouseButtonReleased: 
				{
					if (event.mouseButton.button == sf::Mouse::Left) 
					{
						sf::Vector2i clickPixelPos = { event.mouseButton.x, event.mouseButton.y };
						sf::Vector2f worldPos = _window.mapPixelToCoords(clickPixelPos);

						if (_lastClickedDownButton != nullptr && _lastClickedDownButton->CheckBounds(worldPos.x, worldPos.y)) 
						{
							_lastClickedDownButton->onClick();
							_lastClickedDownButton = nullptr;
						}
						else
						{
							//Cancel Click or drag or others
						}
					}
					break;
				}
				case sf::Event::MouseWheelScrolled: //Moved esta deprecated, no usar
				{
					//Todo pero el profe es un putero i no lo hace
					break;
				}
			default:
				break;
			}
		}
	}
}
