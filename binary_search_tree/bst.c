#include <stdio.h>
#include <stdlib.h>

typedef enum { FALSE, TRUE } bool;	// 효과적인 논리처리를 위한 bool 타입 정의

typedef int element;

typedef struct _tNode {				// 트리의 노드 구조체
	element data;					// 노드의 키 값
	struct _tNode *left, *right;
} tNode;

typedef struct _Queue {
	tNode *queue[10000];
	int front;
	int rear;
} Queue;

int print_menu();								// 프로그램의 기능을 소개하는 메뉴를 출력
void create_node(tNode **root, element key);	// 특정 키 값을 갖는 노드를 트리에 추가
void delete_node(tNode **root, element key);	// 특정 키 값을 갖는 노드를 트리에서 삭제
void preorder(tNode *root);						// 전위 순회로 모든 노드를 방문 & 출력
void inorder(tNode *root);						// 중위 순회로 모든 노드를 방문 & 출력
void postorder(tNode *root);					// 후위 순회로 모든 노드를 방문 & 출력
void search_node(tNode *root, element key);		// 트리에서 특정 키 값이 존재하는지 찾는다.

												// 다시한 번 직접 작성할 필요가 있는 함수
int tree_height(tNode *root);					// 트리의 높이를 반환한다.
int node_count(tNode *root);					// 트리에 존재하는 노드의 개수를 반환한다.

void delete_tree(tNode *root);					// 프로그램을 종료하기 전 동적할당으로 생성한 모든 노드를 반환한다.
int min_key_value(tNode *root);					// 트리에서 가장 작은 키 값을 찾는다.
int max_key_value(tNode *root);					// 트리에서 가장 큰 키 값을 찾는다.

void init(Queue *qu);							// Queue의 초기화를 진행한다.
void push(Queue *qu, tNode *node);				// node를 Queue에 담는 작업
tNode* pop(Queue *qu);							// tNode* 를 담고 있는 Queue에서 pop


int level_find(tNode *root, element key);		// key 값이 속해 있는 레벨을 출력한다.
void level_print(tNode *root, int level);		// 입력한 level에 해당하는 노드들을 출력한다.

int main()
{
	tNode *root = NULL;
	int userInput;			// UI에서 사용자가 입력할 값
	element data;
	int level;

	printf("************* 1233040 Huynwoo Lee ver. *************\n\n");
	printf("you cannot input minus value and zero in this program\n\n");
	while (TRUE)
	{
		userInput = print_menu();

		switch (userInput)
		{
		case 1:
			printf("key value: ");
			scanf("%d", &data);
			while (getchar() != '\n');
			create_node(&root, data);
			break;
		case 2:
			printf("key value: ");
			scanf("%d", &data);
			while (getchar() != '\n');
			delete_node(&root, data);
			break;
		case 3:
			preorder(root);
			putchar('\n');
			break;
		case 4:
			inorder(root);
			putchar('\n');
			break;
		case 5:
			postorder(root);
			putchar('\n');
			break;
		case 6:
			printf("key value: ");
			scanf("%d", &data);
			while (getchar() != '\n');
			search_node(root, data);
			break;
		case 7:
			if (root == NULL)
			{
				printf("you can use this function at least 1 node\n");
				break;
			}
			printf("hieght of tree is %d\n", tree_height(root));
			break;
		case 8:
			printf("count of node in tree is %d\n", node_count(root));
			break;
		case 9:
			delete_tree(root);
			exit(1);
			break;
		case 49:
			min_key_value(root);
			break;
		case 50:
			max_key_value(root);
			break;
		case 51:
			printf("key value: ");
			scanf("%d", &data);
			while (getchar() != '\n');
			printf("level is %d\n", level_find(root, data));
			break;
		case 52:
			printf("level value: ");
			scanf("%d", &level);
			while (getchar() != '\n');
			level_print(root, level);
			break;
		}
	}
	return 0;
}

// 사용자가 기능을 선택할 수 있는 인터페이스를 제공한다.
int print_menu()
{
	int select;

	while (TRUE)
	{
		printf("*********************\n");
		printf("1: insert_node\n");
		printf("2: delete_node\n");
		printf("3: preorder\n");
		printf("4: inorder\n");
		printf("5: postorder\n");
		printf("6: search_node\n");
		printf("7: tree_height\n");
		printf("8: node_count\n");
		printf("9: exit_program\n");
		printf("a: min_key_value\n");
		printf("b: max_key_value\n");
		printf("c: level that has node what you input\n");
		printf("d: node's count of level and its elements\n");
		printf("*** Which select? : ");

		select = getchar();

		// 잘못 입력했을 경우 오류메시지 출력과 함께 다시 입력하게끔 인터페이스 창을 띄운다.
		if (getchar() != '\n')
		{
			printf("what you input is wrong value. retry input\n");
			while (getchar() != '\n');
		}
		else if ((select < '1' || select > '9') && (select != 'a' && select != 'b' && select != 'c' && select != 'd'))
		{
			printf("what you input is wrong value. retry input\n");
		}
		else
			break;

	}

	// 메인함수에서 switch case 사용시 정수 체계를 사용했으므로, 캐릭터로 그대로 반환하는 것이 아니라 '0'을 뺀 값을 반환한다.
	return (select - '0');
}

