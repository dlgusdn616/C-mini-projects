#include "Battle.h"

// Player1 전용 함수
BOOL Player1();
void DrawScreen1(); // 화면 전체를 그린다. 게임판과 게임 이름, 벽까지 한꺼번에 그린다.
void DrawBoard1(); // 게임판만 그린다. 즉 외부벽과 문자열들은 빼고 쌓여 있는 벽돌만 그린다.
void DrawNext1(); // 다음에 나올 블럭을 그린다.
void PrintBrick1(BOOL Show); // 벽돌을 출력하거나 삭제하는데 이동중인 벽돌을 대상으로 하므로
							 // 전역변수 brick1, rot1, nx1, ny1 값을 참조한다.
int GetAround1(int x, int y, int b, int r); // 벽돌 주변에 무엇이 있는지 검사하여 벽돌의 이동 및 회전 가능성을 조사한다.
											// 이동중인 벽돌의 주변을 조사하는 것이 아니므로 인수로 전달된 위치의 벽돌 모양을 참조한다.
BOOL MoveDown1(); // 벽돌을 한 칸 아래로 이동시킨다. 만약 바닥에 닿았다면 TestFull 함수를 호출한 후 TRUE를 리턴한다.
void TestFull1(); // 수평으로 다 채워진 줄을 찾아 삭제한다.
void PrintInfo1(); // 점수와 블럭의 갯수를 출력한다.
void FinishGame1(); // 더 이상 벽돌이 생성될 수 없을 때, 호출되는 함수다. 맨 윗줄부터 벽돌 정보가 기록된 곳을 차례대로 회색으로 물들인다.

// Player2 전용 함수
BOOL Player2();
void DrawScreen2();
void DrawBoard2();
void DrawNext2();
void PrintBrick2(BOOL Show);
int GetAround2(int x, int y, int b, int r);
BOOL MoveDown2();
void TestFull2();
void PrintInfo2();
void FinishGame2();

// 공용 함수
void StartUI(); // 테트리스 제목과 조작키를 설명하는 시작화면을 출력한다.
void Init(); // 전역변수 및 전역배열을 초기화하는 함수
void InputKey(unsigned char); // 사용자의 키 입력을 처리하는데, 1P와 2P가 동시에 처리되기 위해서는 하나의 함수로 설정했다.

// 좌표를 정의하는 Point 구조체 타입을 정의
typedef struct Point {
	int x, y;
} Point;

// 4개의 좌표가 모여 하나의 모양을 구성하고, 4개의 모양 (회전시 최대 4개) 이 모여야 하나의 벽돌이 정의되며
// 이런 벽돌이 7개가 있다. 그래서 다음과 같이 3차원 구조체 배열로 전체 벽돌의 모양을 정의한다.
// Shape[벽돌번호][회전번호][일련번호] 마지막 일련번호의 경우 새끼블럭의 좌표값
// 첫 번째 좌표 (0,0)이 각 모양의 기준점이며, 회전할 때의 중심점이 된다. 나머지 세 좌표는 기준점으로부터 상대적 거리 좌표이다.
Point Shape[][4][4] = {
	{ { { 0,0 },{ 1,0 },{ 2,0 },{ -1,0 } },{ { 0,0 },{ 0,1 },{ 0,-1 },{ 0,-2 } },{ { 0,0 },{ 1,0 },{ 2,0 },{ -1,0 } },{ { 0,0 },{ 0,1 },{ 0,-1 },{ 0,-2 } } }, // ㅡ자 블럭 정의 (변형된 모형의 좌표 2개 중복 -> ㅡ, ㅣ, ㅡ, ㅣ 순으로 중복)
	{ { { 0,0 },{ 1,0 },{ 0,1 },{ 1,1 } },{ { 0,0 },{ 1,0 },{ 0,1 },{ 1,1 } },{ { 0,0 },{ 1,0 },{ 0,1 },{ 1,1 } },{ { 0,0 },{ 1,0 },{ 0,1 },{ 1,1 } } }, // ㅁ자 블럭 정의 (변형된 모형의 좌표 4개 모두동일)
	{ { { 0,0 },{ -1,0 },{ 0,-1 },{ 1,-1 } },{ { 0,0 },{ 0,1 },{ -1,0 },{ -1,-1 } },{ { 0,0 },{ -1,0 },{ 0,-1 },{ 1,-1 } },{ { 0,0 },{ 0,1 },{ -1,0 },{ -1,-1 } } }, // ㄹ(좌향)자 블럭 정의 (변형된 모형의 좌표 2개 중복 -> ㄹ(좌향), 반시계방향 회전 순으로 중복) 
	{ { { 0,0 },{ -1,-1 },{ 0,-1 },{ 1,0 } },{ { 0,0 },{ -1,0 },{ -1,1 },{ 0,-1 } },{ { 0,0 },{ -1,-1 },{ 0,-1 },{ 1,0 } },{ { 0,0 },{ -1,0 },{ -1,1 },{ 0,-1 } } }, // ㄹ(우향)자 블럭 정의 (변형된 모형의 좌표 2개 중복 -> ㄹ(우향), 시계방향 회전 순으로 중복)
	{ { { 0,0 },{ -1,0 },{ 1,0 },{ -1,-1 } },{ { 0,0 },{ 0,-1 },{ 0,1 },{ -1,1 } },{ { 0,0 },{ -1,0 },{ 1,0 },{ 1,1 } },{ 0,0,0,-1,0,1,1,-1 } }, // ㄴ(우향)자 블럭 정의 (반시계방향 회전 순으로 4개 모양 정의)
	{ { { 0,0 },{ 1,0 },{ -1,0 },{ 1,-1 } },{ { 0,0 },{ 0,1 } ,{ 0,-1 },{ -1,-1 } },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } }, // ㄴ(좌향)자 블럭 정의 (반시계방향 회전 순으로 4개 모양 정의)
	{ { { 0,0 },{ -1,0 },{ 1,0 },{ 0,1 } },{ { 0,0 },{ 0,-1 },{ 0,1 },{ 1,0 } },{ { 0,0 },{ -1,0 },{ 1,0 },{ 0,-1 } },{ { 0,0 },{ -1,0 },{ 0,-1 },{ 0,1 } } }, // ㅜ자 블럭 정의 (반시계방향 회전 순으로 4개 모양 정의)
};

// puts(arTile[ttype][enum])과 같이 활용될 것이다. 셋 중 하나로 그려진다.
enum { EMPTY, BRICK, WALL }; // 게임판의 상태를 표현한다. 게임판은 이 셋 중 하나의 상태를 가진다.
							 // define으로 매크로 상수를 각각 정의 할 수도 있지만, 중복되지 않는 일련의 값이므로 열거 멤버로 선언하는 것이 더 바람직

							 // 타일의 모양을 변경할 수 있도록 이차 포인터 배열로 만들었다.
							 // 그리고 현재 사용중인 타일 모양을 저장하기 위한 ttype 전역변수를 추가한다.
