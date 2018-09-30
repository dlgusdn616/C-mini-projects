# Polynomial Program

## 1. 개요

다항식의 덧셈과 뺄셈이 가능한 프로그램이다.

## 2. 조작법

1. 연산을 원하는 두 개의 다항식을 차례로 입력한다.

![01-start](images/01-start.png)

![02-input](images/02-input.png)

2. 위 인터페이스에 구현되어 있는 번호를 보고 원하는 기능을 선택한다. 먼저 1번 Poly Add를 선택하여 다항식의 덧셈을 실행시켜보고 다음으로 2번 Poly Sub를 선택하여 다항식의 뺄셈을 실행시켜본다.

![03-add-sub](images/03-add-sub.png)

3. 실행 뒤 새로운 다항식으로 테스트해보고 싶다면, 3번 Roll Back 기능을 호출하여 새로운 다항식을 입력 받으면 된다.

![04-roll-back](images/04-roll-back.png)

## 3. 입력예시 및 제한

1. 변수는 x만 사용한다.
2. 지수를 입력할 때는 `^` 기호를 사용한다.
   ex) `5x^3 + 2x^2 + 2x + 1`
   * 다항식을 입력할 때는 높은 차수부터 차례대로 입력한다.
   * 지수가 1일 때, 그리고 0일 때 (마지막 항의 상수일 때)는 지수를 생략할 수 있다.
   * 입력시 공백을 입력해도 된다.

## 4. 핵심 알고리즘 설명

### 4.1 사용자가 다항식을 입력할 때 공백이 있다면 공백을 삭제

```c
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
```

문자열에서 공백을 탐색하여 삭제한다. 

### 4.2 문자열에서 계수와 지수를 구분한다.

```c
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
```

문자열을 가리키고 있는 포인터를 이동시키면서 계수, 변수, 지수를 식별한다.

`-` 연산자가 등장했을 때, 그 이외의 경우(변수는 있지만 지수가 1이거나 0인 경우 등)를 각각 처리할 수 있다. 

### 4.3 구분된 지수와 계수는 연결리스트로 구현하여 처리한다.

```c
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
```

다항식 연산 때 사용되는 자료구조이다.