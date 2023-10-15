#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

	cout << "\n Welcome to server socket app";
	/*
	* Initialise the windows socket library.
	*/
	WSADATA  wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "\n FAILED : WSAStartup failed.";
		return 1;
	}
	cout << "\n SUCCESS : Winsock library initialised";
	/*
	* Request for an socket from socket api.
	* Pass the arguments as per your type of socket requirement to socket function.
	* 1st parameter is to tell the address family that is going to be used
	*   1)AF_INET -> IPv4 address
	*   2)AF_INET6 -> IPv6 address
	* 2nd paramter is to tell what type of socket is needed
	*   1)SOCK_STREAM (Stream Sockets) uses TCP protocol
	*   2)SOCK_DGRAM (Datagram Sockets) uses UDP protocol
	* 3rd parameter is to tell the protocol to be used.
	*/
	SOCKET   listeningSocket;
	listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningSocket == INVALID_SOCKET) {
		cerr << "\n FAILED : " << WSAGetLastError() << ", Could not create socket.";
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}
	cout << "\n SUCCESS : Socket created";
	/*
	* Store the details of the Port used along with the IP address family
	* in an SOCKADDR_IN object.
	*/
	const int PORT = 5757; //Port number at which this socket application will be running.
	SOCKADDR_IN   serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	// Pass the IP address of the server
	// Use inet_pton to convert the IP address from human-readable to binary format
	if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
		cerr << "\n Invalid IP address.";
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}
	/*
	* Bind the socket to the SOCKADDR_IN object created in above code.
	*/
	if (bind(listeningSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		cerr << "Bind failed: " << WSAGetLastError();
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}
	/*
	* Initiate a listener to listen for any connection from clients.
	*/
	int listenResult = listen(listeningSocket, SOMAXCONN);
	if (listenResult == SOCKET_ERROR) {
		cerr << "\n FAILED : " << WSAGetLastError() << ", Listening failed.";
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}
	cout << "\n Waiting for a client to connect...";
	/*
	* Create an socket to store client socket information after client connects with server.
	*/
	SOCKET clientSocket;
	SOCKADDR_IN clientAddress;
	int clientAddressSize = sizeof(clientAddress);
	clientSocket = accept(listeningSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "\n FAILED : " << WSAGetLastError() << ", Accept failed.";
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}
	cout << "\n SUCCESS: Client connected.";
	cout << "\n IP Address of client is : " << ;
	/*
	* Receive data from client and send data to client.
	*/
	char buffer[1024];
	int bytesReceived;
	while (true) {
		//Receive data.
		bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesReceived <= 0) {
			// The client has disconnected or an error occurred.
			closesocket(clientSocket);
			break;
		}
		buffer[bytesReceived] = '\0';
		cout << "\n Received from client : " << buffer;
		// Send a response to the client.
		const char* response = "I'm great! What about you?";
		send(clientSocket, response, strlen(response), 0);
	}
	//close socket
	closesocket(listeningSocket);
	WSACleanup();

	return 0;

}