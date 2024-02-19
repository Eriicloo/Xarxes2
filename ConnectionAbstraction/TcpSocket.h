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

	typedef std::function<void(Packet packet)> OnRecievePacket;
	typedef std::function<void(TcpSocket* socket)> OnSocketDisconnect;

	bool Connect(std::string ip, unsigned short port);
	void Recieve();
	bool Send(Packet::PacketKey key);
	bool Send(Packet::PacketKey key, Packet packet);

	void Subscribe(Packet::PacketKey key, OnRecievePacket onRecievePacket);
	void SubscribeAsync(Packet::PacketKey key, OnRecievePacket onRecievePacket);

	void SubscribeOnDisconnect(OnSocketDisconnect onSocketDisconnect);

private:
	
	std::map<Packet::PacketKey, OnRecievePacket> _subscriptions;
	std::mutex _subscriptionsMutex;

	std::vector<OnSocketDisconnect> _onSocketDisconnectList;
	std::mutex _onSocketDisconnectMutex;

	void ProcessPacket(Packet packet);
};

