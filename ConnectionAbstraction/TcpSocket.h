#pragma once
#include <functional>
#include <map>
#include <vector>
#include <thread>
#include <mutex>

#include "Packet.h"

class TcpSocket: public sf::TcpSocket
{
public:

	typedef std::function<void(Packet packet)> OnReceivePacket;
	typedef std::function<void(TcpSocket* socket)> OnSocketDisconnect;

	bool Connect(std::string ip, unsigned short port);
	void Receive();
	bool Send(Packet::PacketKey key);
	bool Send(Packet::PacketKey key, Packet packet);

	void Subscribe(Packet::PacketKey key, OnReceivePacket onRecievePacket);
	void SubscribeAsync(Packet::PacketKey key, OnReceivePacket onRecievePacket);

	void SubscribeOnDisconnect(OnSocketDisconnect onSocketDisconnect);

private:
	
	std::map<Packet::PacketKey, OnReceivePacket> _subscriptions;
	std::mutex _subscriptionsMutex;

	std::vector<OnSocketDisconnect> _onSocketDisconnectList;
	std::mutex _onSocketDisconnectMutex;

	void ProcessPacket(Packet packet);
};

