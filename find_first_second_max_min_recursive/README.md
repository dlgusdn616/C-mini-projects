# Recursive Programming - min, max

## 1. 개요

사용자로부터 정수로 이루어진 배열과 배열의 크기를 입력 받았을 때, 배열 내에서 최대값과 최소값 혹은 두번째로 큰 최대값, 두번째로 작은 최소값을 반환해주는 프로그램이다.

## 2. 조작법

사용자는 UI에서 자신이 원하는 기능에 따라 번호를 입력하면 된다. 

1. 최대값을 구하는 동작
2. 최소값을 구하는 동작
3. 두번째로 큰 최대값을 구하는 동작
4. 두번째로 작은 최대값을 구하는 동작

사용자가 입력한 각 기능을 수행하면서 측정된 수행 시간 또한 출력된다.

```c
start = clock();
/* execute operation */
finish = clock();
duration = (double)(finish - start) / CLOCKS_PER_SEC;
```

## 3. 로직

### 3.1 최대값, 최소값을 구하는 로직

```c
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
```

### 3.2 두번째로 큰 최대값, 두번째로 작은 최소값을 구하는 로직

```c
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
```

