#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <Windows.h>

/*��ũ�� ����*/
#define FIELD_WIDTH 22 // 2���� ���� ��
#define Field_Height 18 // 2���� ���� ��
#define LEFT_CORNER 1 // ���� ���� ��
#define RIGHT_CORNER (FIELD_WIDTH - 2) // ���� ������ ��
#define UP_CORNER 1 // ���� ���� ��
#define DOWN_CORNER (Field_Height - 2) // ���� �Ʒ��� ��

#define MAP_WALL '#' // ��
#define MAP_PLAYER '@' // �÷��̾� ĳ����
#define MAP_ENEMY '&' // �� ĳ����
#define MAP_EMPTY ' ' // ���� ����

#define DIR_LEFT -1 // ĳ���Ͱ� �������� �̵��� �� ��ǥ����� �̿�
#define DIR_RIGHT 1 // ĳ���Ͱ� ���������� �̵��� �� ��ǥ����� �̿�
#define DIR_UP -1 // ĳ���Ͱ� ���� �̵��� �� ��ǥ����� �̿�
#define DIR_DOWN 1 // ĳ���Ͱ� �Ʒ��� �̵��� �� ��ǥ����� �̿�

#define K_ARROW 224 // ����Ű �ƽ�Ű�ڵ�
#define K_UP 72 // �� ����Ű
#define K_DOWN 80 // �Ʒ� ����Ű
#define K_LEFT 75 // ���� ����Ű
#define K_RIGHT 77 // ������ ����Ű
#define K_NONE 0 // �ƹ��� Ű �Է��� ���� �� 
#define K_QUIT 'q' // ��������

/*��������*/
char board[Field_Height][FIELD_WIDTH]; // ���� ����� ���� 2���� �迭

int col_player = 1, dir_player = 1; // col_player: �÷��̾� ��ġ ��ǥ��, dir_player: �÷��̾��� �̵� ������ �����ϴ� ����
int col_enemy = 2, dir_enemy = 1; // col_enemy: �� ��ġ ��ǥ��, dir_enemy: ���� �̵� ������ �����ϴ� ����

int save_score; // ������ �����ϴ� ����

/*�Լ� ����*/
void gotoxy(int, int); // ����, ���� ��ǥ�� �ݴ�� ���ִ� �Լ�

void map(void); // ���� ǥ���� �Լ�

void intro(void); // ���� ����ȭ��
void outro(void); // ���� ����ȭ��

int get_key(void); // Ű���� �Է� ��(Ȥ�� �Է����� �ʾ��� ���� ��)�� ��ȯ

void turn_player_arrow(int); // ����Ű �Է½� �÷��̾��� ��ǥ �̵�
void turn_enemy_arrow(int); // ����Ű �Է½� ���� ��ǥ �̵�

void turn_player_none(void); // Ű �Է��� ���� �� �÷��̾��� ��ǥ �̵�
void turn_enemy_none(void); // Ű �Է��� ���� �� ���� ��ǥ �̵�

void move_player(void); // �÷��̾��� ��ġ�� ǥ���� �Լ�
void move_enemy(void); // ���� ��ġ�� ǥ���� �Լ�

void score(void); // ������ ����ϴ� �Լ�

