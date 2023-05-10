#include "ServerService.h"

sf::IpAddress ServerService::Ip = sf::IpAddress("127.0.0.1");
unsigned short ServerService::Port = 5050;

ServerService::ServerService() : tcpSocket()
{
}

bool ServerService::Connect()
{
	return tcpSocket.connect(Ip, Port) == sf::Socket::Done;
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
