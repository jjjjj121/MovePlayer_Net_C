#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <iostream>
#include <conio.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;


class Player
{
public:
	int X;
	int Y;
	int xIndex;
	int yIndex;

public:
	Player() 
	{
		X = 0;
		Y = 0;
		xIndex = 0;
		yIndex = 0;
	}
	~Player() {}
};


//�������� ���� �� ������ �迭
char Map[10][10] = {};

//�÷��̾� �̵� �� �����ϴ� �Լ�
void SetLocation(int xPlayerLocation, int yPlayerLocation);
void MovePlayer(Player& PlayerLocation, int AddX, int AddY);
void Draw(Player& PlayerLocation, int MapLocationX, int MapLocationY);



int main()
{
	WSAData Winsock;
	WSAStartup(MAKEWORD(2, 2), &Winsock);



	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN ServerSockAddr;

	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(5001);
	ServerSockAddr.sin_family = AF_INET;

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(SOCKADDR_IN));

	//�۽ſ�
	Player MyPlayer;
	char SendBufferLocation[1024] = {};
	char Code = ' ';

	//���ſ�
	char RecvBuffer[1024] = {};
	int RecvLengh = 0;
	int TotalRecvLength = 0;
	
	int AddLocationX = 0;
	int AddLocationY = 0;
	int InputKey;
	
	bool Running = true;


	//������ �ִ� �� ����
	for (int Y = 0; Y < 10; ++Y)
	{
		for (int X = 0; X < 10; ++X)
		{


			recv(ServerSocket, &Map[Y][X], 1, 0);
		}
	}


	//�� ���� ��ġ ���� ����
	int MapLacationX, MapLacationY;
	srand(static_cast<unsigned int>(time(nullptr)));
	MapLacationX = rand() % 8 + 1;
	MapLacationY = rand() % 8 + 1;

	//�� �׸��� / �÷��̾� ��ġ(Ŀ����ġ) ����
	Draw(MyPlayer, MapLacationX, MapLacationY);
	SetLocation(MyPlayer.X, MyPlayer.Y);

	
	while (Running)
	{
		
		//�÷��̾� �̵� �Է�

		InputKey = _getch();
		//memcpy(SendBufferLocation, &InputKey, 4);
		
		if (InputKey == 27)
		{
			Running = false;
			break;
		}

		Code = (char)InputKey;

		
		
		int SentLength = send(ServerSocket, &Code, 1, 0);

		//���� ��ġ���� Add �Ǵ� ��ǥ�� �ޱ�
		do
		{

			RecvLengh = recv(ServerSocket, &RecvBuffer[TotalRecvLength], sizeof(RecvBuffer) - TotalRecvLength, 0);
			//printf("RecvLength : %d\n", RecvLengh);
			TotalRecvLength += RecvLengh;

		} while (TotalRecvLength < 8);


		//�̵��ؾ��� �� ��ȯ�ؼ� ����
		memcpy(&AddLocationX, &RecvBuffer[0], 4);
		memcpy(&AddLocationY, &RecvBuffer[4], 4);



		//�÷��̾� �̵���Ű��
		MovePlayer(MyPlayer, AddLocationX, AddLocationY);


		
		//�ʱ�ȭ
		TotalRecvLength = 0;
		RecvLengh = 0;
	}

	system("cls");

	closesocket(ServerSocket);

	WSACleanup;



	return 0;

}



//Ŀ�� �����
void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //Ŀ�� ���� (1 ~ 100)
	cursorInfo.bVisible = FALSE; //Ŀ�� Visible TRUE(����) FALSE(����)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//�÷��̾� ��ġ(������ ��ǥ���� ��������)
void SetLocation(int xPlayerLocation, int yPlayerLocation)
{
	COORD pos = { xPlayerLocation ,yPlayerLocation };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}

void MovePlayer(Player& PlayerLocation, int AddX, int AddY)
{

	char TempMap = Map[PlayerLocation.yIndex + AddY][PlayerLocation.xIndex + AddX];
	if (TempMap == '1')
	{
		//�÷��̾� ���� ��ġ�� ���� �������(�÷��̾� �����)
		SetLocation(PlayerLocation.X, PlayerLocation.Y);
		cout << ' ';

		//�̵�����ġ�� �÷��̾� ���

		SetLocation(PlayerLocation.X + AddX, PlayerLocation.Y + AddY);
		cout << 'P';


		//�̵��� ��ġ ��ǥ���� ���� ��ġ�� ����
		PlayerLocation.X += AddX;
		PlayerLocation.Y += AddY;

		PlayerLocation.yIndex += AddY;
		PlayerLocation.xIndex += AddX;

	}

}

void Draw(Player& PlayerLocation, int MapLocationX, int MapLocationY)
{



	//���̵��� �󸶳� �ߴ��� ���� �̸� ����. ���Ŀ� ���� ���ϱ⶧��
	//int xTemp = MapLacationX;
	//int yTemp = MapLacationY;

	int TempX, TempY;


	for (int Y = 0; Y < 10; ++Y)
	{
		for (int X = 0; X < 10; ++X)
		{
			char Temp = Map[Y][X];

			if (Temp == '0')
			{
				TempX = X + MapLocationX;
				TempY = Y + MapLocationY;
				SetLocation(TempX, TempY);
				cout << '#';
			}
			else if (Temp == '1')
			{
				TempX = X + MapLocationX;
				TempY = Y + MapLocationY;
				SetLocation(TempX, TempY);
				cout << ' ';
			}
			else if (Temp == '2')
			{
				TempX = X + MapLocationX;
				TempY = Y + MapLocationY;
				SetLocation(TempX, TempY);

				//�ε��� �� ����
				PlayerLocation.xIndex = X;
				PlayerLocation.yIndex = Y;


				//������ �� ��ġ��ŭ �÷��̾� ��ġ�� ����
				PlayerLocation.X = X + MapLocationX;
				PlayerLocation.Y = Y + MapLocationY;



				cout << 'P';
			}



		}

		cout << endl;
	}

}