// 특정 키 값을 담은 노드를 동적으로 생성해서 루트에 붙인다.
void create_node(tNode **root, element key)
{
	tNode *parent = *root;
	tNode *child = NULL;
	tNode *newNode = (tNode *)malloc(sizeof(tNode));
	newNode->data = key;
	newNode->left = newNode->right = NULL;

	if (parent == NULL)
	{
		*root = newNode;
		return;
	}

	while (parent != NULL)
	{
		if (parent->data < newNode->data)
		{
			if (parent->right == NULL)
			{
				parent->right = newNode;
				return;
			}
			else
				parent = parent->right;
		}
		else if (parent->data == newNode->data)
		{
			fprintf(stderr, "same key value already exists in this tree\n");
			free(newNode);
			return;
		}
		else
		{
			if (parent->left == NULL)
			{
				parent->left = newNode;
				return;
			}
			else
				parent = parent->left;
		}
	}
	return;
}

// 특정 키 값을 담은 노드를 삭제하고, 원래의 탐색 트리가 유지될 수 있게 작업을 해준다.
void delete_node(tNode **root, element key)
{
	tNode *p, *t, *child, *succ_p, *succ;
	p = NULL;
	t = *root;

	while (t != NULL && t->data != key)
	{
		p = t;
		(t->data > key) ? (t = t->left) : (t = t->right);
	}

	if (t == NULL) {
		fprintf(stderr, "cannot find the value what you want to delete.\n");
		return;
	}

	if (t->left == NULL && t->right == NULL)
	{
		if (p != NULL)
		{
			if (p->left == t)
				p->left = NULL;
			else
				p->right = NULL;
		}
		else
			*root = NULL;

	}

	else if (t->left == NULL || t->right == NULL) {
		child = (t->left != NULL) ? t->left : t->right;
		if (p != NULL)
		{
			if (p->left == t)
				p->left = child;
			else
				p->right = child;
		}
		else
			*root = NULL;
	}

	else {
		succ_p = t;
		succ = t->right;

		while (succ->left != NULL) {
			succ_p = succ;
			succ = succ->left;
		}

		// while 루프를 돌고난 후 더 이상 왼쪽으로 진행할 수 없을 때
		if (succ_p->left == succ)	// while 루프를 돌았다면 이 조건을 만족할 것이다.
			succ_p->left = succ->right;	// 삭제될 노드의 우측의 것을 succ_p의 좌측에 붙인다.
		else // 우측 경사이진 트리라 아예 한 번도 while루프를 돌지 못했을 때
			succ_p->right = succ->right;

		// 삭제할 노드의 데이터 값을 복사
		t->data = succ->data;
		// 삭제할 노드로 주소값 변경
		t = succ;
	}
	// 노드 삭제
	free(t);
	printf("deleting node is successful.\n");
}

// 전위순회
void preorder(tNode *root)
{
	if (root != NULL)
	{
		printf("%d ", root->data);
		preorder(root->left);
		preorder(root->right);
	}
}

// 중위순회
void inorder(tNode *root)
{
	if (root != NULL)
	{
		inorder(root->left);
		printf("%d ", root->data);
		inorder(root->right);
	}
}

// 후위순회
void postorder(tNode *root)
{
	if (root != NULL)
	{
		postorder(root->left);
		postorder(root->right);
		printf("%d ", root->data);
	}
}

// 특정 키 값을 갖는 노드를 찾아 그 키 값을 출력해준다.
void search_node(tNode *root, element key)
{
	printf("path: ");
	while (root != NULL)
	{
		printf("%d ", root->data);
		if (root->data == key)
		{
			printf("\t finally find it %d\n", key);
			return;
		}
		else if (root->data < key)
			root = root->right;
		else
			root = root->left;
	}
	printf("dosen't exist key in this tree\n");
	return;
}

// 트리의 높이를 구하여 콘솔화면에 출력한다.
int tree_height(tNode *root)
{
	int height = 0;
	if (root != NULL)
		height = 1 + max(tree_height(root->left), tree_height(root->right));

	return height;
}

