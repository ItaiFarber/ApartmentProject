#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <time.h>
typedef struct date {
	short int day;
	short int month;
	short int year;
}Date;

typedef struct apt {

	int code;
	char* address;
	int price;
	short int rooms_num;
	Date dateOfEnter;
	Date dateToPool;
}Apt;

typedef struct pricenode {
	Apt* aptadd;
	int price;
	struct pricenode* right;
	struct pricenode* left;
}PriceNode;

typedef struct pricetree {
	PriceNode* root;
}Pricetree;
char* get_comm_from_user(int* lsize, int* psize);
void insertToArray(char** arr);

//void insertNodeToPriceTreeRec(PriceNode* root, PriceNode* node);
void Insert(Pricetree* pt, PriceNode* node);
PriceNode* FindPlace(PriceNode* t, int val);
int monthToNum(char* month);
void DeleteFromTree(Pricetree* pt, int price);
//"Hasavyonim 23 Ramat Ono" 2000000 4 01 01 20
PriceNode* deleteNode(PriceNode* currentNode, int price);
void DeleteFromTree(Pricetree* pt, int price);
void PrintInOrderRec(PriceNode *t);
void PrintInOrder(Pricetree* t);
PriceNode* findMinimum(PriceNode *currentNode);
char* getSafta(char* sft);
int charToInt(char* num);

void main()
{

	char* arr[7];
	insertToArray(arr);
	
	char* safta;
	safta = getSafta(arr[2]);
	arr[2] = (char*)malloc(20);
	char* str = "imashazona";
	strcpy(arr[2], str);
	printf("%s\n", arr[2]);
	printf("safta: %s\n", safta);



	

	

}
char* getSafta(char* sft)
{
	char* safta = sft;
	return safta;
}
void insertToArray(char** arr)
{
	for (int i = 0; i < 7; i++)
	{
		arr[i] = (char*)malloc(20);
		arr[i] = "NAHUM";
	}
}

char* get_comm_from_user(int* lsize, int* psize)
{
	char* temp = (char*)malloc(psize);
	char ch;
	int i = 0;

	scanf("%c", &ch);
	while (ch != '\n')
	{
		if (*lsize < *psize - 1)//if there is enough space in the string
		{
			temp[i] = ch;
			(*lsize) = *lsize + 1;
		}
		else
		{
			(*psize) = (*psize) * 2;
			temp = realloc(temp, sizeof(char)*(*psize));
			temp[i] = ch;
			(*lsize) = *lsize + 1;
		}
		scanf("%c", &ch);
		i++;
	}

	temp[i] = '\0';
	(*lsize) = *lsize + 1;
	temp = realloc(temp, sizeof(char)*(*lsize));
	return temp;
}
int monthToNum(char* month)
{
	if (strcmp(month, "Jan") == 0)
		return 1;
	if (strcmp(month, "Feb") == 0)
		return 2;
	if (strcmp(month, "Mar") == 0)
		return 3;
	if (strcmp(month, "Apr") == 0)
		return 4;
	if (strcmp(month, "May") == 0)
		return 5;
	if (strcmp(month, "Jun") == 0)
		return 6;
	if (strcmp(month, "Jul") == 0)
		return 7;
	if (strcmp(month, "Aug") == 0)
		return 8;
	if (strcmp(month, "Sep") == 0)
		return 9;
	if (strcmp(month, "Oct") == 0)
		return 10;
	if (strcmp(month, "Nov") == 0)
		return 11;
	if (strcmp(month, "Dec") == 0)
		return 12;
	return -1;
}

void Insert(Pricetree* pt, PriceNode* node)
{
	PriceNode* father;
	
	if (pt->root == NULL)
		pt->root = node;
	else
	{
		father = FindPlace(pt->root, node->price);
		if (father->price >= node->price)
			father->left = node;
		else
			father->right = node;
	}
}

/* Returning the parent of the new val needed to be inserted */
PriceNode* FindPlace(PriceNode* t , int val)
{
	if (t->price >= val)
	{
		if (t->left == NULL)
			return t;
		else
			return FindPlace(t->left, val);
	}
	else
	{
		if (t->right == NULL)
			return t;
		else
			return FindPlace(t->right, val);
	}
}
void DeleteFromTree(Pricetree* pt, int price)
{
	pt->root = deleteNode(pt->root, price);
}
PriceNode* deleteNode(PriceNode* currentNode, int price)
{
	if (currentNode == NULL) // empty tree
		return NULL;
	else if (price < currentNode->price) // price is less than node's price. so go to left subtree
		currentNode->left = deleteNode(currentNode->left, price);
	else if (price > currentNode->price) // price is greater than node's price. so go to right subtree
		currentNode->right = deleteNode(currentNode->right, price);
	else // node found. Let's delete it!
	{
		//node has no child
		if (currentNode->left == NULL && currentNode->right == NULL)
		{
			free(currentNode);
			currentNode = NULL;
			
		}
		else if (currentNode->left == NULL) // node has only right child
		{
			free(currentNode);
			currentNode = currentNode->right;
		}
		else if (currentNode->right == NULL) // node has only left child
		{
			free(currentNode);
			currentNode = currentNode->left;
		}
		else // node has two children
		{
			PriceNode* tempNode = findMinimum(currentNode->right);
			
			currentNode->price = tempNode->price;
			currentNode->right = deleteNode(currentNode->right, tempNode->price);
			
		}

	}

	return currentNode;
}

PriceNode* findMinimum(PriceNode *currentNode)
{
	if (currentNode->left == NULL)
		return currentNode;

	return findMinimum(currentNode->left);
}

void PrintInOrder(Pricetree* t)
{
	PrintInOrderRec(t->root);
}

void PrintInOrderRec(PriceNode *t)
{
	if (t != NULL)
	{
		PrintInOrderRec(t->left);
		printf("%d ", t->price);
		PrintInOrderRec(t->right);
	}
}

int charToInt(char* num)
/*The function gets a string that which is a number and
returns an integer*/
{
	int number = 0;
	int i = 0;

	while (num[i] != '\0')
	{
		number = number * 10 + (num[i] - '0');
		i++;
	}
	return number;
}