#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>

#define TICK_MS			10
#define PLAYER_TICK		20
#define ENEMY_TICK		50

#define FIELD_WIDTH		22
#define FIELD_HEIGHT	18
#define LEFT_CORNER		1
#define RIGHT_CORNER	(FIELD_WIDTH - 2)
#define TOP_CORNER		2
#define BOTTOM_CORNER	(FIELD_HEIGHT - 2)

#define MAP_WALL	'#'
#define MAP_PLAYER	'@'
#define MAP_ENEMY	'$'
#define MAP_EMPTY	' '

#define DIR_LEFT	-1
#define DIR_RIGHT	1
#define DIR_UP		-1
#define DIR_DOWN	1

#define K_ARROW 224
#define K_UP 72
#define K_DOWN 80
#define K_LEFT 75
#define K_RIGHT 77
#define K_NONE	0	// no key input or invalid key input
#define K_QUIT	'q'

void gotoxy(int, int);
void initialize(void);
int get_key(void);
void turn(int);
void player_move(void);
void enemy_move(void);
void display(void);
void intro();
void outro();

int tick;
char backbuf[FIELD_HEIGHT][FIELD_WIDTH], frontbuf[FIELD_HEIGHT][FIELD_WIDTH];
int pcol, pdir, erow, edir;

// (행, 열)로 입력 받아서 (열, 행)위치로 커서를 옮겨주는 함수
void gotoxy(int row, int column) {
	COORD pos = { column, row };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		pos
	);
}

void intro(void) {
	printf("Welcome!");
	getchar();
	system("cls");
}

void outro(void) {
	system("cls");
	printf("Bye!\n");
}

void display(void) {
	for (int i = 0; i < FIELD_HEIGHT; i++) {
		for (int j = 0; j < FIELD_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				frontbuf[i][j] = backbuf[i][j];
				gotoxy(i, j);
				printf("%c", frontbuf[i][j]);
			}
		}
	}
	gotoxy(0, 0);
}

void initialize(void) {
	tick = 0;
	pcol = LEFT_CORNER;
	pdir = DIR_RIGHT;
	erow = TOP_CORNER;
	edir = DIR_UP;

	for (int i = 0; i < FIELD_HEIGHT; i++) {
		for (int j = 0; j < FIELD_WIDTH; j++) {
			if (i == 1 && j == pcol) {
				backbuf[i][j] = MAP_PLAYER;
			}
			else if (j == 1 && i == erow) {
				backbuf[i][j] = MAP_ENEMY;
			}
			else if (i == 0 || i == FIELD_HEIGHT - 1 || j == 0 || j == FIELD_WIDTH - 1) {
				backbuf[i][j] = MAP_WALL;
			}
			else {
				backbuf[i][j] = MAP_EMPTY;
			}
		}
	}
}

int get_key(void) {
	if (_kbhit()) {
		int key = _getch();
		if (key == K_QUIT) {
			return K_QUIT;
		}

		if (key == K_ARROW) {
			key = _getch();
			switch (key) {
			case K_LEFT:
			case K_RIGHT:
			case K_UP:
			case K_DOWN:
				return key;
			default:
				return K_NONE;
			}
		}
	}

	return K_NONE;
}

void turn(int key) {
	if (pcol == LEFT_CORNER) {
		pdir = DIR_RIGHT;
	}
	else if (pcol == RIGHT_CORNER) {
		pdir = DIR_LEFT;
	}
	else if (key == K_LEFT) {
		pdir = DIR_LEFT;
	}
	else if (key == K_RIGHT) {
		pdir = DIR_RIGHT;
	}
	if (erow == TOP_CORNER) {
		edir = DIR_DOWN;
	}
	else if (erow == BOTTOM_CORNER) {
		edir = DIR_UP;
	}
	else if (key == K_UP) {
		edir = DIR_UP;
	}
	else if (key == K_DOWN) {
		edir = DIR_DOWN;
	}
	else {
		// noop
	}
}

void player_move(void) {
	backbuf[1][pcol] = MAP_EMPTY;
	pcol += pdir;
	backbuf[1][pcol] = MAP_PLAYER;
}

void enemy_move(void) {
	backbuf[erow][1] = MAP_EMPTY;
	erow += edir;
	backbuf[erow][1] = MAP_ENEMY;
}

int main(void) {
	intro();
	initialize();
	while (true) {
		display();
		Sleep(TICK_MS);
		tick++;

		int key = K_NONE;
		key = get_key();
		if (key == K_QUIT) {
			break;
		}
		turn(key);

		if (tick % PLAYER_TICK == 0) {
			player_move();
		}
		if (tick % ENEMY_TICK == 0) {
			enemy_move();
		}
	}
	outro();
}
