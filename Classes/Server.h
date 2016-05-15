#include <WinSock2.h>

class Server
{
	public:
		Server();
		~Server();
		int createSocket();
	private:
		struct addrinfo *result;
};