/*�Լ� ����*/
int main(void)
{
	int tick = 0;
	int key = 0;
	int arrow = 0;

	intro();
	map();
	while (true)
	{
		Sleep(10); // ���α׷��� ������ 0.01�� ���� �Ͻ�����(Ű �Է��� 0.01�� ���� ó���ϱ� ����)
		tick++; // 1 tick = 10 ms
		key = get_key(); // Ű���� �Է� ��(Ȥ�� �Է����� �ʾ��� �� ��ȯ ��)�� key�� ����
		if (key == K_QUIT) // q�� ������ ������ ����ȴ�
		{
			break;
		}
		else if (key == K_ARROW) // ����Ű �Է½�(Ű �Է��� ���� ���� �޸�, ����Ű �Է½ÿ��� ĳ������ ��ġ�� ��� �ٲ��)
		{
			arrow = _getch(); // ����Ű �ι�° ��ȯ ���� arrow�� ����(����Ű�� �ƽ�Ű �ڵ尪�� �ι� ��ȯ)
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
		else if (key == K_NONE) // Ű �Է� ���� ��
		{
			if (tick % 20 == 0) // 20 tick(0.2��) ���� �÷��̾��� ��ġ�� ������
			{
				turn_player_none();
				move_player();
			}
			if (tick % 50 == 0) // 50 tick(0.5��) ���� ���� ��ġ�� ������
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
	for (i = 0; i < Field_Height; i++) // ���� 18�� 22���� #�� ����Ѵ�.
	{
		for (j = 0; j < FIELD_WIDTH; j++)
		{
			board[i][j] = MAP_WALL;
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
	for (i = 0; i < DOWN_CORNER; i++) // �� ��, (1,1)���� ��ǥ�� �̵��ϸ� 16�� 20���� ������ ����Ͽ� ���� �ϼ��Ѵ�.
	{
		for (j = 0; j < RIGHT_CORNER; j++)
		{
			board[i][j] = MAP_EMPTY;
			gotoxy(i + 1, j + 1);
			printf("%c", board[i][j]);
		}
	}
	gotoxy(0, 25); // ���� ��� �ȳ�
	printf("<���ӹ��>");
	gotoxy(2, 25);
	printf("@: �÷��̾�, &: ��");
	gotoxy(4, 25);
	printf("�� �� ����Ű�� �÷��̾� �̵�, �� �� ����Ű�� �� �̵�");
	gotoxy(5, 25);
	printf("�÷��̾�� ���� �ε��� �� ���� ������ �ö󰩴ϴ�.");
	gotoxy(7, 25);
	printf("q: ��������");
	gotoxy(9, 25);
	printf("����: ");
}
void intro(void)
{
	printf("Welcome!\n");
	printf("�ƹ�Ű�� ��������");
	while (true)
	{
		if (_kbhit())
		{
			system("cls"); // �ܼ�â�� ��� ������ �����ش�.
			break;
		}
	}
}
void outro(void)
{
	system("cls");
	printf("Bye!\n");
	printf("������ �������� �ƹ�Ű�� ��������...\n");

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
		if (_kbhit()) // Ű���� �Է� ���� �������, �Է� ���� ��ȯ
		{
			return _getch();
		}
		else // Ű���� �Է°��� �������, 0�� ��ȯ
		{
			return K_NONE;
		}
	}
}
void turn_player_arrow(int arrow)
{
	gotoxy(1, col_player);
	printf("%c", MAP_EMPTY); // �÷��̾��� ���� ��ġ�� �����
	switch (arrow)
	{
	case K_LEFT: // ���� ����Ű �Է½�
		col_player += DIR_LEFT;
		dir_player = DIR_LEFT;
		if (col_player < LEFT_CORNER) // �÷��̾��� ��ġ�� ���� ���� ���� ���ϰ� �ϱ� ����
		{
			col_player = LEFT_CORNER;
		}
		break;

	case K_RIGHT: // ������ ����Ű �Է½�
		col_player += DIR_RIGHT;
		dir_player = DIR_RIGHT;
		if (col_player > RIGHT_CORNER) // �÷��̾��� ��ġ�� ������ ���� ���� ���ϰ� �ϱ� ����
		{
			col_player = RIGHT_CORNER;
		}
		break;
	}
}
void turn_enemy_arrow(int arrow)
{
	gotoxy(col_enemy, 1);
	printf("%c", MAP_EMPTY); // ���� ���� ��ġ�� �����
	switch (arrow)
	{
	case K_UP: // ���� ����Ű �Է½�
		col_enemy += DIR_UP;
		dir_enemy = DIR_UP;
		if (col_enemy < UP_CORNER) // ���� ��ġ�� ���� ���� ���� ���ϰ� �ϱ� ����
		{
			col_enemy = UP_CORNER;
		}
		break;

	case K_DOWN: // �Ʒ��� ����Ű �Է½�
		col_enemy += DIR_DOWN;
		dir_enemy = DIR_DOWN;
		if (col_enemy > DOWN_CORNER) // ���� ��ġ�� �Ʒ��� ���� ���� ���ϰ� �ϱ� ����
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
	if (col_player == LEFT_CORNER) // �÷��̾ ���� ���� �ε��� ���, ���������� �̵�
	{
		col_player += DIR_RIGHT;
		dir_player = DIR_RIGHT;
	}
	else if (col_player == RIGHT_CORNER) // �÷��̾ ������ ���� �ε��� ���, �������� �̵�
	{
		col_player += DIR_LEFT;
		dir_player = DIR_LEFT;
	}
	else
	{
		col_player += dir_player; // �÷��̾ ���� �ε����� ���� ���, �̵��ϴ� ���� �״�� �̵�
	}
}
void turn_enemy_none(void)
{
	gotoxy(col_enemy, 1);
	printf("%c", MAP_EMPTY);
	if (col_enemy == UP_CORNER) // ���� ���� ���� �δ��� ���, �Ʒ��� �̵�
	{
		col_enemy += DIR_DOWN;
		dir_enemy = DIR_DOWN;
	}
	else if (col_enemy == DOWN_CORNER) // ���� �Ʒ��� ���� �δ��� ���, ���� �̵�
	{
		col_enemy += DIR_UP;
		dir_enemy = DIR_UP;
	}
	else
	{
		col_enemy += dir_enemy; // ���� ���� �ε����� ���� ���, �̵��ϴ� ���� �״�� �̵�
	}
}
void move_player(void)
{
	gotoxy(1, col_player); // �÷��̾��� ��ġ ��ǥ
	printf("%c", '@');
}
void move_enemy(void)
{
	gotoxy(col_enemy, 1); // ���� ��ġ ��ǥ
	printf("%c", '&');
}
void score(void)
{
	if (col_player == 1 && col_enemy == 1) // @�� &�� �ε��� ������ ���� ����
	{
		save_score++;
		gotoxy(9, 30); // ���� ���
		printf("%d", save_score);
	}
}
void gotoxy(int row, int column)
{
	COORD pos = { column, row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}