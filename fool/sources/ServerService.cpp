#include "ServerService.h"

#include <iostream>

json ServerService::Receive()
{
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect("127.0.0.1", 5050);
	std::string message = "";

	if (status == sf::Socket::Done)
	{
		char data[255];
		std::size_t received = -1;

		while (received != 0)
		{
			if (socket.receive(data, 255, received) == sf::Socket::Done)
			{
				message.append(data, received);
			}
		}

		socket.disconnect();
		return json::parse(message);
	}
	return nullptr;
}