int ttype = 0;
char *arTile[][3] = {
	{ "  ","▣","▩" }, // 새로 생성되었을 때의 블럭 모양
	{ "  ","■","▩" }, // 바닥에 닿았을 때의 블럭 모양
}; // 크기 3의 문자열 배열, 각 타일은 2개의 char로 구성된다.
   // "  " 공백의 경우 2byte로 구성되어 있고 기존에 출력된 블럭을 지울 때 주로 사용된다.
   // 위에 선언되어 있는 열거형 멤버변수를 각 타일에 그대로 매핑시켜 타일배열의 인덱스로 사용할 것이다.

   /* Player1 전용 전역 변수 및 배열 */
int board1[BW + 2][BH + 2]; // 전체 게임판의 상태다. 벽돌이 얼마만큼 쌓여 있는지를 기억한다.
int color1[BW + 2][BH + 2]; // 컬러 기록판이다. board1 배열에 기록된 벽돌의 종류에 맞춰 컬러가 정해져 기록된다.
// 이미 바닥에 착륙한 벽돌의 정보만 기록한다. 이동중인 벽돌들은 아직 자리가 정해지지 않았으므로 기록되지 않는다.
// 실제 화면에 표시되는 좌표와는 차이가 있다. 처음에 이렇게 절대적인 기준 좌표를 설정한 후에 출력이 필요할 때, 별도의 연산과정을 거쳐야 한다.

int nx1, ny1; // Player1의 이동중인 벽돌의 현재 좌표이다. 배열상의 좌표 값이다.
// 배열상의 좌표를 화면상의 좌표로 바꿀 때는 BX + nx*2, BY + ny 식을 사용하면 된다.
// 위와 같은 식을 사용하면, 기존의 10x20 크기의 게임판을 토대로 실제 출력좌표로 구현할 수 있다.
// 출력을 위해서 별도로 좌표를 생각할 필요가 없다는 점이 구현에 있어 최고 장점이다.

// 블럭 전역변수
int brick1, rot1; // 이동중인 벽돌의 번호와 회전 모양을 기억한다. brick, rot, nx, ny 전역변수를 참조하면
				  // 어떤 벽돌이 어떤 모양으로 어느 위치에 있는지를 알게 된다.
int nbrick1; // 다음에 나올 벽돌의 모양을 저장하기 위한 전역변수
int score1; // 점수를 계산하는 전역변수, 공용전역변수에 속하는 arScoreInt 배열에 삭제하는 줄 수에 따라 각각 다른 점수를 구현했다.
int bricknum1; // 총 벽돌의 개수를 저장하는 전역번수
int flag1; // 새로운 벽돌을 출력할지 말지를 결정하는 flag, Player1 함수에 속해 있다.
int nFrame1, nStay1; // 게임의 속도를 결정하는 전역변수, nFrame1값을 기준으로 nStay1값이 역카운팅 (0.05초 간격)을 통해 속도를 결정한다.

 /* Player2 전용 전역 변수 및 배열 */
int board2[BW + 2][BH + 2];
int color2[BW + 2][BH + 2];
int nx2, ny2;
int brick2, rot2;
int nbrick2;
int score2;
int bricknum2;
int flag2;
int nFrame2, nStay2;

// 공용 전역변수 (game, finish, virtualNum, offset 의 경우 공용 전역변수는 아니지만, 여기 배치하는 편이 전체적인 게임 흐름이해에 도움이 된다.)
int game1, game2; // 게임이 끝났는지, 안 끝났는지 유무를 판별하는 전역변수
int finish1, finish2; // 게임플레이가 끝났을 때 0.05초 간격으로 지워짐을 연출, 0.05초는 개발자가 절대적으로 설정한 최소단위의 시간 값이다. 이 값을 기준으로 한다.
int brickColor[7] = { MAGENTA2, RED2, YELLOW2, GREEN2, BLUE2, CYAN2, GRAY2 }; // 각 블럭마다 고유의 색깔이 존재한다.
int arScoreInc[] = { 0, 1, 3, 8, 10 }; // 스코어판, 사라지는 줄 수에 따라 점수가 다르게 반영된다.
int virtualNum1, virtualNum2; // 기존의 난이도 설정 알고리즘에는 치명적 오류가 존재했다. 0.05초 마다 player 함수가 1p, 2p 각각 실행 되면서 nStay 값을
// nFrame 값에 맞춰 역카운팅 해주는 작업을 진행하면서 속도조절이 진행되는데, 기존의 bricknum % n (총 등장한 블럭 수가 n개에 비례할 때) 조건문에 맞추어 nFrame 값을
// 감소시켜주는 작업을 시행했을 경우, 0.05초 단위로 작업이 실행되기에 bricknum 값이 0.05초 단위로 빠르게 바뀌지 않는 이상 조건이 충족했을 때 기하급수적으로
// nFrame 값이 감소하는 문제점이 발생했고, 그로 인해 속도가 갑자기 빨라지는 오류가 발생했다. 그 오류를 virtualNum 변수와 offset 변수로 적절히 조정함으로써
// 조건이 충족했을 시점에 딱 한 번만 nFrame을 감소시켜주게끔 알고리즘을 수정했다.
int offset1, offset2;

