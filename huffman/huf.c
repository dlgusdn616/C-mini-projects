/* 1233040 이현우 허프만 과제 프로그램 */
/* 1233040 devMat Huffman program */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ELEMENT 100		// max_element_num which heap could store
#define MAX_HUF 128			// max_huffman_code_num which each terminal_node in huffman_tree could store
#define MAX_TEST_CASE 30	// max_num which you can input character
#define MAX_DECODE_SIZE 250	// 최대 디코드할 수 있는 문자 개수 제한

typedef struct TreeNode {
	char ch;				// non_terminal_node will have 'W' character
	int flag;				// non_terminal_node has 'W' and flag >= 0
	int weight;
	char huf_code[MAX_HUF];	// terminal_node will have huffman_code

	struct TreeNode *left_child;
	struct TreeNode *right_child;
} TreeNode;

typedef struct {
	TreeNode *ptree;
	int key;
} element;

typedef struct {
	element heap[MAX_ELEMENT];
	int heap_size;
} HeapType;

typedef struct {	// this struct will be used to create array that have information related with huffman_code
	int flag;
	char ch;
	char *huffman_code[MAX_HUF];
	int freq;
	int bits;
} HufType;

void init(HeapType *h);										// initiate HeapType heap

void insert_min_heap(HeapType *h, element item);			// insert element in min_heap

element delete_min_heap(HeapType *h);						// delete element from min_heap

TreeNode *make_tree(TreeNode *left, TreeNode *right);		// make TreeNode tree

void destroy_tree(TreeNode *root);							// destroy every node of huffman_tree from root node

void preorder(TreeNode *root);								// preorder traversal of tree
void heap_preorder(HeapType *h, int index);					// preorder traversal of heap which means array because heap is array of Element
// void inorder(TreeNode *root);
// void postorder(TreeNode *root);

void huffman_tree(char ch_arr[], int freq[], int n);		// create huffman_tree & print procedure of making huffman_tree

// after create huffman_tree, you should give every terminal_node the complete code such as "100110"
void create_huf_code(TreeNode *root, char *code_arr, char code);

/*	after give all terminal_node the complete code, print information of terminal_node
*	and store iniformation of terminal_node in huf_arr.
*	to use easily when you calculate frequency or when you incode and decode huffmna_code */
void huffman_code_print(TreeNode *root, HufType *huf_arr);

double logB(double x, double base) { return log(x) / log(base); }// use this function to calculate fixed bits

void huffman_incode(char *code_test, HufType *huf_arr, char *decode, int n);
void huffman_decode(TreeNode *root, char *decode);

int main()
{
	int word_num;		// num of what you want to test this program
	int i, j;
	char ch;
	int f;
	char *ch_arr;		// words you input would be stored in this array
	int *freq;			// frequency you input about each word would be stored in this array

	printf("/*************************** Huffman Program ***************************/\n");
	printf("if there is redundancy in words what you input, you should retry input amount of redundancy\n\n");

	/* entire input procedure */
	printf("please input counts of word you wanna test <= %d: ", MAX_TEST_CASE);
	scanf("%d", &word_num);
	if (word_num > MAX_TEST_CASE)
		exit(1);

	// dynamic memory allocate procedrue
	ch_arr = (char *)malloc(sizeof(char) * word_num);	// each word has frequency information
	freq = (int *)malloc(sizeof(int) * word_num);	

	for (i = 0; i < word_num; i++) {
		getchar();

		printf("input character and frequency : ");
		scanf("%c %d", &ch_arr[i], &freq[i]);

		// check redundancy in what you input
		for (j = 0; j < i; j++) {
			if (ch_arr[i] == ch_arr[j])	// if redundacny occurred
			{
				i--;	// to retry you should decreasae outer for index
				printf("%c alreaddy exists in words what you input. try again\n", ch_arr[i]);
				break;
			}
		}
	}
	/* end input procedure */

	// start huffman_tree algorithm. there is many inner functions in this function. look carefully.
	huffman_tree(ch_arr, freq, word_num);
	
	/* memory release function */
	free(ch_arr);
	free(freq);
	
	printf("input any character and enter too.\n");
	getchar();

	return 0;
}

void init(HeapType *h) {
	h->heap_size = 0;
}

