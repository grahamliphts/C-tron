#include <WinSock2.h>
#define DEFAULT_PORT "27015"
#define IP_ADDRESS "127.0.0.1"

class Client
{
	public:
		Client();
		~Client();
		int createSocket();
		int sendToServer(char *text);

	private:
		struct addrinfo *result;
		SOCKET ConnectSocket;
}; 
