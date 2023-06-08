#include <stdio.h>
#include <iostream>
#include <string.h>
#ifndef WIN32
#include <errno.h> 
#include <unistd.h>  
#include <arpa/inet.h>   
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
#include <pthread.h>
#else
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#endif

int client_socket;

void* readHandler(void* arg)		
{
	while (1)
	{
		int message_size;
		recv(client_socket, (char*)&message_size, sizeof(int), 0);
		char* msg2 = new char[message_size + 1];
		msg2[message_size] = '\0';
		recv(client_socket, msg2, message_size, 0);
		std::cout << "Received: " << msg2 << "\n";
		delete[] msg2;
	}
}

int main()
{
#ifdef WIN32
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		std::cout << "Error loading library\n";
		exit(1);
	}
#endif

	sockaddr_in server_addr;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(55555);
	server_addr.sin_family = AF_INET;

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
	{
		std::cout << "Error creating socket\n";
		exit(1);
	}

	int connection = connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr));
	if (connection == -1)
	{
		std::cout << "Error creating connection\n";
		exit(1);
	}
	else
	{
		std::cout << "Connected\n";
	}

	pthread_t rd_thread;
	pthread_create(&rd_thread, 0, readHandler, 0);
	pthread_detach(rd_thread);

	char msg[256];
	while (1)
	{
		memset(msg, 0, sizeof(msg));
		std::cin.getline(msg, sizeof(msg));
		if ((strncmp(msg, "end", 3)) == 0)
		{
			std::cout << "Client Exit\n";
			break;
		}
		send(client_socket, msg, sizeof(msg), 0);
	}

#ifdef WIN32
	closesocket(client_socket);
	WSACleanup();
#else
	close(client_socket);
#endif
	return 0;
}