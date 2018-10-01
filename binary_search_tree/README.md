# binary search tree implementation

이진트리 자료구조를 구현하였고 본 프로그램은 다음과 같은 기능이 존재한다. 중요하다고 생각하는 기능에 한정하여 설명과 함께 코드를 덧붙였다. 중간 중간 동작하는 사진도 함께 포함되어 있다.

1. insert_node: 특정 키 값을 갖는 노드를 이진트리 자료구조에 삽입한다.

2. delete_node: 특정 키 값을 갖는 노드를 이진트리 자료구조에서 삭제한다.

3. preorder: 이진트리를 전위순회한 결과를 출력한다. VLR
   ![preorder](images/1-preorder.png)

4. inorder: LVR 순회한 결과를 출력한다.
   ![inorder](images/2-inorder.png)

5. postorder: 이진트리를 후위순회한 결과를 출력한다.
   ![postorder](images/3-postorder.png)

6. search_node: 트리에서 특정 키 값이 존재하는지 찾고 있다면 그 노드까지 도달하기 위한 path를 출력한다.
   ![search_node](images/4-search_node.png)

7. tree_height: 트리의 높이를 반환한다.

8. node_count: 트리에 존재하는 노드의 개수를 반환한다.

9. delete_tree: 프로그램을 종료하기 전 동적할당으로 생성한 모든 노드 메모리를 해제한다.

10. min_key_value: 트리에 가장 작은 키 값을 찾는다.

11. max_key_value:트리에서 가장 큰 키 값을 찾는다.

12. level_find: 특정 키 값이 속해 있는 레벨을 출력한다. (레벨 순회 구현)

    ```c
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
    ```

13. level_print: 입력한 레벨에 해당하는 노드를 출력한다. 

    ```c
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
    ```

사용된 자료구조는 다음과 같다.

* Queue: 특정 키 값이 속해 있는 노드의 레벨(트리 높이)을 출력해야하는 기능 때문에 Queue 자료구조가 사용되었다. 관련 내용은 `레벨 순회` 라는 키워드로 검색하면 구글에서 확인할 수 있다.

```c
typedef struct _Queue {
	tNode *queue[10000];
	int front;
	int rear;
} Queue;
```



* 양 방향 노드: 좌측 자식노드, 우측 자식노드를 가리키는 포인터 2개와 키값을 갖는다.

```c
typedef struct _tNode {				// 트리의 노드 구조체
	element data;					// 노드의 키 값
	struct _tNode *left, *right;
} tNode;

```

