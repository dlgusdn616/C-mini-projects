#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>

typedef struct _element // 희소행렬을 사용할 때 쓰일 구조체
{
	int row;
	int col;
	int val;
} element;

int printUI(void);
int endUI(void);
void runMatrixProgram(void);

void main()
{
	do
	{
		if (printUI() == 2)
			exit(1);
		else
			runMatrixProgram();
	} while (endUI() != -1);
	putchar('\n');
	exit(1);
}

int printUI(void)
{
	char ch;
	do
	{
		putchar('\n');
		printf("Select one of following items:\n");
		printf("\t1. Matrix program\n");
		printf("\t2. Exit\n");
		printf("Which do you want to select a number? (1-2) : ");
		if ((ch = getche()) == '1')
			return 1;
		else if ((ch = getche()) == '2')
			return 2;
	} while (ch != '1' || ch != '2');
}

int endUI(void)
{
	char ch = 0;
	do
	{
		if (ch != 0)
			putchar('\n');
		putchar('\n');
		printf("Do you want to try other program? (Y/N) : ");
		ch = getche();
	} while (ch != 'Y' && ch != 'y' && ch != 'N' && ch != 'n');
	if (ch == 'N' || ch == 'n')
		return -1;
	else
		return 1;

}

void runMatrixProgram(void)
{
	char ch;
	int rows, cols;
	int i, j;
	int terms = 0; // 희소행렬을 동적할당으로 만들 때, 필요한 0이 아닌 항의 갯수
	int temp; // 희소행렬에 원소를 저장할 때 쓰일 카운팅 변수
	int cnt = 0; // 마지막 6개 주소 출력을 위한 카운팅 변수
	int cntStruct = 0; // 구조체 배열에 접근할 때 쓰일 배열의 넘버 변수
	int **ppArr;
	element *pElement;

	putchar('\n');
	printf("Type the number of rows : ");
	scanf("%d", &rows);
	printf("Type the number of columns : ");
	scanf("%d", &cols);
	ppArr = (int **)malloc(sizeof(int *) * rows); // 행렬자체를 저장하는 방식을 나타낼 때 쓰일 int **ppArr 동적할당

	for (i = 0; i < rows; i++)
	{
		*(ppArr + i) = (int *)malloc(sizeof(int) * cols);
		printf("Type the %d elements of %d-th row : ", cols, i);
		for (j = 0; j < cols; j++)
		{
			scanf("%d", *(ppArr + i) + j);
			if (ppArr[i][j] != 0)
				terms++; // 희소행렬을 생성할 때 쓰일 변수, 0이 아닌 원소의 갯수를 카운팅한다.
		}
	}
	temp = terms;
	pElement = (element *)malloc(sizeof(element) * terms); // 희소행렬로 저장을 위한 동적할당으로 pElement 정의 (terms*element 크기로 할당)
	printf("sizeof (pElement): %d\n", sizeof(element));

	for (i = 0; i < rows; i++) // 위에서 할당된 희소행렬을 바탕으로 
	{
		for (j = 0; j < cols; j++)
		{
			if (ppArr[i][j] != 0)
			{
				pElement[cntStruct].row = i;
				pElement[cntStruct].col = j;
				pElement[cntStruct].val = ppArr[i][j];
				temp--;
				cntStruct++;
				if (temp == 0)
					break;
			}
			if (temp == 0)
				break;
		}
		if (temp == 0)
			break;
	}
	putchar('\n');

	printf("Matrix\n");
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
			printf("%d ", ppArr[i][j]);
		putchar('\n');
	}
	putchar('\n');

	printf("Memory comparison\n");
	printf("1) Matrix itself representation : %d bytes\n", sizeof(int) * cols * rows);
	printf("2) Non-zero term representation : %d bytes\n", sizeof(element) * terms + sizeof(pElement) + sizeof(rows) + sizeof(cols) + sizeof(terms));


	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			printf("mat[%d][%d]: %d (%x)\n",
				i, j, ppArr[i][j], &ppArr[i][j]);
			cnt++;
			if (cnt == 6)
				break;
		}
		if (cnt == 6)
			break;
	}
	for (i = 0; i < rows; i++)
		free(*(ppArr + i));
	free(ppArr);
	free(pElement);
}