void insert_min_heap(HeapType *h, element item) {
	int i;
	i = ++(h->heap_size);				// index of new node

	// compare procedure wether child_node.key is smaller than parent_node.key
	while ((i != 1) && (item.key < h->heap[i / 2].key)) {

		h->heap[i] = h->heap[i / 2];	// if child_node.keyy is smaller than parent_node.key, change each position
		i /= 2;							// only input parent_node.key in index of child_node.
										// we don't need to input child_node.key in index of parent_node. we gonna do when break this while function.
	}
	h->heap[i] = item;	// input child_node in right position
}

element delete_min_heap(HeapType *h) {
	int parent, child;
	element item, temp;

	item = h->heap[1];	// remember what is the most small value in heap

	// deletion algorithm performs placing the last node in heap to root node place
	// so you should remember what is the last node first and you gonna start compare procedure with each parent_node repeatedly
	// it is a finding procedure that place the last node to proper place in heap
	temp = h->heap[(h->heap_size)--];

	parent = 1;
	child = 2;

	while (child <= h->heap_size) {
		// find which is the smallest node of sibling level
		// this procedure suppose that there is a right child_node, so child must be smaller than heap_size 
		if ((child < h->heap_size) && (h->heap[child].key) > h->heap[child + 1].key)
			child++;	// compare with smaller child_node because this is the min_heap

		// if the temp_node.key is smaller than child_node.key, it meaans that temp_node is in the right place
		// meet a min_heap condition, so break while
		if (temp.key <= h->heap[child].key)	break;

		// 한단계 아래로 이동
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

TreeNode *make_tree(TreeNode *left, TreeNode *right) {	// make tree_node.
	TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
	if (node == NULL) {
		fprintf(stderr, "Memory Allocation Error\n");
		exit(1);
	}
	node->flag = 0;				// only terminal_node has value of flag 0.
	node->huf_code[0] = '\0';	// it would be used in procedure that creates huffman_code, especially when using strcat function

	node->left_child = left;
	node->right_child = right;
	
	return node;
}

void destroy_tree(TreeNode *root) {
	if (root == NULL) return;
	destroy_tree(root->left_child);
	destroy_tree(root->right_child);
	free(root);
}

void huffman_tree(char ch_arr[], int freq[], int n) {	
	// ch_arr: array of what you input, freq: array of frequency, n: size of each array
	int i;
	TreeNode *node, *x;
	HeapType heap;
	element e, e1, e2;

	// using this array when creates huffman_code, it would be accumulated with '0' or '1' iterative in create_huf_code method
	char code_arr[MAX_HUF];	

	// using this array when you created huffman_tree and want to store character, frequency and so on.
	// this would be helpful when incode huffman_tree and decode also.
	// array(n) operation time is less than tree operation(log n)
	HufType huff_arr[MAX_TEST_CASE];

	// used for calculating variable expression storage
	int bit_sum = 0;
	int freq_bits = 0;

	// used for calculating fixed expression storage
	int freq_sum = 0;							
	int fix_bit = 0;	// log_2(word_num)

	char code_test[MAX_TEST_CASE];				// it will be used for incoding operation
	char decode[MAX_DECODE_SIZE];
	

	init(&heap);								// initate heap

	printf("\n\n");
	printf("/*************************** preorder_traversal_heap ***************************/\n\n");

	for (i = 0; i < n; i++) {
		node = make_tree(NULL, NULL);			// create terminal_node
		
		node->ch = ch_arr[i];					// input character information in node
		e.key = node->weight = freq[i];			// innput frequency information
		e.ptree = node;
		
		insert_min_heap(&heap, e, 1);			// insert this terminal_node in min_heap
		heap_preorder(&heap, 1);				// print all nodes in min_heap using preorder traversal 
		putchar('\n');
	}
		
	for (i = 1; i < n; i++) {
		// delete two min_nodes from min_heap
		e1 = delete_min_heap(&heap);
		e2 = delete_min_heap(&heap);

		// sum these two min_nodes and make new_node which called x_node
		x = make_tree(e1.ptree, e2.ptree);
		x->flag = i;							// this node has value bigger than 0 because this node is non_terminal_node
		e.key = x->weight = e1.key + e2.key;	// x_node has value of key which is a sum of two_min_nodes from min_heap
		e.ptree = x;						
		insert_min_heap(&heap, e, 0);			// insert this x_node to min_heap again to make huffman_tree
	}

	e = delete_min_heap(&heap);					// return element that has the root of huffman_tree
	
	code_arr[0] = '\0';									// it would be used create_huf_code method below
	
	// insert parameter e.ptree that must be root_node of huffman_tree, code_arr which would be used to accumulate char information when travel huffman_tree <left: 0, right: 1>
	create_huf_code(e.ptree, code_arr, NULL);


	printf("\n\n");
	printf("/*************************** preorder_traversal_huffman_tree  ***************************/\n\n");
	
	preorder(e.ptree);
	putchar('\n');

	for (i = 0; i < MAX_TEST_CASE; i++) {
		huff_arr[i].flag = 0;							// the reson initiate value 0 to huff_arr is to set
	}

	putchar('\n');
	// 1. print the character what you input and its huffman_code too
	// 2. restore huffman_tree_terminal_node_information in HufType huff_arr to make it easy to calculate below operations
	huffman_code_print(e.ptree, huff_arr);
	printf("\n\n");

	printf("/*********************** print storage needed to fixed expressions | variable expressions ***********************/\n");
	/* calculate each storage fixed or variable */
	for (i = 0; i < MAX_TEST_CASE; i++) {
		if (huff_arr[i].flag == 0)	// it means there is no data in huff_arr[i].
			break;
		// if there is data in huff_arr[i]
		else
		{
			freq_bits = huff_arr[i].freq * huff_arr[i].bits;	// storage that each character use in huffman_algorithm
			bit_sum += freq_bits;								// accumulate storage that each character use in huffman_algoritm
			freq_sum += huff_arr[i].freq;						// total frequency that every character use. it will be used by fixed expression storage
		}
	}
	fix_bit = ((int)logB(n, 2.0)) + 1;			// using for fix_bit calculation, fix_bit = round_up { log_2 (word_num) }

	printf("\nStorage of fixed expressions: %d\n", freq_sum * fix_bit);	// total frequency * fix_bit(=log_2 word_num)
	printf("Storage of variable expressions using huffman: %d\n", bit_sum);

	printf("\n\n");
	printf("/*************************** incode huffman or decode huffman ***************************/\n");

	printf("\ninput characters what you wanna incode : ");
	
	while (getchar() != '\n');	// flush buffer
	gets(code_test);
	decode[0] = '\0';			// to accumulate incoding characters we gonna use strcat so we prepare to use strcat. 

	// incoding result will be stored in char array decode
	huffman_incode(code_test, huff_arr, decode, strlen(code_test));

	putchar('\n');
	printf("incoding result: %s\n\n", decode);
	
	printf("please input the result of incoding: ");
	gets(decode);

	printf("\ndecoding result: ");
	huffman_decode(e.ptree, decode);	
	printf("\n\n");
	// destroy huffman_tree
	destroy_tree(e.ptree);
}

/* print huffman_tree in a way of preorder traversal */
// only print terminal_node information, if there is a flag biggerthan 0 it means non_terminal_node
void preorder(TreeNode *root) {
	if (root) {
		if (root->flag)
			printf("W-%d ", root->flag);
		else
			printf("%c ", root->ch);
		preorder(root->left_child);
		preorder(root->right_child);
	}
}

/* print all elements in heap in a way of preorder traversal */
void heap_preorder(HeapType *h, int index)
{
	if (index > h->heap_size)
		return;
	else
	{
		printf("%c", h->heap[index].ptree->ch);
		heap_preorder(h, index * 2);
		heap_preorder(h, index * 2 + 1);
	}
}

// create huffman_code in a way of going down. this is a recursion function
void create_huf_code(TreeNode *root, char *code_arr, char code) {
	// 1. check the root_node wether is terminal or non_terminal, move only when root_node is terminal_node
	// 1-1. this function is recursion function. so the meaning of root_node changes each time of execution this function
	// 2. if root_node is terminal_node, store huffman_code information in root.huf_code.
	// 2-1. code_arr would provide huffman_code. code_arr would be accumulated with '0' or '1' in each execution.
	// 2-2. <0>: down to left_child_tree_node, <1>: down to right_child_tree_node. both character will be stored in char code, the third parameter of this function

	int i, index;
	index = 0;

	if (code) {	// if there is code from previous execution
		for (i = 0; i < MAX_HUF; i++) {
			// find '\0' first because we would execute strcat function.
			// strcat function needs information of '\0'.
			if (code_arr[i] == '\0')	
			{
				index = i;	// find index where is '\0' in string
				break;
			}
		}
		code_arr[index] = code;	// '\0' is changed to code '0' or '1'
		code_arr[index + 1] = '\0';	// add '\0' because code_arr must be character array
	}

	if (root->flag)	// if there is a flag bigger than 0, it is a non_terminal_node so you keep going down left or right
	{
		create_huf_code(root->left_child, code_arr, '0');	// when you go down left, gives '0' to next execution
		create_huf_code(root->right_child, code_arr, '1');	// when you go down right, gives '1' to next execution
		
		// you already went down left and right, so you should go to parent_node
		// before go to parent_node, you need to add '\0' in code_arr[index] that means reduce one character in string
		code_arr[index] = '\0';
	}
	// if current_node is terminal_node, be ready to store string in code_arr to huf_code in TreeNode struct.
	else
	{
		strcpy(root->huf_code, code_arr);
		// we are now in terminal_node and we finished stroe data in terminal node
		// now we are gonna exit this execution and go to parent_node, so we should put '\0' in current index
		// because going to parent means delete code '0' or '1'. if you couldn't get it, draw huffman_tree and follow incoding procedure using pencil.
		code_arr[index] = '\0';
	}
}

/*	1. create array which have information associated with huffman_code
*	2. print character which means each code what you inputted in first and huffman_code of that code
*	why do <1>?: ar ray would be useful when incode, decode huffman_code or calculate total frequency */
void huffman_code_print(TreeNode *root, HufType *huff_arr) {
	/*	travel each node in a way of inorder traversal LVR, L:left_node, V:root_node, R:right_node
	*	the order of storation dosen't mean nothing in this program so you can use preorder or postorder traversal */
	if (root) {
		huffman_code_print(root->left_child, huff_arr);
		if (root->flag == 0)
		{
			int i, len;
			for (i = 0; i < MAX_TEST_CASE; i++) {
				if (huff_arr[i].flag == 0)	// flag == 0 means huff_arr[i] is enable to store huffman inforamtion
					break;
			}

			/* store procedure */
			huff_arr[i].ch = root->ch;
			strcpy(huff_arr[i].huffman_code, root->huf_code);
			huff_arr[i].bits = strlen(huff_arr[i].huffman_code);
			huff_arr[i].freq = root->weight;
			huff_arr[i].flag = 1;

			/* print procedure */
			printf("%c -> %s\n", root->ch, root->huf_code);
		}

		huffman_code_print(root->right_child, huff_arr);
	}
}

// store incoding information about characters user input to char *decode
void huffman_incode(char *code_test, HufType *huff_arr, char *decode, int n) {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < MAX_TEST_CASE; j++) {
			if (code_test[i] == huff_arr[j].ch)
			{
				strcat(decode, huff_arr[j].huffman_code);
				break;
			}
		}
	}
}

