#ifndef BATTLE_HEADER
#define BATTLE_HEADER
#pragma comment(lib, "winmm.lib")

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <Mmsystem.h>
#include <Digitalv.h>

typedef enum CURSOR_TYPE { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
typedef enum { false, true } bool;

#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15
#define UIJUMP 8
#define WHITESPACE "  "

#define Z 122
#define Q 113
#define ENTER 13
#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.
#define P1DOWN ' ' // Player1의 경우 스페이스바를 누르면 바로 내리기 가능
#define P2DOWN 'j' // Player2의 경우 j키를 누르면 바로 내리기 가능
#define LEFT 75 // Player1는 방향키로 조작
#define RIGHT 77
#define UP 72
#define DOWN 80
#define UP2		'w'//player2 는 AWSD 로 방향키 대신
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'
#define ESC 27
#define BX1 5 // 외부 벽의 좌상단 좌표 BX, BY
#define BY 1
#define BX2 65 // Plyaer2의 외부 벽의 좌상단 좌표 BX2, BY값은 동일

#define BW 10 // 게임판의 폭과 높이 BW, BH
#define BH 20 // 하지만 이 폭과 높이는 개념상 정의된 수치이지, 실제 바이트와는 다르다.
// BW의 경우, 출력되는 블럭들이 2바이트씩 출력되므로 실제 BW의 값은 2*10 이 될 것이다.
// BH의 경우, 출력되는 블럭들에 상관 없이 값은 1바이트로 실제 BH의 값은 그대로 20이 될 것이다.
/* BW + 2 = 게임판의 폭에 양쪽 벽의 폭을 더한 값 */

void clrscr();
void gotoxy(int x, int y);
int wherex();
int wherey();
void setcursortype(CURSOR_TYPE c);
void textcolor(int fg_color, int bg_color);

#define delay(n) Sleep(n)                              // n/1000초만큼 시간 지연
#define randomize() srand((unsigned)time(NULL))         // 난수 발생기 초기화
#define random(n) (rand() % (n))                        //0~n-1까지의 난수 발생

// 화면을 모두 지운다.
void clrscr()
{
	system("cls");
}

// 커서를 x,y좌표로 이동시킨다.
void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

// 커서의 x 좌표를 조사한다.
int wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.X;
}

// 커서의 y좌표를 조사한다.
int wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.Y;
}

// 커서를 숨기거나 다시 표시한다.
void setcursortype(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

#endif