#include "SocketsManager.h"

SocketsManager::SocketsManager(OnSocketConnected onSocketConnected)
{
	_onSocketConnected = onSocketConnected;
}

SocketsManager::~SocketsManager()
{
	delete _listener;

	for (TcpSocket* socket : _sockets)
	{
		delete socket;
	}
}

void SocketsManager::StartLoop()
{
	_isRunningMutex.lock();

	if (_isRunning) //Prevent multiple loops
	{
		_isRunningMutex.unlock();
		return;
	}

	_isRunning = true;

	_isRunningMutex.unlock();

	std::thread* loopThread = new std::thread(&SocketsManager::SelectorLoop, this);
	loopThread->detach();
}

bool SocketsManager::StartListener(unsigned short port)
{
	_listenerMutex.lock();

	if (_listener != nullptr)
	{
		_listenerMutex.unlock(); //Prevent multiple listeners
		return false;
	}

	//Esto nos sirve para hacer el Peer to peer
	_listener = new TcpListener();
	if (!_listener->Listen(port))//TODO
	{
		delete _listener;
		_listenerMutex.unlock();
		return false;
	}

	_selector.Add(*_listener);//TODO

	_listenerMutex.unlock();
	return true;
}

bool SocketsManager::ConnectToServer(std::string ip, unsigned short port)
{
	TcpSocket* socket = new TcpSocket();
	if (socket->Connect(ip, port))
	{
		delete socket;
		return false;
	}

	AddSocket(socket);

	return true;
}

void SocketsManager::SelectorLoop()
{
	_isRunningMutex.lock();
	bool isRunning = _isRunning;
	_isRunningMutex.unlock();
	
	while (isRunning)
	{
		if (_selector.Wait())
		{
			CheckListener();
			CheckSockets();
		}

		_isRunningMutex.lock();
		isRunning = _isRunning;
		_isRunningMutex.unlock();
	}
}

void SocketsManager::CheckListener()
{
	_listenerMutex.lock();

	if (_listener != nullptr && _selector.IsReady(*_listener))
	{
		TcpSocket* socket = new TcpSocket(); 
		if (_listener->Accept(*socket))
		{
			AddSocket(socket);
		}
		else
		{
			delete socket;
			//TODO Gestionar el error, show error i todo eso by Pedro
		}
	}

	_listenerMutex.unlock();
}

void SocketsManager::CheckSockets()
{
	_socketsMutex.lock();

	for (TcpSocket* socket : _sockets) 
	{
		if (_selector.IsReady(*socket))//TODO
		{
			socket->Receive();//TODO
		}
	}

	_socketsMutex.unlock();
}

void SocketsManager::AddSocket(TcpSocket* socket)
{
	_socketsMutex.lock();

	_sockets.push_back(socket);
	_selector.Add(*socket); //TODO

	_onSocketConnected(socket);

	//TODO
	socket->SubscribeOnDisconnect([this](TcpSocket* socket) 
		{
			//This is done async to remove it in another thread to to all the necesary things and then delete it
			RemoveSocketAsync(socket);
		});

	_socketsMutex.unlock();
}

void SocketsManager::RemoveSocket(TcpSocket* socket)
{
	_selector.Remove(*socket);//TODO
	_socketsMutex.lock();

	//Podemos hacer remove porque es una lista i no un vector
	_sockets.remove(socket);

	_socketsMutex.unlock();

	delete socket;
}

void SocketsManager::RemoveSocketAsync(TcpSocket* socket)
{
	std::thread* removeSocketThread = new std::thread(&SocketsManager::RemoveSocket, this, socket);
	removeSocketThread->detach();
}
