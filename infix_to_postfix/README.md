# Program that change infix to postfix 

## 1. 개요

중위식을 입력하면 후위식으로 변환하여 출력하는 프로그램이다. 

## 2. 제약 조건

다음과 같은 경우 프로그램은 정상적으로 동작하지 않는다.

* 아무것도 입력하지 않는 경우
* 숫자와 연산자, 괄호 이외의 것을 입력하는 경우
* 괄호를 짝에 맞게 입력하지 않는 경우

## 3. 메인 알고리즘 및 코드

다음과 같은 자료구조가 사용되었다.

```c
typedef struct StackNode {
	element item;
	struct StackNode *link;
} StackNode;

typedef struct {
	StackNode *top;
	int count; // 원소의 저장 갯수를 나타내주는 변수
} LinkedStackType;
```

```c
void deleteWhiteSpace(char*);	// 사용자가 입력한 문자열의 공백을 제거
void make_stack_node(char*, LinkedStackType*);	// 입력한 문자열을 연결된 스택 구조로 변환
void infix_to_postfix(LinkedStackType*);	// 중위식을 후위식으로 변환 출력
```

### 3.1 연결된 스택 구조로 변환

```c
void make_stack_node(char *poly, LinkedStackType *s) {
	int i, len;
	len = strlen(poly);
	init(s);
	for (i = 0; i < len; i++) {
		push(s, poly[i]);
	}
}
```

공백이 없는 문자열 poly를 연결된 스택 노드로 생성한다.

### 3.2 중위식을 후위식으로 변환 후 출력

```c
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
```