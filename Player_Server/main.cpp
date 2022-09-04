#include <winsock2.h>
#include <stdio.h>
#include <iostream>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

//��
char Map[10][10] = {
	{'0','0','0','0','0','0','0','0','0','0'},
	{'0','1','1','1','1','1','1','1','1','0'},
	{'0','1','2','1','1','1','1','1','1','0'},
	{'0','1','1','1','1','1','0','1','1','0'},
	{'0','1','1','1','1','1','1','1','1','0'},
	{'0','1','1','0','1','1','1','1','1','0'},
	{'0','1','1','1','1','1','1','1','1','0'},
	{'0','1','1','1','1','1','1','1','1','0'},
	{'0','1','1','1','1','1','1','1','1','0'},
	{'0','0','0','0','0','0','0','0','0','0'},
};


int main()
{
	WSAData Winsock;
	WSAStartup(MAKEWORD(2, 2), &Winsock);


	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(SOCKADDR_IN));

	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_port = htons(5001);
	ServerSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);


	bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(SOCKADDR_IN));
	listen(ServerSocket, 0);


	SOCKET ClientSocket;
	SOCKADDR_IN ClientSockAddr;
	int ClientAddrLength = sizeof(ClientSockAddr);

	ClientSocket = accept(ServerSocket, (SOCKADDR*)&ServerSockAddr, &ClientAddrLength);

	//�۽ſ�
	char SendBuffer[1024] = {};

	//���ſ�
	char RecvCode = ' ';
	int RecvLengh = 0;
	int TotalRecvLength = 0;



	int PlayerInputKey = 0;
	int PlayerX = 0;
	int PlayerY = 0;


	bool Running = true;

	for (int Y = 0; Y < 10; ++Y)
	{
		for (int X = 0; X < 10; ++X)
		{


			int SentLength = send(ClientSocket, &Map[Y][X], 1, 0);
		}
	}


	while(Running)
	{

		//char�� �޾Ƽ� ��ȯ���� ���.
		RecvLengh = recv(ClientSocket, &RecvCode, 1, 0);

		if (RecvLengh == 0)
		{
			printf("Disconnected Client.");
			Running = false;
			break;

		}
		else if (RecvLengh < 0)
		{
			printf("Disconnected Client By Error : %d\n", GetLastError());
			Running = false;
			break;

		}




		//�÷��̾� ��ġ ����
		switch (RecvCode)
		{
			//A ������ ��
		case('a'):
		case('A'):
			--PlayerX;
			break;
			//S ������ ��
		case('s'):
		case('S'):
			++PlayerY;
			break;
			//D ������ ��
		case('d'):
		case('D'):
			++PlayerX;
			break;
			//W ������ ��
		case('w'):
		case('W'):
			--PlayerY;
			break;
		case 27:
			Running = false;
			break;

		}


		memcpy(&SendBuffer, &PlayerX, 4);
		memcpy(&SendBuffer[4], &PlayerY, 4);

		printf("Player X : %d\n", PlayerX);
		printf("Player Y : %d\n", PlayerY);

		//send(ClientSocket, SendBuffer, 8, 0);
		for (int i = 0; i < 8; ++i)
		{
			int SentLength = send(ClientSocket, &SendBuffer[i], 1, 0);

		}


		//�ʱ�ȭ
		TotalRecvLength = 0;
		RecvLengh = 0;
		PlayerX = 0;
		PlayerY = 0;
		//memset(RecvBuffer, 0, sizeof(RecvBuffer));
	}


	closesocket(ClientSocket);
	closesocket(ServerSocket);


	WSACleanup;

	return 0;
}


