#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <Windows.h>

/*매크로 정의*/
#define FIELD_WIDTH 22 // 2차원 맵의 열
#define Field_Height 18 // 2차원 맵의 행
#define LEFT_CORNER 1 // 맵의 왼쪽 끝
#define RIGHT_CORNER (FIELD_WIDTH - 2) // 맵의 오른쪽 끝
#define UP_CORNER 1 // 맵의 위쪽 끝
#define DOWN_CORNER (Field_Height - 2) // 맵의 아래쪽 끝

#define MAP_WALL '#' // 벽
#define MAP_PLAYER '@' // 플레이어 캐릭터
#define MAP_ENEMY '&' // 적 캐릭터
#define MAP_EMPTY ' ' // 맵의 공백

#define DIR_LEFT -1 // 캐릭터가 왼쪽으로 이동할 때 좌표변경시 이용
#define DIR_RIGHT 1 // 캐릭터가 오른쪽으로 이동할 때 좌표변경시 이용
#define DIR_UP -1 // 캐릭터가 위로 이동할 때 좌표변경시 이용
#define DIR_DOWN 1 // 캐릭터가 아래로 이동할 때 좌표변경시 이용

#define K_ARROW 224 // 방향키 아스키코드
#define K_UP 72 // 윗 방향키
#define K_DOWN 80 // 아래 방향키
#define K_LEFT 75 // 왼쪽 방향키
#define K_RIGHT 77 // 오른쪽 방향키
#define K_NONE 0 // 아무런 키 입력이 없을 때 
#define K_QUIT 'q' // 게임종료

/*전역변수*/
char board[Field_Height][FIELD_WIDTH]; // 맵을 만들기 위한 2차원 배열

int col_player = 1, dir_player = 1; // col_player: 플레이어 위치 좌표값, dir_player: 플레이어의 이동 방향을 저장하는 변수
int col_enemy = 2, dir_enemy = 1; // col_enemy: 적 위치 좌표값, dir_enemy: 적의 이동 방향을 저장하는 변수

int save_score; // 점수를 저장하는 변수

/*함수 선언*/
void gotoxy(int, int); // 가로, 세로 좌표를 반대로 해주는 함수

void map(void); // 맵을 표현한 함수

void intro(void); // 게임 시작화면
void outro(void); // 게임 종료화면

int get_key(void); // 키보드 입력 값(혹은 입력하지 않았을 때의 값)을 반환

void turn_player_arrow(int); // 방향키 입력시 플레이어의 좌표 이동
void turn_enemy_arrow(int); // 방향키 입력시 적의 좌표 이동

void turn_player_none(void); // 키 입력이 없을 시 플레이어의 좌표 이동
void turn_enemy_none(void); // 키 입력이 없을 시 적의 좌표 이동

void move_player(void); // 플레이어의 위치를 표현한 함수
void move_enemy(void); // 적의 위치를 표현한 함수

void score(void); // 점수를 출력하는 함수

