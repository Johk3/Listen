#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <algorithm>


#pragma comment (lib, "ws2_32.lib")


std::string parse(std::string input) {
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	return input;
}

std::string process(std::string message) {
	if (message == "lul") {
		std::cout << "Works" << std::endl;
	}
	return "";
}

void server() {
	// Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		std::cerr << "Can't Initialize winsock! Quitting...." << std::endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "Invalid socket!" << std::endl;
		return;
	}

	// Bind the socket to an ip address and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	std::cout << "PORT: ";
	std::cout << ntohs(hint.sin_port) << std::endl;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)& client, &clientSize);

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)& client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		std::cout << host << " Connected on port " << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " Connected on port " << ntohs(client.sin_port) << std::endl;
	}

	// Close listening socket 
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];

	while (true) {
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR) {
			std::cerr << "Error in recv. Quitting..." << std::endl;
			break;
		}

		if (bytesReceived == 0) {
			std::cout << "Client disconnected" << std::endl;
			break;
		}

		// Echo message back to client
		std::string message = parse(buf);
		std::cout << message << std::endl;
		std::string outmessage;
		std::cout << "<Server>: ";
		std::getline(std::cin, outmessage);
		std::cout << bytesReceived << std::endl;

		int outBytesReceived = outmessage.length();

		send(clientSocket, outmessage.c_str(), outBytesReceived, 0);
	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();
}