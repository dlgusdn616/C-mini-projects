// 1233040 이현우
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct ListNode { // 다항식의 각 항의 정보를 저장하기 위한 노드 구조체
	int coef;
	int expon;
	struct ListNode *link;
} ListNode;

typedef struct ListHeader { // 헤더포인터 역할을 하는 구조체
	int length;
	ListNode *head;
	ListNode *tail; // 제일 끝에 있는 다항식의 항의 정보를 읽어올 수 있다.
} ListHeader;

void init(ListHeader *plist); // 리스트를 초기화 
void error(char *message); // 동적할당 실패시 에러메시지 출력
void insert_node_last(ListHeader *plist, int coef, int expon); // 새로운 노드를 만들어 동적 할당

void poly_add(ListHeader *plist1, ListHeader *plist2, ListHeader *plist3); // 연결리스트로 구현된 다항식의 덧셈 함수
void poly_sub(ListHeader *plist1, ListHeader *plist2, ListHeader *plist3); // 뺄셈 함수

void deletePoly(ListHeader *plist); // 동적할당된 리스트를 반환
void deleteWhiteSpace(char *poly1, char *poly2); // 사용자가 입력한 문자열 중 빈칸을 지워주는 함수

void printUI(char *poly1, char *poly2); // 두 개의 문자열로 된 다항식을 입력 받는 UI
int printInputUI(); // 입력 받은 다항식을 바탕으로 어떤 연산을 할 지 나타내는 UI

void changeStringToInteger(char *poly, ListHeader *list); // 사용자가 입력한 문자열을 정수 형태로 계수와 지수로 변환
														  // 변환한 계수와 지수 정보를 리스트에 새로운 노드를 만들면서 저장 (insert_node_last를 함수 내에 포함)

void poly_print(int nUser, ListHeader *plist); // 각 경우에 수에 맞는 최적화된 다항식 출력
											   // 계수가 1이거나 지수가 1이거나 0일 때와 같은 상황들을 인간친화적인 표현으로 출력

