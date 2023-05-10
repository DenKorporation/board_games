#include <SFML/Network.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ServerService
{
public:
	static sf::IpAddress Ip;
	static unsigned short Port;

public:
	ServerService();
	bool Connect();
	void Send(json message);
	json Receive();
	void Disconnect();

private:
	sf::TcpSocket tcpSocket;
};