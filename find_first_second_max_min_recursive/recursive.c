
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// findMax, findMin에 쓰일 매크로 정의
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

// 가장 큰(작은) 수, 그리고 두 번째로 큰(작은) 수를 저장하기 위한 전역배열 arrMm 선언
int arrMm[2] = { 0 };

// 사용자화면을 출력하는 함수
int PrintUI(void);

// 사용자화면을 정리해주는 함수
void clearScreen(int arr[], int n);

// findMax, findMin : 가장 큰, 작은 수를 리턴해주는 함수
int findMax(int arr[], int n);
int findMin(int arr[], int n);

// secMax, secMin : 두번째로 큰, 작은 수를 전역배열 arrMm[1]에 저장하는 함수
int secMax(int arr[], int n);
int secMin(int arr[], int n);

void main()
{
	// main 함수에서 기초가 될 배열의 정보를 입력받는다.
	int arr[100] = { 0 };
	int n;
	int i;
	int nMenu;
	int nMax;
	int nMin;

	clock_t start, finish;
	double duration;
	printf("배열에 몇 개의 데이터를 입력하시겠습니까? ");
	scanf_s("%d", &n);

	printf("%d개의 데이터를 입력해주세요\n", n);
	for (i = 0; i < n; i++)
		scanf_s("%d", &arr[i]);
	putchar('\n');

	// 사용자의 메뉴 선택에 따라 각 메뉴별 실행시간을 측정한다.
	while ((nMenu = PrintUI()) != 0)
	{
		switch (nMenu)
		{
		case 1:
		{
			start = clock();
			clearScreen(arr, n);
			nMax = findMax(arr, n - 1);
			printf("입력된 수 중 가장 큰 값은 %d 입니다.\n", nMax);
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			printf("프로그램 실행시간: %f 초\n\n", duration);
			break;
		}
		case 2:
		{
			start = clock();
			clearScreen(arr, n);
			nMin = findMin(arr, n - 1);
			printf("입력된 수 중 가장 작은 값은 %d 입니다.\n", nMin);
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			printf("프로그램 실행시간: %f 초\n\n", duration);
			break;
		}
		case 3:
		{
			start = clock();
			clearScreen(arr, n);
			secMax(arr, n - 1);
			printf("입력된 수 중 2번째로 큰 값은 %d 입니다.\n", arrMm[1]);
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			printf("프로그램 실행시간: %f 초\n\n", duration);
			break;
		}
		case 4:
		{
			start = clock();
			clearScreen(arr, n);
			secMin(arr, n - 1);
			printf("입력된 수 중 2번째로 작은 값은 %d 입니다.\n", arrMm[1]);
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			printf("프로그램 실행시간: %f 초\n\n", duration);
			break;
		}
		}
	}
	exit(1);
}

int PrintUI(void)
{
	int n = 0;
	printf("원하는 기능에 해당하는 번호를 입력 해주세요.\n");
	printf("1번: 최대값을 구한다.\n");
	printf("2번: 최솟값을 구한다.\n");
	printf("3번: 두 번째로 큰 값을 구한다.\n");
	printf("4번: 두 번째로 작은 값을 구한다.\n");
	printf("0번: 프로그램을 종료합니다.\n");
	scanf_s("%d", &n);
	return n;
}

void clearScreen(int arr[], int n)
{
	int i;
	system("cls");
	printf("\n각 요소에 입력된 데이터를 출력합니다.\n");
	for (i = 0; i < n; i++)
		printf("%d ", arr[i]);
	putchar('\n');
}

int findMax(int arr[], int n)
{
	if (n == 1)
		return MAX(arr[n], arr[n - 1]);
	return MAX(arr[n], findMax(arr, n - 1));
}

int findMin(int arr[], int n)
{
	if (n == 1)
		return MIN(arr[n], arr[n - 1]);
	return MIN(arr[n], findMin(arr, n - 1));
}

// 아래에 보이는 두 함수 secMax 와 secMin 의 가장 큰 특징은, 전역배열을 이용한다는 점이다.
// 순환의 끝지점 (즉, 배열의 인덱스가 0과 1일 때) 에서 해당 요소들을 전역배열 arrMm[0], arrMm[1]에 각각 저장한다.
// arrMm[0]에는 가장 큰(작은) 값이 저장될 것이고, arrMm[1]에는 두번째로 큰(작은) 값이 저장될 것이다.
int secMax(int arr[], int n)
{
	int nIndex;
	if (n == 1)
	{
		if (arr[n] > arr[n - 1])
		{
			arrMm[0] = arr[n];
			arrMm[1] = arr[n - 1];
			return n;
		}
		else
		{
			arrMm[0] = arr[n - 1];
			arrMm[1] = arr[n];
			return n - 1;
		}
	}
	nIndex = secMax(arr, n - 1);
	if (arr[n] > arr[nIndex])
	{
		arrMm[1] = arrMm[0];
		arrMm[0] = arr[n];
		return n;
	}
	else
	{
		if (arrMm[1] < arr[n] && arr[n] != arrMm[0])
			arrMm[1] = arr[n];
		return nIndex;
	}
}

int secMin(int arr[], int n)
{
	int nIndex;
	if (n == 1)
	{
		if (arr[n] < arr[n - 1])
		{
			arrMm[0] = arr[n];
			arrMm[1] = arr[n - 1];
			return n;
		}
		else
		{
			arrMm[0] = arr[n - 1];
			arrMm[1] = arr[n];
			return n - 1;
		}
	}
	nIndex = secMin(arr, n - 1);
	if (arr[n] < arr[nIndex])
	{
		arrMm[1] = arrMm[0];
		arrMm[0] = arr[n];
		return n;
	}
	else
	{
		if (arrMm[1] > arr[n] && arr[n] != arrMm[0])
			arrMm[1] = arr[n];
		return nIndex;
	}
}