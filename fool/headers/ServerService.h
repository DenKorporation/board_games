#include <SFML/Network.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ServerService
{
public:
	static void Connect();
	static void Send();
	static json Receive();

private:
	static sf::TcpSocket tcpSocket;
};