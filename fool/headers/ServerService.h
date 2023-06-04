#include <SFML/Network.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ServerService
{
private:
	static std::string StringIp;
	static sf::IpAddress Ip;
	static unsigned short Port;

public:
	ServerService();

	static bool setIp(const std::string &ip);
	static bool setPort(unsigned short port);

	static std::string getIp();
	static unsigned short getPort();

	bool Connect();
	bool Connect(std::string ip, unsigned short port);
	void Send(json message);
	json Receive();
	void Disconnect();

private:
	sf::TcpSocket tcpSocket;
	std::string restData;
};