void main() {
	char poly1[128];
	char poly2[128];
	int nUser;
	ListHeader list1, list2, list3; // list1, list2는 사용자가 입력할 다항식이 연결리스트로 저장할 구조체 변수
									// list3는 두 다항식의 연산 결과가 입력될 구조체 변수

	printf("┏━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃   학번 : 1233040     이름 : 이 현 우   ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("┏━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃	  Start Polynomial Program 	 ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━┛\n");

	do {
		init(&list1);	init(&list2);	init(&list3); // 각 구조체변수 초기화
		printUI(poly1, poly2); // 사용자로부터 입력 받는 UI 출력

		deleteWhiteSpace(poly1, poly2); // 사용자가 입력한 문자열에서 공백을 비우는 함수

		changeStringToInteger(poly1, &list1); // 사용자가 입력한 다항식 문자열을 정수형태로 list1 구조체 변수에 저장
		changeStringToInteger(poly2, &list2); // 사용자가 입력한 다항식 문자열을 정수형태로 list2 구조체 변수에 저장

		nUser = 1; // 초기화를 위한 단순 값 대입 ( + while루프 진입용)

		while ((nUser - '0') != 3) { // getch()로 입력을 받기 때문에 아스키코드값으로 계산
			nUser = printInputUI();
			switch (nUser - '0') { // 아스키코드값으로 계산, 사용자는 인터페이스 상에서 1~4번만 누르면 된다
			case 1:
				poly_add(&list1, &list2, &list3);
				poly_print(nUser - '0', &list3);
				deletePoly(&list3); // 다음 연산을 위해 list3 동적 메모리 반납 및 초기화
				init(&list3);
				break;
			case 2:
				poly_sub(&list1, &list2, &list3);
				poly_print(nUser - '0', &list3);
				deletePoly(&list3); // 다음 연산을 위해 list3 동적 메모리 반납 및 초기화
				init(&list3);
				break;
			case 3: // 프로그램을 재실행하기 전 동적할당 받은 메모리를 전부 반납
				deletePoly(&list1);	deletePoly(&list2);
				deletePoly(&list3);
				break;
			case 4: // 프로그램을 종료하기 전 동적할당 받은 메모리를 전부 반납
				deletePoly(&list1);	deletePoly(&list2);
				deletePoly(&list3);
				exit(1);
			}
		}
	} while ((nUser - '0') == 3);
}

void printUI(char *poly1, char *poly2) {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃    Input Two Polynomial That You Want To Calculate   ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("		(1) : ");	gets(poly1);
	printf("		(2) : ");	gets(poly2);
}

int printInputUI() {
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃       1. Poly Add		2. Poly Sub		3. Roll Back		4.Exit Program       ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	return (int)getch();
}

void init(ListHeader *plist) {
	plist->length = 0;
	plist->head = plist->tail = NULL;
}

void error(char *message) {
	fprintf(stderr, "%s\n", message);
	exit(1);
}

void insert_node_last(ListHeader *plist, int coef, int expon) {
	ListNode *temp = (ListNode*)malloc(sizeof(ListNode));
	if (temp == NULL)	error("메모리 할당 에러");
	temp->coef = coef;
	temp->expon = expon;
	temp->link = NULL;
	if (plist->tail == NULL) {
		plist->head = plist->tail = temp;
	}
	else {
		plist->tail->link = temp;
		plist->tail = temp;
	}
	plist->length++;
}

void poly_add(ListHeader *plist1, ListHeader *plist2, ListHeader *plist3) {
	ListNode *a = plist1->head;
	ListNode *b = plist2->head;
	int sum;

	while (a != NULL && b != NULL) {
		if (a->expon == b->expon) {
			sum = a->coef + b->coef;
			if (sum != 0)	insert_node_last(plist3, sum, a->expon);
			a = a->link; b = b->link;
		}
		else if (a->expon > b->expon) {
			insert_node_last(plist3, a->coef, a->expon);
			a = a->link;
		}
		else {
			insert_node_last(plist3, b->coef, b->expon);
			b = b->link;
		}
	}

	if (a != NULL) {
		for (; a != NULL; a = a->link) {
			insert_node_last(plist3, a->coef, a->expon);
		}
	}
	if (b != NULL) {
		for (; b != NULL; b = b->link) {
			insert_node_last(plist3, b->coef, b->expon);
		}
	}
}

void poly_sub(ListHeader *plist1, ListHeader *plist2, ListHeader *plist3) {
	ListNode *a = plist1->head;
	ListNode *b = plist2->head;
	int sum;

	while (a != NULL && b != NULL) {
		if (a->expon == b->expon) {
			sum = a->coef - b->coef;
			if (sum != 0)	insert_node_last(plist3, sum, a->expon);
			a = a->link; b = b->link;
		}
		else if (a->expon > b->expon) {
			insert_node_last(plist3, a->coef, a->expon);
			a = a->link;
		}
		else {
			insert_node_last(plist3, (-1) * b->coef, b->expon);
			b = b->link;
		}
	}

	if (a != NULL) {
		for (; a != NULL; a = a->link) {
			insert_node_last(plist3, a->coef, a->expon);
		}
	}
	if (b != NULL) {
		for (; b != NULL; b = b->link) {
			insert_node_last(plist3, (-1) * b->coef, b->expon);
		}
	}
}

void deletePoly(ListHeader *plist) { // 동적할당 받은 다항식을 반납하는 함수
	ListNode *phead = plist->head;
	if (phead) {
		ListNode *p = phead;
		while (p != NULL) {
			phead = phead->link;
			free(p);
			p = phead;
		}
	}
}

void poly_print(int nUser, ListHeader *plist) { // 출력에 대한 예외 설정
	ListNode *p = plist->head;
	int nNode = plist->length;
	int cnt = 0;
	putchar('\n');
	switch (nUser) {
	case 1:
		printf("\tPoly Add\t다항식 출력 결과: ");
		break;
	case 2:
		printf("\tPoly Sub\t다항식 출력 결과: ");
		break;
	}

	for (; p; p = p->link) {
		cnt++;
		if (cnt == 1) { // 다항식 첫 번째 항의 예외처리
			if (p->coef < 0) { // 첫 번째 항의 계수가 0보다 작을 때
				if (p->coef == -1) { // 첫 번째 항의 계수가 -1일 때
					if (p->expon == 1) { // 해당 지수가 1일 때는 계수 없이 -x만 출력
						printf("\t\t-x ");
					}
					else if (p->expon == 0) { // 해당 지수가 0일 때는 상수만 출력
						printf("\t\t%d ", p->coef);
					}
					else {
						printf("\t\t-x^%d ", p->expon); // 지수가 1보다 크면 지수 출력
					}
				}
				else { // 첫 번째 항의 계수가 -2 ~ -9일 때
					if (p->expon == 1) { // 해당 지수가 1일 때는 계수 (음수) 와 함께 x 출력
						printf("\t\t%dx ", (p->coef));
					}
					else if (p->expon == 0) { // 해당 지수가 0일 때는 계수 (음수) 만 출력
						printf("\t\t%d ", p->coef);
					}
					else {
						printf("\t\t%dx^%d ", p->coef, p->expon); // 지수가 1보다 크면 계수 (음수) 와 함께 지수 출력
					}
				}
			}
			else if (p->coef > 0) { // 첫 번째 항의 계수가 0보다 클 때
				if (p->coef == 1) { // 첫 번째 항의 계수가 1일 때
					if (p->expon == 1) { // 해당 지수가 1일 때는 계수 없이 x만 출력
						printf("\t\tx ");
					}
					else if (p->expon == 0) { // 해당 지수가 0일 때는 상수만 출력
						printf("\t\t%d ", p->coef);
					}
					else {
						printf("\t\tx^%d ", p->expon); // 지수가 1보다 크면 지수 출력
					}
				}
				else { // 계수가 1보다 클 때
					if (p->expon == 1) { // 해당 지수가 1일 때는 계수와 x만 출력
						printf("\t\t%dx ", p->coef);
					}
					else if (p->expon == 0) { // 해당 지수가 0일 때는 상수만 출력
						printf("\t\t%d ", p->coef);
					}
					else {
						printf("\t\t%dx^%d ", p->coef, p->expon);
					}
				}
			}
		}
		else {
			if (p->coef < 0) { // 다항식 첫 번째 항 이후의 식을 출력
				printf("- "); // 계수가 음수일 때는 연산자 = '-'
				if (p->coef == -1) { // 계수가 -1일 때
					if (p->expon == 0) { // 지수가 0일 때는 계수만 출력
						printf("%d ", (-1) * (p->coef));
					}
					else if (p->expon == 1) { // 지수가 1일 때는 x만 출력 (앞에 '-'연산자가 있기 때문에 x만 출력)
						printf("x ");
					}
					else { // 지수가 1보다 클 때 해당 계수와 지수까지 출력
						printf("%dx^%d ", (-1) * (p->coef), p->expon);
					}
				}
				else { // 계수가 -2 ~ -9 일 때
					if (p->expon == 0) { // 지수가 0일 때 해당 계수만 출력 (-1을 곱하는 이유는 앞에 '-'연산자가 있기 때문)
						printf("%d ", (-1) * (p->coef));
					}
					else if (p->expon == 1) { // 지수가 1일 때 해당 계수와 x만 출력 (앞에 '-'연산자가 있기 때문에 x만 출력)
						printf("%dx ", (-1) * (p->coef));
					}
					else {
						printf("%dx^%d ", (-1) * (p->coef), p->expon); // 지수가 1보다 클 때 해당 계수와 지수까지 출력
					}
				}
			}
			else { // 계수가 양수일 때
				printf("+ ");
				if (p->coef == 1) { // 계수가 1일 때
					if (p->expon == 0) { // 지수가 0일 때는 계수만 출력
						printf("%d ", (p->coef));
					}
					else if (p->expon == 1) { // 지수가 1일 때는 x만 출력
						printf("x ");
					}
					else { // 지수가 1보다 클 때는 x의 지수승 출력
						printf("x^%d ", p->expon);
					}
				}
				else { // 계수가 1보다 클 때
					if (p->expon == 0) { // 지수가 0일 때 해당 계수만 출력
						printf("%d ", (p->coef));
					}
					else if (p->expon == 1) { // 지수가 1일 때 해당 계수와 x만 출력
						printf("%dx ", (p->coef));
					}
					else {
						printf("%dx^%d ", (p->coef), p->expon);
					}
				}
			}
		}
	}
	printf("\n\n");
}

void deleteWhiteSpace(char *poly1, char *poly2) {
	char *tmp1, *tmp2;
	tmp1 = poly1;	tmp2 = poly2;
	while (strchr(poly1, ' ') != '\0') { // poly1에 공백 문자가 있을 때
		tmp1 = strchr(poly1, ' '); // 공백문자가 위치한 곳을 따로 저장
		*tmp1 = '\0';
		strcat(tmp1, tmp1 + 1); // 
	}
	while (strchr(poly2, ' ') != '\0') {
		tmp2 = strchr(poly2, ' ');
		*tmp2 = '\0';
		strcat(tmp2, tmp2 + 1);
	}
}

void changeStringToInteger(char *poly, ListHeader *list) {
	int coef;
	int expon;
	char chInt[128];
	int cnt = 0; // 사용자가 입력한 계수의 자릿수를 최대 128자리까지 받기 위해 필요한 카운팅변수
	char *tmp = poly;
	while (*tmp != '\0') {
		if (*tmp == '-') { // 입력된 항이 '-' 일 경우 (음수 처리 루틴)
			tmp++;
			if (*tmp >= 48 && *tmp <= 57) { // 상수가 존재할 때 (0~9)
				if (*(tmp + 1) >= 48 && *(tmp + 1) <= 57) { // 연속된 상수가 존재할 때
					cnt = 0; // 초기화가 안되어 있을 가능성 염두
					while (*tmp != '\0' && *tmp != 'x' && *tmp != 'X' && *tmp != '+' && *tmp != '-') { // 몇 자리 계수인지 확인하는 루틴
						chInt[cnt++] = *(tmp++); // 캐릭터 배열에 차례대로 저장 후 포인터 이동
					}
					chInt[cnt] = '\0';
					coef = (-1) * atoi(chInt); // 음수 계수 저장
					cnt = 0; // 카운팅 변수 초기화
				}
				else { // 연속된 상수가 존재하지 않을 때, 즉 상수가 한 자릿 수 일 때
					coef = *(tmp++) - '0'; // 값을 계수로 저장시켜준 후, 포인터 한 칸 이동
					coef *= (-1); // 음수이기 때문에 음수 처리를 해준다.
				}
				if (*tmp == 'x' || *tmp == 'X') { // 이동한 위치에 변수가 있을 경우
					if (*(tmp + 1) == '^') { // 지수가 1이상일 경우
						expon = (*(tmp + 2)) - '0';
						insert_node_last(list, coef, expon);
						tmp = tmp + 3;
					}
					else { // 지수가 1일 경우
						expon = 1;
						insert_node_last(list, coef, expon);
						tmp++;
					}
				}
				else { // 마지막 항의 상수였을 경우
					expon = 0;
					insert_node_last(list, coef, expon);
				}
			}
			else if (*tmp == 'x' || *tmp == 'X') { // 계수가 -1, 변수가 존재할 때
				coef = -1;
				if (*(tmp + 1) == '^') { // 지수가 1이상일 경우
					expon = (*(tmp + 1)) - '0';
					insert_node_last(list, coef, expon);
					tmp = tmp + 3;
				}
				else { // 지수가 1일 경우
					expon = 1;
					insert_node_last(list, coef, expon);
					tmp++;
				}
			}
		}
		else { // 입력된 항이 '-'가 아닐 경우 (양수 처리 루틴)
			if (*tmp == '+') {
				tmp++;
			}
			if (*tmp >= 48 && *tmp <= 57) { // 상수가 등장했을 때 (0~9)
				if (*(tmp + 1) >= 48 && *(tmp + 1) <= 57) { // 연속된 상수가 존재할 때
					cnt = 0; // 초기화가 안되어 있을 수 있을 가능성 염두
					while (*tmp != '\0' && *tmp != 'x' && *tmp != 'X' && *tmp != '+' && *tmp != '-') { // 몇 자리 계수인지 확인하는 루틴
						chInt[cnt++] = *(tmp++); // 캐릭터 배열에 차례대로 저장 후 포인터 이동
					}
					chInt[cnt] = '\0';
					coef = atoi(chInt); // 계수 저장
					cnt = 0;
				}
				else { // 연속된 상수가 존재하지 않을 때, 즉 상수가 한 자릿 수 일 때
					coef = *(tmp++) - '0'; // 값을 계수로 저장시켜준 후, 포인터 한 칸 이동
				}
				if (*tmp == 'x' || *tmp == 'X') { // 이동한 위치에 변수가 있을 경우
					if (*(tmp + 1) == '^') { // 지수가 1이상일 경우
						expon = (*(tmp + 2)) - '0'; // 캐릭터로 입력되어 있는 지수이기 때문에 아스키 값 48에 해당하는 '0'을 빼준다.
						insert_node_last(list, coef, expon);
						tmp = tmp + 3;
					}
					else { // 지수가 1일 경우
						expon = 1;
						insert_node_last(list, coef, expon);
						tmp++;
					}
				}
				else { // 마지막 항의 상수일 경우
					expon = 0;
					insert_node_last(list, coef, expon);
				}
			}
			else if (*tmp == 'x' || *tmp == 'X') { // 계수가 1, 변수가 존재할 때
				coef = 1;
				if (*(tmp + 1) == '^') { // 지수가 1이상일 경우
					expon = (*(tmp + 1)) - '0';
					insert_node_last(list, coef, expon);
					tmp = tmp + 3;
				}
				else { // 지수가 1일 경우
					expon = 1;
					insert_node_last(list, coef, expon);
					tmp++;
				}
			}
		}
	}
}