#pragma once
#include <list>
#include <mutex>

#include "Button.h"
#include "Image.h"

class Window
{
public:

	typedef std::function<void()> MainThreadTask;

	/// <summary>
	/// Recordar que el constructor crea la ventana y que no se puede 
	/// tocar la ventana desde otro thread que no sea el que la ha creado.
	/// </summary>
	Window(unsigned int width = 512, unsigned int height = 512, std::string title = "Abunda la window");



	void AddButton(Button* button);
	void AddImage(Image* image);

	void AddTask(MainThreadTask task);


	//Sistema de gestio de accions en el thread
	//Finestra en el teu cicle de vida normal, executa aquesta funcio. La finestra s'esta executant en el thread que toca.
	//Quan executem aquesta funcio, 
	void RunWindowsLoop();

private:

	std::mutex _mutex;
	sf::RenderWindow _window;

	Button* _lastClickedDownButton;
	std::list <Button*> _buttons;
	std::list <Image*> _images;
	std::list <sf::Drawable*> _objectsToDraw;
	
	//Una task es qualsevol tipus de accio relacionada amb el renderitzat. 
	//Ex: Afegir un boto, moure un peça, eliminar una peça, crear un peça, cambiar una peça, crear el tablero/grid, etc.
	//Una task seria addtask [](Reina, Window) { el codi que hagi de fer la tasca }
	MainThreadTask _mainThreadTask = [](){};


	std::vector<MainThreadTask> _tasks;
	std::mutex _tasksMutex;

};