// 트리의 노드 갯수를 구하여 콘솔화면에 출력한다.
int node_count(tNode *root)
{
	int count = 0;
	if (root == NULL)
		return 0;
	count = 1 + node_count(root->left) + node_count(root->right);

	return count;
}

// 트리에 존재하는 모든 노드를 반환한다. 프로그램 종료를 눌렀을 때 실행된다.
void delete_tree(tNode *root)
{
	if (root == NULL)
		return;
	else if (root->left == NULL && root->right == NULL)
	{
		free(root);
		return;
	}
	delete_tree(root->left);
	delete_tree(root->right);
	free(root);
}

// 트리에서 최소값을 찾아 출력한다.
int min_key_value(tNode *root)
{
	tNode *p = NULL;

	// 아무 값도 없으므로 최소 값을 찾을 수 없다.
	if (root == NULL) {
		fprintf(stderr, "there is no value to find.\n");
		return -1;
	}
	while (root != NULL) {
		p = root;
		root = root->left;
	}
	printf("min_value in this tree is %d\n", p->data);
}

// 트리에서 최대값을 찾아 출력한다.
int max_key_value(tNode *root)
{
	tNode *p = NULL;

	// 아무 값도 없으므로 최소 값을 찾을 수 없다.
	if (root == NULL) {
		fprintf(stderr, "there is no value to find.\n");
		return -1;
	}
	while (root != NULL) {
		p = root;
		root = root->right;
	}
	printf("min_value in this tree is %d\n", p->data);
}

// Queue의 초기화를 진행한다.
void init(Queue *qu)
{
	int i;
	qu->front = 1;
	qu->rear = 0;

	for (i = 0; i < 10000; i++)
	{
		qu->queue[i] = NULL;
	}
}

// node를 Queue에 담는 작업
void push(Queue *qu, tNode *node)
{
	qu->queue[++(qu->rear)] = node;
}

// tNode* 를 담고 있는 Queue에서 pop
tNode* pop(Queue *qu)
{
	return qu->queue[(qu->front)++];
}

// key 값이 속해 있는 레벨을 출력한다.
int level_find(tNode *root, element key)
{
	int level = 0;
	while (root != NULL)
	{
		level++;
		if (root->data == key)
			return level;

		else if (root->data < key)
			root = root->right;
		else
			root = root->left;
	}
	printf("dosen't exist key in this tree\n");
	return -1;
}

// 특정 레벨에 있는 노드들과 그 개수를 보여준다.
void level_print(tNode *root, int level)
{
	// 레벨 출력이므로, 노드들 값을 따로 저장할 배열을 생성해서 모든 노드의 값을 배열에 저장한다.
	// 단, 저장할 때 주어진 레벨로 최대 저장할 노드의 개수를 미리 계산하여
	// 쓸데 없이 많은 저장 연산이 이뤄지지 않게끔 구성한다.
	tNode *tmp = NULL;
	element nodeArray[1000] = { 0 };
	int index = 1;
	int i;
	int sIndex = 1;
	int eIndex = 1;
	int maxIndex = 1;
	for (i = 0; i < level; i++)
		maxIndex *= 2;

	int node_cnt = 0;
	
	Queue qu;
	init(&qu);

	// 처음에 root 값을 넣는 것으로 함수 시작
	push(&qu, root);

	while (TRUE)	// 주어진 레벨 정보를 토대로 저장이 끝날 때까지 계속 반복한다.
	{
		// pop()을 했는데 NULL 이었다면 Queue에 NULL을 두 번 넣는다.
		// NULL이 아니고 정상적인 노드였다면, 그 밑에 자식 노드들을 큐에 넣는다.
		tmp = pop(&qu);

		if (index >= maxIndex)
			break;

		if (tmp != NULL)
		{
			push(&qu, tmp->left);
			push(&qu, tmp->right);

			nodeArray[index++] = tmp->data;
		}
		// 꺼낸 노드포인터가 NULL이라면 배열에는 그대로 0이 남아있게 된다. 따라서 index만 증가시킨다.
		else
		{
			push(&qu, NULL);
			push(&qu, NULL);
			index++;
		}
	}

	// 주어진 레벨을 토대로 startIndex를 유추한다.
	for (i = 0; i < level - 1; i++)
		sIndex *= 2;

	// 주어진 레벨을 토대로 endIndex를 유추한다.
	for (i = 0; i < level; i++)
		eIndex *= 2;
	eIndex--;
	
	printf("node of this level is ");

	for (i = sIndex; i <= eIndex; i++)
		if (nodeArray[i] != 0)
		{
			printf("%d ", nodeArray[i]);
			node_cnt++;
		}

	printf("\nthe count of node is %d\n", node_cnt);
}