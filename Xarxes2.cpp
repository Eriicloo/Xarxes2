

#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "Chat.h"
#include "ConnectionAbstraction/SocketsManager.h"
#include "WindowsHelpers/Window.h"

void RunClient();
void RunServer();
void RunWindows();

unsigned short port = 3000;

enum PackageKey : Packet::PacketKey {Message = 0};

int main()
{
	std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;
	
	char mode = ' ';

	RunWindows();


	do 
	{
		
		mode = ConsoleControl::WaitForReadNextChar();
		
	} while (mode != 'C' && mode != 'c' && mode != 'S' && mode != 's');

	switch (mode)
	{
	case 'C': 
	case 'c':
	{
		RunClient();
		break;
	}
	case 'S':
	case 's':
	{
		RunServer();
		break;
	}
	default:
		break;
	}


	while (true)
	{

	}
}

void RunClient()
{
	std::cout << "Client";
	std::cout << std::endl << "Set server IP --> ";

	std::string ip;
	std::getline(std::cin, ip);

	SocketsManager* SM = new SocketsManager([](TcpSocket* socket)
	{
		std::cout << std::endl << "Socket connected: " << socket->getRemoteAddress().toString();

		socket->Subscribe(Message, [socket](Packet packet)
		{
			std::string message;
			packet >> message;
			std::cout << std::endl << "New Message: " << message;

		});

		socket->SubscribeOnDisconnect([](TcpSocket* socket)
		{
			std::cout << std::endl << "Socket disconnected: " << socket->getRemoteAddress().toString();
		});

		std::string response = "Soy cliente";
		Packet packet;
		packet << response;

		socket->Send(Message, packet);

	});

	if (SM->ConnectToServer(ip, port))
	{
		SM->StartLoop();
	}
}

void RunServer()
{
	std::cout << "Server";

	SocketsManager* SM = new SocketsManager([](TcpSocket* socket)
	{
		std::cout << std::endl << "Socket connected: " << socket->getRemoteAddress().toString();

		socket->Subscribe(Message, [socket](Packet packet) 
		{
			std::string message;
			packet >> message;
			std::cout << std::endl << "New Message: " << message;

			std::string response = "Soy server";
			Packet responsePacket;
			responsePacket << response;

			socket->Send(Message, responsePacket);
		});

		socket->SubscribeOnDisconnect([](TcpSocket* socket) 
		{
				std::cout << std::endl << "Socket disconnected: " << socket->getRemoteAddress().toString();
		});
		
	});

	if (SM->StartListener(port)) 
	{
		sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
		std::cout << "Listening on IP: " << ipAddress.toString();
		SM->StartLoop();
	}

}

void RunWindows()
{
	Window window;

	Button* button = new Button(50, 20, "Piezas/QG.png");
	button->onClick = []() 
		{
			std::cout << std::endl << "Long Live the Queen";
		};

	window.AddButton(button);
	window.RunWindowsLoop();

	/*sf::RenderWindow windows;

	windows.create(sf::VideoMode(800, 600), "Chat");
	windows.setFramerateLimit(60);

	sf::Text label;

	sf::Font font;
	font.loadFromFile("Minecraft.ttf");

	label.setFont(font);
	label.setCharacterSize(16);
	label.setFillColor(sf::Color::Cyan);
	label.setString("Mamamelo no mamaba");
	label.setOrigin(0.5, 0.5);

	label.setPosition(windows.getSize().x * 0.5, windows.getSize().y * 0.5);


	while (windows.isOpen())
	{
		windows.clear(sf::Color::Black);
		windows.draw(label);
		windows.display();
	}*/
}
