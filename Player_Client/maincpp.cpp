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


//서버에서 받은 맵 저장할 배열
char Map[10][10] = {};

//플레이어 이동 및 렌더하는 함수
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

	//송신용
	Player MyPlayer;
	char SendBufferLocation[1024] = {};
	char Code = ' ';

	//수신용
	char RecvBuffer[1024] = {};
	int RecvLengh = 0;
	int TotalRecvLength = 0;
	
	int AddLocationX = 0;
	int AddLocationY = 0;
	int InputKey;
	
	bool Running = true;


	//서버에 있는 맵 받음
	for (int Y = 0; Y < 10; ++Y)
	{
		for (int X = 0; X < 10; ++X)
		{


			recv(ServerSocket, &Map[Y][X], 1, 0);
		}
	}


	//맵 시작 위치 랜덤 설정
	int MapLacationX, MapLacationY;
	srand(static_cast<unsigned int>(time(nullptr)));
	MapLacationX = rand() % 8 + 1;
	MapLacationY = rand() % 8 + 1;

	//맵 그리기 / 플레이어 위치(커서위치) 조정
	Draw(MyPlayer, MapLacationX, MapLacationY);
	SetLocation(MyPlayer.X, MyPlayer.Y);

	
	while (Running)
	{
		
		//플레이어 이동 입력

		InputKey = _getch();
		//memcpy(SendBufferLocation, &InputKey, 4);
		
		if (InputKey == 27)
		{
			Running = false;
			break;
		}

		Code = (char)InputKey;

		
		
		int SentLength = send(ServerSocket, &Code, 1, 0);

		//현재 위치에서 Add 되는 좌표값 받기
		do
		{

			RecvLengh = recv(ServerSocket, &RecvBuffer[TotalRecvLength], sizeof(RecvBuffer) - TotalRecvLength, 0);
			//printf("RecvLength : %d\n", RecvLengh);
			TotalRecvLength += RecvLengh;

		} while (TotalRecvLength < 8);


		//이동해야할 값 변환해서 저장
		memcpy(&AddLocationX, &RecvBuffer[0], 4);
		memcpy(&AddLocationY, &RecvBuffer[4], 4);



		//플레이어 이동시키기
		MovePlayer(MyPlayer, AddLocationX, AddLocationY);


		
		//초기화
		TotalRecvLength = 0;
		RecvLengh = 0;
	}

	system("cls");

	closesocket(ServerSocket);

	WSACleanup;



	return 0;

}



//커서 숨기기
void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//플레이어 위치(지정된 좌표값을 수정해줌)
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
		//플레이어 현재 위치로 가서 공백출력(플레이어 지우기)
		SetLocation(PlayerLocation.X, PlayerLocation.Y);
		cout << ' ';

		//이동한위치로 플레이어 출력

		SetLocation(PlayerLocation.X + AddX, PlayerLocation.Y + AddY);
		cout << 'P';


		//이동한 위치 좌표값을 현재 위치로 적용
		PlayerLocation.X += AddX;
		PlayerLocation.Y += AddY;

		PlayerLocation.yIndex += AddY;
		PlayerLocation.xIndex += AddX;

	}

}

void Draw(Player& PlayerLocation, int MapLocationX, int MapLocationY)
{



	//맵이동을 얼마나 했는지 값을 미리 저장. 이후에 값이 변하기때문
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

				//인덱스 값 저장
				PlayerLocation.xIndex = X;
				PlayerLocation.yIndex = Y;


				//변경한 맵 위치만큼 플레이어 위치값 변경
				PlayerLocation.X = X + MapLocationX;
				PlayerLocation.Y = Y + MapLocationY;



				cout << 'P';
			}



		}

		cout << endl;
	}

}