#include "Chat.h"

void Chat::ShowMessage(std::string message)
{
	_messagesMutex.lock();
	_messages.push_back(message);
	_messagesMutex.unlock();

	ConsoleControl::LockMutex();
	std::cout << std::endl << message;
	ConsoleControl::UnlockMutex();
}

void Chat::ShowWarning(std::string warning)
{
	ConsoleControl::LockMutex();
	ConsoleControl::SetColor(ConsoleControl::RED);
	std::cout << std::endl << warning;
	ConsoleControl::SetColor(ConsoleControl::WHITE);
	ConsoleControl::UnlockMutex();
}

void Chat::ShowError(std::string error)
{
	ConsoleControl::LockMutex();
	ConsoleControl::SetColor(ConsoleControl::YELLOW);
	std::cout << std::endl << error;
	ConsoleControl::SetColor(ConsoleControl::WHITE);
	ConsoleControl::UnlockMutex();
}

void Chat::ListenClientsConnections(unsigned short port)
{
	sf::TcpListener listener;

	if (listener.listen(port) != sf::Socket::Done)
	{
		std::cout << std::endl << "Error on Start Listener";
		return;
	}

	sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
	std::cout << std::endl << "Listening on IP: " + ipAddress.toString();

	sf::TcpSocket client;

	if (listener.accept(client) != sf::Socket::Done)
	{
		std::cout << std::endl << "Error on accept Client";
		return;
	}


}

Chat* Chat::Server(unsigned short port)
{
	Chat* chat = new Chat();
	chat->_serverAddress = sf::IpAddress::getLocalAddress();

	

	return chat;
}

Chat* Chat::Client(unsigned short port, std::string ip)
{
	Chat* chat = new Chat();
	


	return chat;
}
