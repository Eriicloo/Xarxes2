#pragma once
#include <list>
#include <mutex>

#include "Button.h"

class Window
{
public:

	/// <summary>
	/// Recordar que el constructor crea la ventana y que no se puede 
	/// tocar la ventana desde otro thread que no sea el que la ha creado.
	/// </summary>
	Window(unsigned int width = 600, unsigned int height = 600, std::string title = "Abunda la window");



	void AddButton(Button* button);

	//Sistema de gestio de accions en el thread
	//Finestra en el teu cicle de vida normal, executa aquesta funcio. La finestra s'esta executant en el thread que toca.
	//Quan executem aquesta funcio, 
	void RunWindowsLoop();

private:

	std::mutex _mutex;
	sf::RenderWindow _window;

	Button* _lastClickedDownButton;
	std::list <Button*> _buttons;
	std::list <sf::Drawable*> _objectsToDraw;



};

