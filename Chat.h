#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <SFML/Network.hpp>
#include <vector>
#include <list>

#include "ConsoleControl.h"


class Chat
{
private:
	
	std::vector<std::string> _messages;
	std::mutex _messagesMutex;

	sf::IpAddress _serverAddress;
	
	Chat() {};

	void ShowMessage(std::string);
	void ShowWarning(std::string);
	void ShowError(std::string);

	void ListenClientsConnections(unsigned short port);

protected:


public:
	static Chat* Server(unsigned short port);
	static Chat* Client(unsigned short port, std::string ip);
};

