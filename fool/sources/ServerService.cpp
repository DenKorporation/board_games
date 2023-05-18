#include "ServerService.h"

std::string ServerService::StringIp = "127.0.0.1";
sf::IpAddress ServerService::Ip = sf::IpAddress("127.0.0.1");
unsigned short ServerService::Port = 5050;

ServerService::ServerService() : tcpSocket()
{
}

bool ServerService::setIp(const std::string &ip)
{
	sf::IpAddress address = sf::IpAddress(ip);
	if (address != sf::IpAddress::None)
	{
		StringIp = ip;
		Ip = address;
		return true;
	}
	return false;
}

// port and unsigned short is 2 bytes
bool ServerService::setPort(unsigned short port)
{
	Port = port;
	return true;
}

std::string ServerService::getIp()
{
	return StringIp;
}

unsigned short ServerService::getPort()
{
	return Port;
}

bool ServerService::Connect()
{
	return tcpSocket.connect(Ip, Port) == sf::Socket::Done;
}

bool ServerService::Connect(std::string ip, unsigned short port)
{
	return tcpSocket.connect(ip, port) == sf::Socket::Done;
}

void ServerService::Send(json message)
{
	std::string stringMessage = message.dump();
	char end = '\n';
	tcpSocket.send(stringMessage.data(), stringMessage.length());
	tcpSocket.send(&end, 1);
}

json ServerService::Receive()
{
	std::string message = "";
	char data[255];
	std::size_t received = -1;

	while (received != 0)
	{
		sf::Socket::Status status = tcpSocket.receive(data, 255, received);

		if (status == sf::Socket::Done)
		{
			message.append(data, received);
		}
		else if (message.empty())
		{
			return nullptr;
		}
	}

	return json::parse(message);
}

void ServerService::Disconnect()
{
	tcpSocket.disconnect();
}