/*함수 정의*/
int main(void)
{
	int tick = 0;
	int key = 0;
	int arrow = 0;

	intro();
	map();
	while (true)
	{
		Sleep(10); // 프로그램의 진행을 0.01초 마다 일시정지(키 입력을 0.01초 마다 처리하기 위함)
		tick++; // 1 tick = 10 ms
		key = get_key(); // 키보드 입력 값(혹은 입력하지 않았을 때 반환 값)을 key에 저장
		if (key == K_QUIT) // q를 누르면 게임이 종료된다
		{
			break;
		}
		else if (key == K_ARROW) // 방향키 입력시(키 입력이 없을 때와 달리, 방향키 입력시에는 캐릭터의 위치가 즉시 바뀐다)
		{
			arrow = _getch(); // 방향키 두번째 반환 값을 arrow에 저장(방향키는 아스키 코드값을 두번 반환)
			if (arrow == K_LEFT || arrow == K_RIGHT)
			{
				turn_player_arrow(arrow);
				move_player();
			}
			else if (arrow == K_UP || arrow == K_DOWN)
			{
				turn_enemy_arrow(arrow);
				move_enemy();
			}
		}
		else if (key == K_NONE) // 키 입력 없을 시
		{
			if (tick % 20 == 0) // 20 tick(0.2초) 마다 플레이어의 위치를 움직임
			{
				turn_player_none();
				move_player();
			}
			if (tick % 50 == 0) // 50 tick(0.5초) 마다 적의 위치를 움직임
			{
				turn_enemy_none();
				move_enemy();
			}
		}
		score();
	}
	outro();
	return 0;
}
void map(void)
{
	int i, j;
	for (i = 0; i < Field_Height; i++) // 먼저 18행 22열의 #을 출력한다.
	{
		for (j = 0; j < FIELD_WIDTH; j++)
		{
			board[i][j] = MAP_WALL;
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
	for (i = 0; i < DOWN_CORNER; i++) // 그 후, (1,1)부터 좌표를 이동하며 16행 20열의 공백을 출력하여 맵을 완성한다.
	{
		for (j = 0; j < RIGHT_CORNER; j++)
		{
			board[i][j] = MAP_EMPTY;
			gotoxy(i + 1, j + 1);
			printf("%c", board[i][j]);
		}
	}
	gotoxy(0, 25); // 게임 방법 안내
	printf("<게임방법>");
	gotoxy(2, 25);
	printf("@: 플레이어, &: 적");
	gotoxy(4, 25);
	printf("→ ← 방향키로 플레이어 이동, ↑ ↓ 방향키로 적 이동");
	gotoxy(5, 25);
	printf("플레이어와 적이 부딪힐 때 마다 점수가 올라갑니다.");
	gotoxy(7, 25);
	printf("q: 게임종료");
	gotoxy(9, 25);
	printf("점수: ");
}
void intro(void)
{
	printf("Welcome!\n");
	printf("아무키나 누르세요");
	while (true)
	{
		if (_kbhit())
		{
			system("cls"); // 콘솔창의 모든 내용을 지워준다.
			break;
		}
	}
}
void outro(void)
{
	system("cls");
	printf("Bye!\n");
	printf("게임을 나가려면 아무키나 누르세요...\n");

	while (true)
	{
		if (_kbhit())
		{
			break;
		}
	}
}
int get_key(void)
{
	while (true)
	{
		if (_kbhit()) // 키보드 입력 값이 있을경우, 입력 값을 반환
		{
			return _getch();
		}
		else // 키보드 입력값이 없을경우, 0을 반환
		{
			return K_NONE;
		}
	}
}
void turn_player_arrow(int arrow)
{
	gotoxy(1, col_player);
	printf("%c", MAP_EMPTY); // 플레이어의 이전 위치를 지운다
	switch (arrow)
	{
	case K_LEFT: // 왼쪽 방향키 입력시
		col_player += DIR_LEFT;
		dir_player = DIR_LEFT;
		if (col_player < LEFT_CORNER) // 플레이어의 위치가 왼쪽 벽을 뚫지 못하게 하기 위함
		{
			col_player = LEFT_CORNER;
		}
		break;

	case K_RIGHT: // 오른쪽 방향키 입력시
		col_player += DIR_RIGHT;
		dir_player = DIR_RIGHT;
		if (col_player > RIGHT_CORNER) // 플레이어의 위치가 오른쪽 벽을 뚫지 못하게 하기 위함
		{
			col_player = RIGHT_CORNER;
		}
		break;
	}
}
void turn_enemy_arrow(int arrow)
{
	gotoxy(col_enemy, 1);
	printf("%c", MAP_EMPTY); // 적의 이전 위치를 지운다
	switch (arrow)
	{
	case K_UP: // 위쪽 방향키 입력시
		col_enemy += DIR_UP;
		dir_enemy = DIR_UP;
		if (col_enemy < UP_CORNER) // 적의 위치가 위쪽 벽을 뚫지 못하게 하기 위함
		{
			col_enemy = UP_CORNER;
		}
		break;

	case K_DOWN: // 아래쪽 방향키 입력시
		col_enemy += DIR_DOWN;
		dir_enemy = DIR_DOWN;
		if (col_enemy > DOWN_CORNER) // 적의 위치가 아래쪽 벽을 뚫지 못하게 하기 위함
		{
			col_enemy = DOWN_CORNER;
		}
		break;
	}
}
void turn_player_none(void)
{
	gotoxy(1, col_player);
	printf("%c", MAP_EMPTY);
	if (col_player == LEFT_CORNER) // 플레이어가 왼쪽 벽에 부딪힐 경우, 오른쪽으로 이동
	{
		col_player += DIR_RIGHT;
		dir_player = DIR_RIGHT;
	}
	else if (col_player == RIGHT_CORNER) // 플레이어가 오른쪽 벽에 부딪힐 경우, 왼쪽으로 이동
	{
		col_player += DIR_LEFT;
		dir_player = DIR_LEFT;
	}
	else
	{
		col_player += dir_player; // 플레이어가 벽에 부딪히지 않을 경우, 이동하던 방향 그대로 이동
	}
}
void turn_enemy_none(void)
{
	gotoxy(col_enemy, 1);
	printf("%c", MAP_EMPTY);
	if (col_enemy == UP_CORNER) // 적이 위쪽 벽에 부닺힐 경우, 아래로 이동
	{
		col_enemy += DIR_DOWN;
		dir_enemy = DIR_DOWN;
	}
	else if (col_enemy == DOWN_CORNER) // 적이 아래쪽 벽에 부닺힐 경우, 위로 이동
	{
		col_enemy += DIR_UP;
		dir_enemy = DIR_UP;
	}
	else
	{
		col_enemy += dir_enemy; // 적이 벽에 부딪히지 않을 경우, 이동하던 방향 그대로 이동
	}
}
void move_player(void)
{
	gotoxy(1, col_player); // 플레이어의 위치 좌표
	printf("%c", '@');
}
void move_enemy(void)
{
	gotoxy(col_enemy, 1); // 적의 위치 좌표
	printf("%c", '&');
}
void score(void)
{
	if (col_player == 1 && col_enemy == 1) // @와 &가 부딪힐 때마다 점수 증가
	{
		save_score++;
		gotoxy(9, 30); // 점수 출력
		printf("%d", save_score);
	}
}
void gotoxy(int row, int column)
{
	COORD pos = { column, row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}