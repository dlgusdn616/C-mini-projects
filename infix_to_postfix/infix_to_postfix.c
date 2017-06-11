#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int element;
char evalchar[128]; // 후위식을 계산할 때 사용할 전역변수

typedef struct StackNode {
	element item;
	struct StackNode *link;
} StackNode;

typedef struct {
	StackNode *top;
	int count; // 원소의 저장 갯수를 나타내주는 변수
} LinkedStackType;

void init(LinkedStackType *s) {
	s->top = NULL;
	s->count = 0;
}

int is_empty(LinkedStackType *s) {
	return (s->top == NULL || s->count == 0);
}

int printUI();
void push(LinkedStackType*, element);
element pop(LinkedStackType*);
element peek(LinkedStackType*);
void deleteWhiteSpace(char *); // 사용자가 입력한 문자열의 공백을 제거
void make_stack_node(char *, LinkedStackType*); // 입력한 문자열을 연결된 스택 구조로 변환
int prec(char);
void infix_to_postfix(LinkedStackType*); // 중위식을 후위식으로 변환출력
int eval(char*); // 후위 표기 수식 계산함수

void main() {
	char poly[128];
	int i;
	do {
		for (i = 0; i < 128; i++)
			evalchar[i] = NULL; // 전역변수 초기화
		LinkedStackType *s = (LinkedStackType *)malloc(sizeof(LinkedStackType));
		printf("중위식으로 문자열을 입력하세요.\n");
		gets(poly);
		printf("사용자가 입력한 중위식: %s\n", poly);
		deleteWhiteSpace(poly); // 사용자가 입력한 중위식의 공백문자를 제거
		make_stack_node(poly, s); // 사용자가 입력한 다항식을 연결된 스택 구조로 변환
		printf("후위식으로 변환 후 출력: ");
		infix_to_postfix(s); // 후위식으로 변환 후 출력
		printf("후위식을 계산한 값: %d\n", eval(evalchar));
		free(s);
	} while (printUI() - '0' == 1);
	exit(1);
}

int printUI() {
	putchar('\n');
	printf("계속 하시겠습니까?\n");
	printf("1 : 다시 처음부터 \t 2 : 프로그램 종료\n");
	return getch();
}

void push(LinkedStackType *s, element item) {
	StackNode *temp = (StackNode*)malloc(sizeof(StackNode));
	if (temp == NULL)
		fprintf(stderr, "메모리 할당에러\n");
	else {
		temp->item = item;
		temp->link = s->top;
		s->top = temp;
		(s->count)++;
	}
}

element pop(LinkedStackType *s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택이 비어있음\n");
		exit(1);
	}
	else {
		StackNode *temp = s->top;
		element item = temp->item;
		s->top = s->top->link;
		free(temp);
		(s->count)--;
		return item;
	}
}

element peek(LinkedStackType *s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택이 비어있음\n");
		exit(1);
	}
	else {
		return s->top->item;
	}
}

void deleteWhiteSpace(char *poly) {
	char *tmp;
	tmp = poly;
	while (strchr(poly, ' ') != '\0') { // poly1에 공백 문자가 있을 때
		tmp = strchr(poly, ' '); // 공백문자가 위치한 곳을 따로 저장
		*tmp = '\0';
		strcat(tmp, tmp + 1); // 
	}
}

void make_stack_node(char *poly, LinkedStackType *s) {
	int i, len;
	len = strlen(poly);
	init(s);
	for (i = 0; i < len; i++) {
		push(s, poly[i]);
	}
}

int prec(char op) {
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}

void infix_to_postfix(LinkedStackType* s) {
	int i = 0;
	char ch, top_op;
	char evalch;
	int eval_result;
	LinkedStackType p; // 기존에 저장된 연결된 스택노드 s에 저장된 순서를 거꾸로 뒤집기 위한 변수 p
	LinkedStackType *temp = (LinkedStackType *)malloc(sizeof(LinkedStackType));
	// 후위 변환된 노드들을 저장할 연결된 스택타입의 포인터
	init(&p);
	while (!is_empty(s)) { // 기존의 연결된 스택노드 s를 없애고 그 순서를 바꾼 그대로 p로 옮긴다.
		push(&p, pop(s));
	}
	init(temp); 
	while(!is_empty(&p)) {
		ch = pop(&p);
		switch (ch) {
		case '+': case '-': case '*': case '/': // 연산자
			while (!is_empty(temp) && (prec(ch) <= prec(peek(temp)))) {
				evalch = pop(temp);
				printf("%c", evalch);
				evalchar[i++] = evalch;
			}
			push(temp, ch);
			break;
		case '(':
			push(temp, ch);
			break;
		case ')':
			top_op = pop(temp);
			while (top_op != '(') {
				printf("%c", top_op);
				evalchar[i++] = top_op;
				top_op = pop(temp);
			}
			break;
		default:
			printf("%c", ch);
			evalchar[i++] = ch;
			break;
		}
	}

	while (!is_empty(temp)) { // 스택에 저장된 연산자들 출력
		evalch = pop(temp);
		printf("%c", evalch);
		evalchar[i++] = evalch;
	}
	putchar('\n');
	evalchar[i] = '\0';
	free(temp);
}

int eval(char *evalch) {
	int op1, op2, value, i = 0;
	int len = strlen(evalch);
	char ch;
	LinkedStackType temp;
	init(&temp);
	for(i=0; i<len; i++) {
		ch = evalch[i];
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
			// 입력이 피연사자일 경우
			value = ch - '0';
			push(&temp, value);
		}
		else {
			op2 = pop(&temp); // 정수값 리턴
			op1 = pop(&temp); // 정수값 리턴 
			switch (ch) {
			case '+': push(&temp, op1 + op2); break;
			case '-': push(&temp, op1 - op2); break;
			case '*': push(&temp, op1 * op2); break;
			case '/': push(&temp, op1 / op2); break;
			}
		}
	}
	return pop(&temp);
}