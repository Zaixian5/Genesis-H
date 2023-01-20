#define _CRT_SECURE_NO_WARNINGS //scanf()의 보안경고를 무시, 반드시 맨 앞줄에 와야 함.
#include <stdio.h>
#include <stdbool.h> //true, false를 인자로 사용가능.
#include <conio.h>
#include <Windows.h>

void gotoxy(int, int);

int main(void)
{
	// Sleep(1000): <Windows.h>에 포함. ms마다 프로그램의 진행을 일시정지함.
	// System("cls"): <Windows.h>에 포함. 콘솔창의 모든 내용을 지운다.
	// _kbhit(): <conio.h>에 포함. 키보드의 입력유무를 검사.(키보드 입력이 있을 때 실행하고 없을 때는 있을 때까지 기다려야 하므로 무한반복문과 함께 구성)
	// _getch(): <conio.h>에 포함. 입력 키 값을 즉시 반환(사용자가 엔터를 칠 때 까지 기다리지 않는다.)
	// gotoxy(): <conio.h>와 <Windows.h>를 모두 사용해야 함. 콘솔상의 좌표를 옮겨주는 함수를 정의 할 것이다.
    
	/* gotoxy()의 정의:
	   
    void gotoxy(int x,int y)
    {
        COORD pos={x,y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
    }
	// 콘솔상의 위치를 x, y로 옮겨준다.
    */

	char szChoice = 0;
	int x = 0, y = 0;

	while (true)
	{
		gotoxy(x, y);
		printf("@");
		if (_kbhit()) //키보드의 입력이 있다면 조건문을 실행하여,
		{
			szChoice = _getch(); //그 입력 값을 저장한다.
			switch (szChoice) // wsad방식 게임
			{
			case 'W':  
			case 'w':
				y--;
				break;
			case 'S':
			case 's':
				y++;
				break;
			case 'A':
			case 'a':
				x--;
				break;
			case 'D':
			case 'd':
				x++;
				break;
			}
			Sleep(500); //캐릭터의 좌표이동 속도를 키를 입력 받고 0.5초 후로 한다.
			system("cls"); //좌표가 이동되면 콘솔을 지워, 이전 위치의 캐릭터가 없어진다.
		}
	}
	return 0;
}
void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}