/*	check char array decode in order
*	case '0' down to left of tree
*	case '1' down to right of tree */	
void huffman_decode(TreeNode *root, char *decode) {
	int i;
	TreeNode *head = root;

	for (i = 0; i < MAX_DECODE_SIZE; i++) {
		switch (decode[i]) {
		case '0':	// down to left
			head = head->left_child;
			break;
		case '1':	// down to right
			head = head->right_child;
			break;
		default:
			return;
		}

		// only print the terminal_node
		if (head != NULL && head->flag == 0) {
			putchar(head->ch);
			head = root;
		}
	}
}

//void inorder(TreeNode *root) {
//	if (root) {
//		inorder(root->left_child);
//		if (root->flag)
//			printf("W-%d ", root->flag);
//		else
//			printf("%c ", root->ch);
//		inorder(root->right_child);
//	}
//}
//void heap_inorder(HeapType *h, int index)
//{
//	if (index > h->heap_size)
//		return;
//	else
//	{
//		heap_inorder(h, index * 2);
//		printf("%c", h->heap[index].ptree->ch);
//		heap_inorder(h, index * 2 + 1);
//	}
//}
//
//void postorder(TreeNode *root) {
//	if (root) {
//		postorder(root->left_child);
//		postorder(root->right_child);
//		if (root->flag)
//			printf("W-%d ", root->flag);
//		else
//			printf("%c ", root->ch);
//	}
//}
//void heap_postorder(HeapType *h, int index)
//{
//	if (index > h->heap_size)
//		return;
//	else
//	{
//		heap_postorder(h, index * 2);
//		heap_postorder(h, index * 2 + 1);
//		printf("%c", h->heap[index].ptree->ch);
//	}
//}