void main()
{
	unsigned char ch; // 특수키 입력까지 처리해야할 char 변수
	randomize(); // 난수 초기화 (srand(time(NULL))과 동일
	StartUI();

	while (1) { // 다시시작하겠습니까? 등의 메시지를 전부 삭제함과 동시에 게임보드를 초기화 해야하는 등의 작업이 필요하기에
				// 루프는 여기서부터 다시 시작한다.
		clrscr();
		Init();
		PlaySound(TEXT("Tetris.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		while (2) { // 게임 전체를 감싸는 루프이다.
			if (finish1 == 1 || finish2 == 1) { // finish 전역변수가 둘 중 하나만이라도 1이면 게임 속행
				// 단, 게임이 완전히 끝났음을 의미하는 finish라는 비트가 켜져 있어야 한다.
				if (kbhit() == 1) {
					// 실행할 때마다 game의 값을 체크해준다. 체크해주는 이유는 혹여나 둘 중 하나가 플레이가 끝났다면, 쓸데 없이 실행되는 일이 없도록
					// 프로그램의 효율성을 위해 계속 체크해준다.
					ch = getch();
					if (game1 == 1) { // 게임이 끝나지 않았을 때
						Player1();
					}
					else
						FinishGame1(); // 게임이 끝났을 때는 위에서부터 0.05초 간격으로 한 칸씩 GRAY2 컬러화 시키는 작업을 진행한다.

					if (game2 == 1) {
						Player2();
					}
					else
						FinishGame2();
					InputKey(ch);
				}
				else { // 키를 입력받지 않았을 때
					if (game1 == 1)
						Player1();
					else
						FinishGame1();

					if (game2 == 1)
						Player2();
					else
						FinishGame2();
				}
			}
			else {
				break; // finish1과 finish2의 값이 0일 때 루프를 빠져 나온다. 즉, 하나의 온전한 플레이가 종료됐다는 뜻이다.
				// finish 전역변수들이 0이 되는 건, GRAY2 컬러화가 완전히 끝났다는 것을 의미한다.
			}

			delay(1000 / 20); // 1ms = 1/1000 초, 0.05초
		}
		delay(2000); // 2초
		clrscr();
		gotoxy(35, 10); puts("G A M E  O V E R");
		gotoxy(38, 13);
		if (score1 > score2)
			printf("Player1 WIN\n");
		else if (score1 == score2)
			printf("Draw Game\n");
		else
			printf("Playejr2 WIN\n");
		gotoxy(29, 16); puts("다시 시작하려면 Y를 누르세요");
		if (tolower(getch()) != 'y') break;
	}
	PlaySound(NULL, 0, 0);
	setcursortype(NORMALCURSOR);
}

void StartUI()
{
	char buf[128]; // Key Instruction을 그릴 때 길이측정에 사용될 캐릭터 배열
	char startKey;
	setcursortype(NOCURSOR); // 커서를 보이지 않게 설정한다.

							 // BATTLE 출력
							 // B 출력
	textcolor(MAGENTA2, BLACK);
	gotoxy(5, 2);	printf("■■");
	gotoxy(5, 3);	printf("■  ■");
	gotoxy(5, 4);	printf("■■");
	gotoxy(5, 5);	printf("■  ■");
	gotoxy(5, 6);	printf("■■");
	// A 출력
	gotoxy(5 + UIJUMP, 2);	printf("  ■  ");
	gotoxy(5 + UIJUMP, 3);	printf("■  ■");
	gotoxy(5 + UIJUMP, 4);	printf("■■■");
	gotoxy(5 + UIJUMP, 5);	printf("■  ■");
	gotoxy(5 + UIJUMP, 6);	printf("■  ■");
	// T 출력
	gotoxy(5 + UIJUMP * 2, 2);	printf("■■■");
	gotoxy(5 + UIJUMP * 2, 3);	printf("  ■  ");
	gotoxy(5 + UIJUMP * 2, 4);	printf("  ■  ");
	gotoxy(5 + UIJUMP * 2, 5);	printf("  ■  ");
	gotoxy(5 + UIJUMP * 2, 6);	printf("  ■  ");
	// T 출력 
	gotoxy(5 + UIJUMP * 3, 2);	printf("■■■");
	gotoxy(5 + UIJUMP * 3, 3);	printf("  ■  ");
	gotoxy(5 + UIJUMP * 3, 4);	printf("  ■  ");
	gotoxy(5 + UIJUMP * 3, 5);	printf("  ■  ");
	gotoxy(5 + UIJUMP * 3, 6);	printf("  ■  ");
	// L 출력
	gotoxy(5 + UIJUMP * 4, 2);	printf("■");
	gotoxy(5 + UIJUMP * 4, 3);	printf("■");
	gotoxy(5 + UIJUMP * 4, 4);	printf("■");
	gotoxy(5 + UIJUMP * 4, 5);	printf("■");
	gotoxy(5 + UIJUMP * 4, 6);	printf("■■■");
	// E 출력
	gotoxy(5 + UIJUMP * 5, 2);	printf("■■■");
	gotoxy(5 + UIJUMP * 5, 3);	printf("■");
	gotoxy(5 + UIJUMP * 5, 4);	printf("■■■");
	gotoxy(5 + UIJUMP * 5, 5);	printf("■");
	gotoxy(5 + UIJUMP * 5, 6);	printf("■■■");
	// version 출력
	textcolor(WHITE, BLACK);
	gotoxy(5 + UIJUMP * 7, 6);	printf("ver. 1.0");

	// TETRIS 출력
	textcolor(GREEN2, BLACK);
	// T 출력
	gotoxy(5 + UIJUMP * 2, 8);	printf("▣▣▣");
	gotoxy(5 + UIJUMP * 2, 9);	printf("  ▣  ");
	gotoxy(5 + UIJUMP * 2, 10);	printf("  ▣  ");
	gotoxy(5 + UIJUMP * 2, 11);	printf("  ▣  ");
	gotoxy(5 + UIJUMP * 2, 12);	printf("  ▣  ");
	// E 출력
	gotoxy(5 + UIJUMP * 3, 8);	printf("▣▣▣");
	gotoxy(5 + UIJUMP * 3, 9);	printf("▣");
	gotoxy(5 + UIJUMP * 3, 10);	printf("▣▣▣");
	gotoxy(5 + UIJUMP * 3, 11);	printf("▣");
	gotoxy(5 + UIJUMP * 3, 12);	printf("▣▣▣");
	// T 출력
	gotoxy(5 + UIJUMP * 4, 8);	printf("▣▣▣");
	gotoxy(5 + UIJUMP * 4, 9);	printf("  ▣  ");
	gotoxy(5 + UIJUMP * 4, 10);	printf("  ▣  ");
	gotoxy(5 + UIJUMP * 4, 11);	printf("  ▣  ");
	gotoxy(5 + UIJUMP * 4, 12);	printf("  ▣  ");
	// R 출력
	gotoxy(5 + UIJUMP * 5, 8);	printf("▣▣  ");
	gotoxy(5 + UIJUMP * 5, 9);	printf("▣  ▣");
	gotoxy(5 + UIJUMP * 5, 10);	printf("▣▣");
	gotoxy(5 + UIJUMP * 5, 11);	printf("▣  ▣");
	gotoxy(5 + UIJUMP * 5, 12);	printf("▣  ▣");
	// I 출력
	gotoxy(5 + UIJUMP * 6, 8);	printf("▣");
	gotoxy(5 + UIJUMP * 6, 8);	printf("▣");
	gotoxy(5 + UIJUMP * 6, 9);	printf("▣");
	gotoxy(5 + UIJUMP * 6, 10);	printf("▣");
	gotoxy(5 + UIJUMP * 6, 11);	printf("▣");
	gotoxy(5 + UIJUMP * 6, 12);	printf("▣");
	// S 출력
	gotoxy(5 + UIJUMP * 6 + 4, 8);	printf("▣▣▣");
	gotoxy(5 + UIJUMP * 6 + 4, 9);	printf("▣");
	gotoxy(5 + UIJUMP * 6 + 4, 10);	printf("▣▣▣");
	gotoxy(5 + UIJUMP * 6 + 4, 11);	printf("    ▣");
	gotoxy(5 + UIJUMP * 6 + 4, 12);	printf("▣▣▣");

	// Key Instructions 출력
	textcolor(GRAY1, BLACK);
	gotoxy(4, 17);
	strcpy(buf, "┌──<   Key Instructions   >──┐");
	printf("%s", buf);
	gotoxy(4, 18);	printf("│");
	gotoxy(6, 18);	printf("PLAYER 1");
	gotoxy(30, 18);	printf("PLAYER 2");
	gotoxy(38, 18);	printf("│");

	gotoxy(4, 19);	printf("│");
	gotoxy(9, 19);	printf("↑");
	gotoxy(21, 19); printf("UP");
	gotoxy(33, 19); printf("W");
	gotoxy(38, 19);	printf("│");

	gotoxy(4, 20);	printf("│");
	gotoxy(9, 20);	printf("←");
	gotoxy(20, 20); printf("LEFT");
	gotoxy(33, 20); printf("A");
	gotoxy(38, 20);	printf("│");

	gotoxy(4, 21);	printf("│");
	gotoxy(9, 21);	printf("→");
	gotoxy(19, 21); printf("RIGHT");
	gotoxy(33, 21); printf("D");
	gotoxy(38, 21);	printf("│");

	gotoxy(4, 22);	printf("│");
	gotoxy(9, 22);	printf("↓");
	gotoxy(20, 22); printf("DOWN");
	gotoxy(33, 22); printf("S");
	gotoxy(38, 22);	printf("│");

	gotoxy(4, 23);	printf("│");
	gotoxy(8, 23);	printf("sBar");
	gotoxy(18, 23); printf("STRAIGHT");
	gotoxy(33, 23); printf("J");
	gotoxy(38, 23);	printf("│");

	gotoxy(4, 24);
	printf("└────────────────┘");

	gotoxy(5 + UIJUMP * 3 + 5, 15);
	textcolor(WHITE, BLACK);

	printf("< Press Enter Key to Start >");
	while (1) {
		startKey = getch();
		if (startKey == ENTER)
			return;
		else
			continue;
	}
}

void Init()
{
	int x, y;
	for (x = 0; x < BW + 2; x++) {
		for (y = 0; y < BH + 2; y++) { // x와 y가 각각 0일 때, 게임판의 틀(BW, BH)을 벗어났을 때 WALL을 설정
									   // 즉, 게임보드를 벗어났을 때의 상황에 WALL 값을 설정해준다.
			board1[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
			board2[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
			color1[x][y] = WHITE;
			color2[x][y] = WHITE;
		}
	}
	// DrawScreen() 안에 점수 출력 란이 있으므로 초기화 작업을 진행해준다.
	score1 = score2 = 0; // 점수 초기화
	bricknum1 = bricknum2 = 0; // 초기 블럭이 생성될 모양이다. 기본 도형모양인 0으로 설정해준다.
	DrawScreen1();		DrawScreen2(); // 게임 화면 전체를 출력한다. 벽으로 설정되어 있던 부분은 벽으로, 공백 부분은 공백으로 출력된다.
	nStay1 = nFrame1 = 20; // 프레임 값을 결정해준다. 20번을 역카운팅 해야 MoveDown()이 호출된다. 각 카운팅당 실행시간은 별도로 설정해준다.
	nStay2 = nFrame2 = 20;
	nbrick1 = random(sizeof(Shape) / sizeof(Shape[0])); // 초기 블럭 모양을 랜덤하게 결정
	nbrick2 = random(sizeof(Shape) / sizeof(Shape[0]));
	flag1 = flag2 = 0;
	game1 = game2 = 1; // 게임이 시작됨을 의미한다. 이 둘 값이 0이 되면 게임이 끝남을 의미한다.
	finish1 = finish2 = 1;
	offset1 = offset2 = 1; // 속도 조절에 쓰일 offset값을 1로 초기화
}

void InputKey(unsigned char ch)
{
	int trot;
	if (ch == SPECIAL1 || ch == SPECIAL2) {
		ch = getch();
		switch (ch) {
		case LEFT:
			if (game1 == 1) {
				if (GetAround1(nx1 - 1, ny1, brick1, rot1) == EMPTY) { // 왼쪽으로 한 칸 갈 경우를 미리 예비해 해당 좌표에 무엇이 있는지를 조사한다.
					PrintBrick1(FALSE); // 비어져 있을 경우 기존 좌표에 해당하는 좌표 값의 블럭을 지운다.
					nx1--; // 현재 x좌표의 값을 왼쪽으로 한 칸 이동시킨다.
					PrintBrick1(TRUE); // 새로운 좌표에 블럭을 출력한다.
				}
			}
			break;
		case RIGHT:
			if (game1 == 1) {
				if (GetAround1(nx1 + 1, ny1, brick1, rot1) == EMPTY) {
					PrintBrick1(FALSE);
					nx1++;
					PrintBrick1(TRUE);
				}
			}
			break;
		case UP:
			if (game1 == 1) {
				trot = (rot1 == 3 ? 0 : rot1 + 1); // 순환하는 값을 가져야하므로 (0~3) 3이 되었을 경우는 다시 0으로, 그 외에는 하나의 값을 증가시킨다.
				if (GetAround1(nx1, ny1, brick1, trot) == EMPTY) { // 변경된 회전값을 미리 적용시켜 출력이 가능한지 확인한다.
					PrintBrick1(FALSE); // 출력이 가능하다면, 현재 좌표에 출력되어 있는 블럭을 공백문자로 채워 삭제하는 작업을 진행한다.
					rot1 = trot; // 출력이 가능하기 때문에, 회전 상태를 나타내는 전역변수 rot의 값을 변경된 회전상태를 나타내는 trot의 값으로부터 복사한다.
					PrintBrick1(TRUE); // 변경된 rot의 값을 적용하여 그 위치에 출력한다.
				}
			}
			break;
		case DOWN:
			if (game1 == 1) {
				if (MoveDown1()) { // 한 칸 아래로 내리는 함수를 호출한 후 바로 TRUE를 리턴한다. (키 값이 눌렸기 때문에)
					return TRUE;
				}
				break;
			}
		}
	}
	else {
		switch (tolower(ch)) { // 어차피 소문자가 들어 있었다면 그대로 소문자이고 대문자면 소문자로 변형하기에 훨씬 세련된 조건문이라 할 수 있다.
		case ' ':
			if (game1 == 1) {
				while (MoveDown1() == FALSE) { ; } // 바닥에 닿으면 MoveDown() 은 TRUE를 리턴한다. 따라서, 바닥에 닿지 않은 경우 무한루프로 MoveDown()을 호출한다.
				return TRUE; // 바닥에 닿았으면 TRUE를 리턴한다. (키 값이 눌렸다는 것을 의미하는 TRUE)
			}
			break;
		case ESC:
			clrscr();
			exit(0);
		case 'p': // 잠시 정지를 할 때는 화면을 깨끗하게 지우고 getch로 대기한다.
			clrscr();
			gotoxy(15, 10);
			puts("Tetris 잠시 중지. 다시 시작하려면 아무키나 누르세요.");
			getch();
			clrscr(); // 키를 눌러 다시 돌아올 때는 텍스트를 지우고
			ttype = 1; // 바닥에 닿았을 때는 블럭 모양이 변한다.
			DrawScreen1(); // 전체화면을 다시 그린다.
			DrawBoard1();
			DrawNext1();
			PrintBrick1(TRUE); // 이동중인 벽돌도 같이 그려야 한다. (현재좌표 기준)
			PrintInfo1();
			DrawScreen2();
			DrawBoard2();
			DrawNext2();
			PrintBrick2(TRUE);
			PrintInfo2();
			ttype = 0;
			break;
		case LEFT2:
			if (game2 == 1) {
				if (GetAround2(nx2 - 1, ny2, brick2, rot2) == EMPTY) { // 왼쪽으로 한 칸 갈 경우를 미리 예비해 해당 좌표에 무엇이 있는지를 조사한다.
					PrintBrick2(FALSE); // 비어져 있을 경우 기존 좌표에 해당하는 좌표 값의 블럭을 지운다.
					nx2--; // 현재 x좌표의 값을 왼쪽으로 한 칸 이동시킨다.
					PrintBrick2(TRUE); // 새로운 좌표에 블럭을 출력한다.
				}
			}
			break;
		case RIGHT2:
			if (game2 == 1) {
				if (GetAround2(nx2 + 1, ny2, brick2, rot2) == EMPTY) {
					PrintBrick2(FALSE);
					nx2++;
					PrintBrick2(TRUE);
				}
			}
			break;
		case UP2:
			if (game2 == 1) {
				trot = (rot2 == 3 ? 0 : rot2 + 1); // 순환하는 값을 가져야하므로 (0~3) 3이 되었을 경우는 다시 0으로, 그 외에는 하나의 값을 증가시킨다.
				if (GetAround2(nx2, ny2, brick2, trot) == EMPTY) { // 변경된 회전값을 미리 적용시켜 출력이 가능한지 확인한다.
					PrintBrick2(FALSE); // 출력이 가능하다면, 현재 좌표에 출력되어 있는 블럭을 공백문자로 채워 삭제하는 작업을 진행한다.
					rot2 = trot; // 출력이 가능하기 때문에, 회전 상태를 나타내는 전역변수 rot의 값을 변경된 회전상태를 나타내는 trot의 값으로부터 복사한다.
					PrintBrick2(TRUE); // 변경된 rot의 값을 적용하여 그 위치에 출력한다.
				}
			}
			break;
		case DOWN2:
			if (game2 == 1) {
				if (MoveDown2()) { // 한 칸 아래로 내리는 함수를 호출한 후 바로 TRUE를 리턴한다. (키 값이 눌렸기 때문에)
					return TRUE;
				}
			}
			break;
		case 'j':
			if (game2 == 1) {
				while (MoveDown2() == FALSE) { ; } // 바닥에 닿으면 MoveDown() 은 TRUE를 리턴한다. 따라서, 바닥에 닿지 않은 경우 무한루프로 MoveDown()을 호출한다.
				return TRUE; // 바닥에 닿았으면 TRUE를 리턴한다. (키 값이 눌렸다는 것을 의미하는 TRUE)
			}
		}
	}
}

BOOL Player1()
{
	int i;
	if (flag1 == 0) {
		bricknum1++; // 블럭이 등장할 때마다 총 블럭의 수를 1씩 늘려준다.
		PrintInfo1();
		brick1 = nbrick1; // 초기에 결정된 모양을 복사, 다음에 생성될 모양이 그대로 복사 (두 번째 시행부터)
		nbrick1 = random(sizeof(Shape) / sizeof(Shape[0])); // 다음에 생성될 모양을 랜덤하게 결정

		DrawNext1(); // 다음에 생성될 모양을 화면에 출력
					 // 새로운 벽돌을 위쪽 중앙 좌표인 (BW/2, 3) 위치에 생성한다.

		nx1 = BW / 2; // Player1 필드의 중앙부터 생성된다.
		ny1 = 3; // 시작 y좌표는 3으로 고정
		rot1 = 0; // 첫 등장 블럭은 회전이 되지 않은 상태이므로 rot의 값은 0이다.
		PrintBrick1(TRUE); // TRUE 값을 넣으면 블럭이 출력된다.
		flag1 = 1; // 블럭이 새로 생겼으므로 flag1 값은 다시 1로 설정해준다.
	}
	if (GetAround1(BW / 2, 3, brick1, rot1) != EMPTY) {
		for (i = 0; i < 4; i++) {
			board1[nx1 + Shape[brick1][rot1][i].x][ny1 + Shape[brick1][rot1][i].y] = BRICK;
		}
		game1 = 0;
		return FALSE;
	}
	if (--nStay1 == 0) { // 설정한 프레임에 따라 nStay1 값이 0.05초 간격으로 카운팅되고, 20번 실행됐을 때 (즉, 1초가 됐을 때) MoveDown1() 함수를 호출한다.
		nStay1 = nFrame1;// 위와 같은 원리로 정확히 1초에 한 칸씩 내려가게끔 설정이 되어 있다.
		if (MoveDown1()) {
			return TRUE; // 시간이 지나 벽돌이 바닥에 닿으면 MoveDown 함수가 TRUE를 리턴한다.
		}
	}
	// 굳이 nStay를 역카운트 해주면서 1초마다 동작 시키는 이유는, 만약 시간 간격 자체를 1초로 설정해 버리면,
	// 중간에 받는 키 입력 또한 1초마다 입력받을 수가 있기 때문에 게임의 반응성과 자유도에 심각한 영향을 끼치게 된다.
	// 따라서 키 입력을 매 프레임마다 받을수 있도록 하기 위해 일부러 시간을 분할했다.
	if (offset1) {
		if (bricknum1 < 100 && bricknum1 % 10 == 0 && nFrame1 > 5) {
			// 벽돌이 100개 이하일 때는 10개가 떨어질 때마다 프레임 수를 1씩 감소시켜 벽돌이 떨어지는 속도를 증가시킨다.
			// nFrame은 벽돌이 한 번 내려올 프레임 수이므로 이 값이 감소하면 속도는 증가한다. (0.05초 x nFrame 값이 한 칸 아래로 내려올 때의 속도다.)
			// 100개 이하일 때의 최대 속도는 0.25초에 한 번 내려오는 것으로 제한하였다.
			// 이 식에 사용된 상수(bricknum1) 10와 nFrame1 5는 난이도를 조절하는 수단이다.
			virtualNum1 = bricknum1 + 1;
			nFrame1--;
			nStay1 = nFrame1; // 역카운팅에 필요한 nStay1 변수를 갱신해준다. 이 과정에서 완벽한 시간계산이 되지 않는 한계가 존재한다.
			// 기존에 nStay카운팅 상수가 남아 있을 때 갱신될 경우, 기존의 카운팅 정보가 날라가기 때문이다.
			offset1 = 0;
		}
		else if (bricknum1 >= 100 && bricknum1 % 10 == 0 && nFrame1 > 1) { // 벽돌이 100개 이상일 때는 nFrame이 최소 2까지 갈 수 있게끔 난이도를 설정했다. (2*0.05 = 0.1초)
			virtualNum1 = bricknum1 + 1; // 가상블럭넘버 virtualNum1이 존재하는 이유는 조건이 만족했을 때 속도를 조절하는 구문이 0.05초 단위로 실행되는 걸 막기 위함이다.
			nFrame1--; // 즉 조건이 만족했으면 한 번만 실행이 되어야 하는 구조를 만들어야하기 때문에, virtualNum1을 brickNum 보다 1증가 시킨 값으로 주고
			nStay1 = nFrame1; // offset1을 0으로 맞춰준다. offset버튼을 껐으면 다음 실행이 될 때는 밑의 else문으로 가게 된다.
			offset1 = 0;
		}
	}
	else { // 위에서 속도조절하는 문장이 한 번 시행된 후 오는 조건문이다.
		if (bricknum1 == virtualNum1) { // virtualNum1이 기존의 bricknum1 보다 1 높게 설정이 되어 있기 때문에, 새로운 블럭이 등장하면
			offset1 = 1; // 그 때 다시 offset버튼이 켜져 위의 조건문을 다시금 재점검할 수 있게 되는 원리다.
		}
	}
	return TRUE;
}

BOOL Player2()
{
	int i;
	if (flag2 == 0) {
		bricknum2++;
		PrintInfo2();
		brick2 = nbrick2;
		nbrick2 = random(sizeof(Shape) / sizeof(Shape[0]));
		DrawNext2();
		nx2 = BW / 2;
		ny2 = 3;
		rot2 = 0;
		PrintBrick2(TRUE);
		flag2 = 1;
	}
	if (GetAround2(BW / 2, 3, brick2, rot2) != EMPTY) {
		for (i = 0; i < 4; i++) {
			board2[nx2 + Shape[brick2][rot2][i].x][ny2 + Shape[brick2][rot2][i].y] = BRICK;
		}
		game2 = 0;
		return FALSE;
	}
	if (--nStay2 == 0) {
		nStay2 = nFrame2;
		if (MoveDown2()) {
			return TRUE;
		}
	}
	if (offset2) {
		if (bricknum2 < 100 && bricknum2 % 10 == 0 && nFrame2 > 5) {
			
			virtualNum2 = bricknum2 + 1;
			nFrame2--;
			nStay2 = nFrame2;
			offset2 = 0;
		}
		else if (bricknum2 >= 100 && bricknum2 % 10 == 0 && nFrame2 > 1) {
			virtualNum2 = bricknum2 + 1;
			nFrame2--;
			nStay2 = nFrame2;
			offset2 = 0;
		}
	}
	else { 
		if (bricknum2 == virtualNum2) { 
			offset2 = 1;
		}
	}
	return TRUE;
}

void DrawScreen1()
{
	int x, y;

	for (x = 0; x < BW + 2; x++) {
		for (y = 0; y < BH + 2; y++) { // 좌상단 x, y좌표에 해당하는 BX, BY로부터
									   // x좌표의 경우 2씩, y좌표의 경우 1씩 증가시키면서 보드를 출력한다.
			gotoxy(BX1 + x * 2, BY + y);
			// Init()에서 벽에 해당하는 부분에는 각각 WALL 값을 설정했으므로 WALL이 출력될 것이다.
			// 벽의 조건: x == 0 || y == 0 || x == BW + 1 || y == BH + 1
			puts(arTile[ttype][board1[x][y]]);
		}
	}
}

void DrawScreen2()
{
	int x, y;
	for (x = 0; x < BW + 2; x++) {
		for (y = 0; y < BH + 2; y++) {
			gotoxy(BX2 + x * 2, BY + y);
			puts(arTile[ttype][board2[x][y]]);
		}
	}
}

void DrawBoard1()
{
	int x, y;

	for (x = 1; x < BW + 1; x++) {
		for (y = 1; y < BH + 1; y++) { // 좌상단부터 우측하단까지 쭈욱 순회하면서
			gotoxy(BX1 + x * 2, BY + y); // 가상좌표를 실제좌표로 변환한 곳으로 커서를 이동시키고
			textcolor(brickColor[color1[x][y]], BLACK);
			puts(arTile[ttype][board1[x][y]]); // 보드배열에 기록된 정보를 토대로 출력한다.
		}
	}
	textcolor(WHITE, BLACK);
}

void DrawBoard2()
{
	int x, y;
	for (x = 1; x < BW + 1; x++) {
		for (y = 1; y < BH + 1; y++) {
			gotoxy(BX2 + x * 2, BY + y);
			textcolor(brickColor[color2[x][y]], BLACK);
			puts(arTile[ttype][board2[x][y]]);
		}
	}
	textcolor(WHITE, BLACK);
}

void DrawNext1()
{
	int x, y, i;
	gotoxy(32, 1);
	printf("<PLAYER1>");
	gotoxy(32, 2);
	printf("Next:");
	textcolor(brickColor[nbrick1], BLACK); // 다음에 나올 블럭에 맞는 인덱스 nbrick1값을 인덱스로 brickColor에 저장되어 있는 색을 불러온다.
	for (x = 30; x <= 38; x += 2) { // 다음에 나올 블럭은 꾸준히 바뀌기 때문에, 해당 부분 화면만 지워주는 작업이 필요하다.
		for (y = 3; y <= 6; y++) {
			gotoxy(x, y);
			puts(arTile[ttype][EMPTY]);
		}
	}
	for (i = 0; i < 4; i++) { // 블럭을 위의 for문으로 지워줬으므로 다음에 나올 블럭을 새롭게 출력한다.
		gotoxy(34 + (Shape[nbrick1][0][i].x) * 2, 5 + Shape[nbrick1][0][i].y);
		puts(arTile[ttype][BRICK]);
	}
	textcolor(WHITE, BLACK); // 원하는 블럭을 출력했으니 다시 색깔을 초기화해준다.
}

void DrawNext2()
{
	int x, y, i;
	gotoxy(50, 1);
	printf("<PLAYER2>");
	gotoxy(50, 2);
	printf("Next:");
	textcolor(brickColor[nbrick2], BLACK);
	for (x = 50; x <= 58; x += 2) {
		for (y = 3; y <= 6; y++) {
			gotoxy(x, y);
			puts(arTile[ttype][EMPTY]);
		}
	}
	for (i = 0; i < 4; i++) {
		gotoxy(52 + (Shape[nbrick2][0][i].x) * 2, 5 + Shape[nbrick2][0][i].y);
		puts(arTile[ttype][BRICK]);
	}
	textcolor(WHITE, BLACK);
}

// 전역변수 brick, rot가 지정하는 벽돌 모양을 현재 위치 nx, ny에 출력하거나 지운다.
void PrintBrick1(BOOL Show) // TRUE 값이 들어오면 BRICK을 출력하고, FALSE 값이 들어오면 EMPTY를 출력한다. (즉, 공백을 출력한다.)
{
	int i;
	textcolor(brickColor[brick1], BLACK);
	for (i = 0; i < 4; i++) { // 새끼 블럭의 좌표값들을 계산하여 하나씩 출력하여 하나의 완전한 블럭을 출력한다.
		gotoxy(BX1 + (Shape[brick1][rot1][i].x + nx1) * 2, BY + Shape[brick1][rot1][i].y + ny1);
		// 기준 좌표와 기준 길이의 개념을 가지고, 이 수식으로 실제 좌표로 구현한다.
		// 기준 시작 좌표는 (0,0), 기준 길이 (BW, BH) 는 마치 C++의 추상클래스와 비슷한 개념으로
		// 실제 구현에서는 위와 같이 수식을 써주어 화면에 출력해야 한다.
		puts(arTile[ttype][Show ? BRICK : EMPTY]); // 블럭 혹은 공백을 출력한다. (2바이트)
	}
	textcolor(WHITE, BLACK);
}

void PrintBrick2(BOOL Show)
{
	int i;
	textcolor(brickColor[brick2], BLACK);
	for (i = 0; i < 4; i++) {
		gotoxy(BX2 + (Shape[brick2][rot2][i].x + nx2) * 2, BY + Shape[brick2][rot2][i].y + ny2);
		puts(arTile[ttype][Show ? BRICK : EMPTY]);
	}
	textcolor(WHITE, BLACK);
}

// 이동중인 벽돌의 주변 정보를 조사해야하므로, 전역변수를 참조하지 않고, 조사할 벽돌의 정보를 인수로 전달받는다.
int GetAround1(int x, int y, int b, int r)
{
	int i, k = EMPTY;

	for (i = 0; i < 4; i++) { // 최초 EMPTY(0)로 초기화된 k는 벽돌이 있는 좌표를 순회하면서 최대값을 조사하게 된다.
		// EMPTY < BRICK < WALL 순으로 평가하고, 만약 이 위치에 아무것도 없다면 그대로 EMPTY가 리턴될 것이고, 벽돌과 충돌했으면 BRICK
		// 벽과 충돌 되었다면 WALL 이 리턴될 것이다. 이 함수가 조사하는 주변 값은 벽돌의 이동가능 여부, 게임의 계속여부 등을 판단하는 중요한 기준이 된다.
		k = max(k, board1[x + Shape[b][r][i].x][y + Shape[b][r][i].y]); // max는 stdlib에 매크로로 구현되어 있다.
	}
	return k;
}

int GetAround2(int x, int y, int b, int r)
{
	int i, k = EMPTY;

	for (i = 0; i < 4; i++) {
		k = max(k, board2[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown1()
{
	if (GetAround1(nx1, ny1 + 1, brick1, rot1) != EMPTY) { // 회전값과 x값은 그대로 유지한 채, y값만 하나 증가시켜 주변에 블럭이나 벽이 있는지 확인한다.
		ttype = 1; // 바닥에 닿았을 때는 블럭 모양이 변한다.
		TestFull1(); // 만약 벽이나 블럭이 있다면, TestFull() 을 호출하여 채워진 줄 제거를 한다. (있을 경우)
					 // 이 점이 바로 왼쪽 혹은 오른쪽 이동과 다르다.
		flag1 = 0; // 블럭을 새로 생성하기 위한 flag변수 설정, 이로써 Player1 함수에서 새로운 블럭 생성이 가능해진다.
		DrawBoard1(); // board 배열의 갱신된 값을 바탕으로 다시 그린다.
		// PlaySound(TEXT("BlockBreak.wav"), NULL, SND_FILENAME | SND_ASYNC);
		ttype = 0; // 블럭 모양 원상복귀
		return TRUE; // 바닥에 닿은 경우, TRUE를 리턴한다. 왜냐하면, 다음 블럭을 출력해야하기 때문이다.
	}
	PrintBrick1(FALSE); // 아래로 이동할 공간에 벽이나 블럭이 없으므로, 기존의 위치의 벽돌을 지워준다.
	ny1++; // y값을 하나 증가시킨 후, 그 좌표에 블럭을 출력하여 한 칸 아래 이동을 구현한다.
	PrintBrick1(TRUE);
	return FALSE; // 바닥에 닿은 것이 아니므로 FALSE를 리턴한다.
}

BOOL MoveDown2()
{
	if (GetAround2(nx2, ny2 + 1, brick2, rot2) != EMPTY) {
		ttype = 1; // 바닥에 닿았을 때는 블럭 모양이 변한다.
		TestFull2();
		flag2 = 0;
		DrawBoard2(); // board 배열의 갱신된 값을 바탕으로 다시 그린다. 변경된 블럭 모양으로 출력하기 위함 (원래는 없어도 상관 없음)
		// PlaySound(TEXT("BlockBreak.wav"), NULL, SND_FILENAME | SND_ASYNC);
		ttype = 0; // 블럭 모양 원상복귀
		return TRUE;
	}
	PrintBrick2(FALSE);
	ny2++;
	PrintBrick2(TRUE);
	return FALSE;
}

void TestFull1() // 채워진 줄 제거를 하는 함수
{	// 이 함수는 MoveDown() 함수에서 호출되고, MoveDown()에서 한 칸 아래 공백이 아니라는 것이 판정되면 호출된다.
	// 한 칸 아래가 WALL or BRICK 이므로 어느 쪽이든 일단 블럭은 출력되어야 하고 삭제 되는 과정은 그 뒤에 일어난다.
	int i, j, x, y, ty;
	int rNum[3];
	int count = 0;
	for (i = 0; i < 4; i++) { // 현재 좌표에 해당하는 블럭 값을 게임판 배열에 기록한다. (없어질지라도, 일단 해당 정보가 기록되어 있어야 검사조건에 감지될 수 있기 때문이다.) 
		board1[nx1 + Shape[brick1][rot1][i].x][ny1 + Shape[brick1][rot1][i].y] = BRICK;
		color1[nx1 + Shape[brick1][rot1][i].x][ny1 + Shape[brick1][rot1][i].y] = brick1;
	}
	// 수직으로 제일 윗줄부터 검사한다.
	for (y = 1; y < BH + 1; y++) { // y값은 고정되고 x값이 가변이므로, 좌표평면상 한 라인을 검사하는 for문이다. 경계선 전까지 모든 라인을 검사한다.
		for (x = 1; x < BW + 1; x++) {
			if (board1[x][y] != BRICK) break; // 하나라도 비어 있는 칸이 있다면, 한 라인으로 볼 수 없으므로 for문을 나간다.
		}
		// 하나의 라인이 채워져 있다는 것이 판정됐을 시
		if (x == BW + 1) { // 해당 줄에 해당하는 y부터 1까지 순차적으로 감소하는 이유는 위의 공백라인의 정보까지 복사를 해줘야 기존의 복사 후 남는 더미블럭을 제거할 수 있기 때문이다.
			count++; // 없애는 줄 수마다 count를 하나씩 증가시킨다.
			for (ty = y; ty > 1; ty--) { // 현재 y값부터 1까지 순차적으로 감소하면서
				for (x = 1; x < BW + 1; x++) { // 해당 y값의 모든 x를 순회하면서
					board1[x][ty] = board1[x][ty - 1]; // 기록되어 있는 블럭을 한 칸 아래로 옮긴다.
					color1[x][ty] = color1[x][ty - 1];
				}
			}
			DrawBoard1(); // board 배열의 갱신된 값을 바탕으로 다시 그린다.
		}
		score1 += arScoreInc[count]; // 룩업테이블에서 없앤 줄 수를 첨자로 하여 점수를 더한다.
									 // PrintInfo(); // 점수기능이 들어갔으므로 게임 중에 사용자가 이 정보를 볼 수 있도록 해야 한다.
	}
	if (count >= 3 && game2 == 1) { // 지운 블럭이 3개 이상일 때, 상대방에게 공격을 가한다.
		for (i = 0; i < count; i++) { // 한 칸 위로 옮기는 작업을 count 수만큼 해준다.
			for (y = 1; y < BH + 1; y++) { // 해당 줄에 해당하는 y부터 1까지 순차적으로 증가하는 이유는 아래의 공백라인의 정보까지 복사를 해줘야하기 때문이다.
				for (x = 1; x < BW + 1; x++) { // 해당 y값의 모든 x를 순회하면서
					board2[x][y - 1] = board2[x][y]; // 기록되어 있는 블럭을 한 칸 위로 옮긴다.
					color2[x][y - 1] = color2[x][y];
					board2[x][y] = EMPTY;
				}
			}
		}
		for (i = 0; i < 3; i++) { // 랜덤으로 빈칸이 뚫릴 x좌표값 3개를 배열에 저장해준다.
			rNum[i] = random(BW) + 1;
			for (j = 0; j < i; j++) {
				if (rNum[j] == rNum[i]) {
					i--;
					break;
				}
			}
		}
		for (y = BH; y >= BH - count + 1; y--) {
			for (x = 1; x < BW + 1; x++) {
				for (i = 0; i < 3; i++) {
					if (rNum[i] == x) {  // 빈칸을 뚫는데 사용된 배열 내용에 flag를 표시해준다. 표시를 해주지 않으면 똑같은 곳에 구멍이 뚫리게된다.
						rNum[i] = 0;
						board2[x][y] = EMPTY;
						break;
					}
					else {
						board2[x][y] = BRICK;
						color2[x][y] = brickColor[6]; // 연산이 어디가 잘못된 건진 모르겠으나, BLUE2로 출력된다.
						break;
					}
				}
			}
			for (i = 0; i < 3; i++) { // 빈칸이 랜덤으로 3군데 뚫린 바를 상대편으로 날린다. (공격)
				rNum[i] = random(BW) + 1;
				for (j = 0; j < i; j++) { // flag를 표시했던 배열의 값을 포함하여 전체를 리셋해준다.
					if (rNum[j] == rNum[i]) { 
						i--;
						break;
					}
				}
			}
		}
		DrawBoard2();
	}
}

void TestFull2()
{
	int i, j, x, y, ty;
	int rNum[3];
	int count = 0;
	for (i = 0; i < 4; i++) {
		board2[nx2 + Shape[brick2][rot2][i].x][ny2 + Shape[brick2][rot2][i].y] = BRICK;
		color2[nx2 + Shape[brick2][rot2][i].x][ny2 + Shape[brick2][rot2][i].y] = brick2;
	}
	// 수직으로 제일 윗줄부터 검사한다.
	for (y = 1; y < BH + 1; y++) { // y값은 고정되고 x값이 가변이므로, 좌표평면상 한 라인을 검사하는 for문이다. 경계선 전까지 모든 라인을 검사한다.
		for (x = 1; x < BW + 1; x++) {
			if (board2[x][y] != BRICK) break; // 하나라도 비어 있는 칸이 있다면, 한 라인으로 볼 수 없으므로 for문을 나간다.
		}
		// 하나의 라인이 채워져 있다는 것이 판정됐을 시
		if (x == BW + 1) { // 해당 줄에 해당하는 y부터 1까지 순차적으로 감소하는 이유는 위의 공백라인의 정보까지 복사를 해줘야 기존의 복사 후 남는 더미블럭을 제거할 수 있기 때문이다.
			count++; // 없애는 줄 수마다 count를 하나씩 증가시킨다.
			for (ty = y; ty > 1; ty--) { // 현재 y값부터 1까지 순차적으로 감소하면서
				for (x = 1; x < BW + 1; x++) { // 해당 y값의 모든 x를 순회하면서
					board2[x][ty] = board2[x][ty - 1]; // 기록되어 있는 블럭을 한 칸 아래로 옮긴다.
					color2[x][ty] = color2[x][ty - 1];
				}
			}
			DrawBoard2(); // board 배열의 갱신된 값을 바탕으로 다시 그린다.
		}
		score2 += arScoreInc[count]; // 룩업테이블에서 없앤 줄 수를 첨자로 하여 점수를 더한다.
									 // PrintInfo(); // 점수기능이 들어갔으므로 게임 중에 사용자가 이 정보를 볼 수 있도록 해야 한다.
	}

	if (count >= 3 && game1 == 1) {
		for (i = 0; i < count; i++) { // 한 칸 위로 옮기는 작업을 count 수만큼 해준다.
			for (y = 1; y < BH + 1; y++) { // 해당 줄에 해당하는 y부터 1까지 순차적으로 증가하는 이유는 아래의 공백라인의 정보까지 복사를 해줘야하기 때문이다.
				for (x = 1; x < BW + 1; x++) { // 해당 y값의 모든 x를 순회하면서
					board1[x][y - 1] = board1[x][y]; // 기록되어 있는 블럭을 한 칸 위로 옮긴다.
					color1[x][y - 1] = color1[x][y];
					board1[x][y] = EMPTY;
				}
			}
		}
		for (i = 0; i < 3; i++) { // 랜덤으로 빈칸이 뚫릴 x좌표값 3개를 배열에 저장해준다.
			rNum[i] = random(BW) + 1;
			for (j = 0; j < i; j++) {
				if (rNum[j] == rNum[i]) {
					i--;
					break;
				}
			}
		}
		for (y = BH; y >= BH - count + 1; y--) {
			for (x = 1; x < BW + 1; x++) {
				for (i = 0; i < 3; i++) {
					if (rNum[i] == x) {
						rNum[i] = 0; // 빈칸을 뚫는데 사용된 배열 내용에 flag를 표시해준다. 표시를 해주지 않으면 똑같은 곳에 구멍이 뚫리게된다.
						board1[x][y] = EMPTY;
						break;
					}
					else {
						board1[x][y] = BRICK;
						color1[x][y] = brickColor[6];
						break;
					}
				}
			}
			for (i = 0; i < 3; i++) { // flag를 표시했던 배열의 값을 포함하여 전체를 리셋해준다.
				rNum[i] = random(BW) + 1;
				for (j = 0; j < i; j++) {
					if (rNum[j] == rNum[i]) {
						i--;
						break;
					}
				}
			}
		}
		DrawBoard1();
	}
}

void FinishGame1() {
	int x, y;
	int set1 = 0;
	ttype = 1; // 게임이 끝날 때도 블럭 모양이 변한다.
	for (y = 0; y < BH + 3; y++) {
		for (x = 0; x < BW + 3; x++) {
			if (board1[x][y] == BRICK) {
				gotoxy(BX1 + x * 2, BY + y); // 가상좌표를 실제좌표로 변환한 곳으로 커서를 이동시키고
				textcolor(GRAY2, BLACK);
				puts(arTile[ttype][BRICK]);
				board1[x][y] = EMPTY;
				set1 = 1;
				break;
			}
			if (x == BW + 2 && y == BH + 2) {
				finish1 = 0;
			}
		}
		if (set1 == 1)
			break;
	}
	textcolor(WHITE, BLACK);
	ttype = 0; // 게임이 끝날 때도 블럭 모양이 변한다.
}

void FinishGame2() {
	int x, y;
	int set2 = 0;
	ttype = 1; // 게임이 끝날 때도 블럭 모양이 변한다.
	for (y = 0; y < BH + 3; y++) {
		for (x = 0; x < BW + 3; x++) {
			if (board2[x][y] == BRICK) {
				gotoxy(BX2 + x * 2, BY + y); // 가상좌표를 실제좌표로 변환한 곳으로 커서를 이동시키고
				textcolor(GRAY2, BLACK);
				puts(arTile[ttype][BRICK]);
				board2[x][y] = EMPTY;
				set2 = 1;
				break;
			}
			if (x == BW + 2 && y == BH + 2) {
				finish2 = 0;
			}
		}
		if (set2 == 1)
			break;
	}
	textcolor(WHITE, BLACK);
	ttype = 0;
}

void PrintInfo1()
{
	gotoxy(34, 9); printf("점수 : %d ", score1);
	gotoxy(34, 10); printf("벽돌 : %d 개 ", bricknum1);
}

void PrintInfo2()
{
	gotoxy(50, 9); printf("점수 : %d ", score2);
	gotoxy(50, 10); printf("벽돌 : %d 개 ", bricknum2);
}