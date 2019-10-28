#include "APT_head.h"

void add_APT(Aptlst* lst, Pricetree* ptree, char comm[])
/*The function insert appartment to the appartment list and
to a tree that is sorted according to appartemnt's prices*/
{
	const char q[2] = "\"";
	const char sp[2] = " ";
	const char ent[2] = "\n";
	char* token;
	char* tokencurrtime;
	time_t currtime;
	PriceNode* pnode;
	Apt* newapt;
	Lstnode* lnode;

	newapt = (Apt*)malloc(sizeof(Apt) * 1);

	// copy address
	token = strtok(comm, q);
	newapt->address = (char*)malloc(sizeof(char)*(strlen(token) + 1));
	strcpy(newapt->address, token);

	//price
	token = strtok(NULL, sp);
	newapt->price = charToInt(token);

	//num of rooms
	token = strtok(NULL, sp);
	newapt->rooms_num = (short int)charToInt(token);

	//date of entrance
	//day
	token = strtok(NULL, sp);
	newapt->dateOfEnter.day = (short int)charToInt(token);
	//month
	token = strtok(NULL, sp);
	newapt->dateOfEnter.month = (short int)charToInt(token);
	//year
	token = strtok(NULL, sp);
	newapt->dateOfEnter.year = (short int)charToInt(token) + 2000;

	//date entrance to pool
	time(&currtime);
	//month
	tokencurrtime = strtok(ctime(&currtime), sp);
	tokencurrtime = strtok(NULL, sp);
	newapt->dateToPool.month = monthToNum(tokencurrtime);
	//day
	tokencurrtime = strtok(NULL, sp);
	newapt->dateToPool.day = (short int)charToInt(tokencurrtime);
	//year
	tokencurrtime = strtok(NULL, sp);
	tokencurrtime = strtok(NULL, ent);
	newapt->dateToPool.year = (short int)charToInt(tokencurrtime);

	//code
	//newapt->code = code;
	if (lst->head == NULL)
	{
		newapt->code = 1;
	}
	else
	{
		newapt->code = lst->tail->aptn->code + 1;
	}

	lnode = createlstnode(newapt);

	//inserting to list of apt
	if (lst->head == NULL)//if the list is empty
	{
		lst->head = lnode;
		lst->tail = lnode;
	}
	else
	{
		lnode->prev = lst->tail;
		lst->tail->next = lnode;
		lst->tail = lnode;
	}

	//updating price tree
	pnode = CreatePriceNode(newapt->price, newapt, newapt->code);
	Insert(ptree, pnode);
}

Lstnode* createlstnode(Apt* node)
/*the function creates list node for the list of apt*/
{
	Lstnode* newnode = (Lstnode*)malloc(sizeof(Lstnode) * 1);
	newnode->aptn = node;
	newnode->next = NULL;
	newnode->prev = NULL;

	return newnode;
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

short int monthToNum(char* month)
/*The function gets a month and return its number*/
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

PriceNode* CreatePriceNode(int price, Apt* add, int code)
/*The function creates a price node*/
{
	PriceNode* newnode = (PriceNode*)malloc(sizeof(PriceNode) * 1);
	newnode->aptadd = add;
	newnode->price = price;
	newnode->code = code;
	newnode->right = NULL;
	newnode->left = NULL;

	return newnode;
}

void buy_APT(Aptlst* lst, Pricetree* pt, char comm[])
/*The function is taking care of the command buy-an-apt
the function gets a code and delete the apt with that code from
the list and the tree*/
{
	int code = charToInt(comm);
	Lstnode* curr = lst->head;
	int found = 0;
	int price;


	//delete from apt list
	while (curr != NULL && found == 0)
	{
		if (curr->aptn->code == code)
		{
			price = curr->aptn->price;
			if (curr == lst->head)// if the node is the head
			{
				if (curr->next == NULL)//if the node is the only one in the list
				{
					lst->head = NULL; //new
					lst->tail = NULL; // new
				}
				else
				{
					lst->head = curr->next;
					curr->next->prev = NULL;
				}
				free(curr->aptn->address);
				free(curr->aptn);
				free(curr);
				found = 1;
			}
			else if (curr == lst->tail)// if the node is the tail
			{
				lst->tail = curr->prev;
				lst->tail->next = NULL;
				free(curr->aptn->address);
				free(curr->aptn);
				free(curr);
				found = 1;
			}
			else// the node is in the middle of the list 
			{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
				free(curr->aptn->address);
				free(curr->aptn);
				free(curr);
				found = 1;
			}
		}

		curr = curr->next;
	}

	//delete from price tree
	DeleteFromTree(pt, price, code);
}


void Insert(Pricetree* pt, PriceNode* node)
/*The function insert node of price to the price tree sorted*/
{
	PriceNode* father;

	if (pt->root == NULL)
		pt->root = node;
	else
	{
		father = FindPlace(pt->root, node->price);
		if (father->price > node->price)
			father->left = node;
		else
			father->right = node;
	}
}

PriceNode* FindPlace(PriceNode* t, int val)
/* Returning the parent of the new val needed to be inserted */
{
	if (t->price > val)
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

void DeleteFromTree(Pricetree* pt, int price, int code)
/*The function delete a node from the price tree*/
{
	pt->root = deleteNode(pt->root, price, code);
}

PriceNode* deleteNode(PriceNode* currentNode, int price, int code)
/*Help function*/
{
	PriceNode* tmp;
	if (currentNode == NULL) // empty tree
		return NULL;
	else if (price < currentNode->price) // price is less than node's price. so go to left subtree
	{
		currentNode->left = deleteNode(currentNode->left, price, code);
	}
	else if (price > currentNode->price) // price is greater than node's price. so go to right subtree
		currentNode->right = deleteNode(currentNode->right, price, code);
	else if (code != currentNode->code)
		currentNode->right = deleteNode(currentNode->right, price, code);
	else // node found. Let's delete it!
	{
		//node has no child
		if (currentNode->left == NULL && currentNode->right == NULL)
		{
			//printf("recursion");
			//free(currentNode);
			tmp = currentNode;
			currentNode = NULL;
			free(tmp);

		}
		else if (currentNode->left == NULL) // node has only right child
		{
			//free(currentNode);
			tmp = currentNode;
			currentNode = currentNode->right;
			free(tmp);
		}
		else if (currentNode->right == NULL) // node has only left child
		{
			//free(currentNode);
			tmp = currentNode;
			currentNode = currentNode->left;
			free(tmp);
		}
		else // node has two children
		{
			PriceNode* tempNode = findMinimum(currentNode->right);
			currentNode->price = tempNode->price;
			currentNode->code = tempNode->code;
			currentNode->aptadd = tempNode->aptadd;
			currentNode->right = deleteNode(currentNode->right, tempNode->price, tempNode->code);
		}

	}

	return currentNode;
}

PriceNode* findMinimum(PriceNode *currentNode)
/*The function finds the first node that has null to the left*/
{
	if (currentNode->left == NULL)
		return currentNode;

	return findMinimum(currentNode->left);
}

Date findDateBefore1(Date d)
/*The function gets a date and return a date before it by 1 day*/
{
	if (d.day != 1)
	{
		d.day--;

	}
	else//start of a month
	{
		switch (d.month)
		{
		case 1:
		{
			d.year--;
			d.day = 31;
			d.month = 12;
			break;
		}
		case 2:
		{
			d.day = 31;
			d.month--;
			break;
		}
		case 3:
		{
			if (isleap(d.year) == 1)
				d.day = 29;
			else
				d.day = 28;
			d.month--;
			break;

		}
		case 4:
		{
			d.day = 31;
			d.month--;
			break;
		}
		case 5:
		{
			d.day = 30;
			d.month--;
			break;
		}
		case 6:
		{
			d.day = 31;
			d.month--;
			break;
		}
		case 7:
		{
			d.day = 30;
			d.month--;
			break;
		}
		case 8:
		{
			d.day = 31;
			d.month--;
			break;
		}
		case 9:
		{
			d.day = 31;
			d.month--;
			break;
		}
		case 10:
		{
			d.day = 30;
			d.month--;
			break;
		}
		case 11:
		{
			d.day = 31;
			d.month--;
			break;
		}
		case 12:
		{
			d.day = 30;
			d.month--;
			break;
		}
		}
	}

	return d;

}

int isleap(int year)
/*The function gets a year and return 1 if it is a leap year else 0*/
{
	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void delete_APT(Aptlst* lst, Pricetree* pt, char comm[])
/*The function take care of the command delete-an-apt
The function deletes all the apts that were entered to the pool in the last num days
comm =-Enter <num>*/
{
	char* token;
	char sp[2] = " ";
	char ent[2] = "\n";
	int numofdays;
	Date cdate;
	time_t currtime;
	char* tokencurrtime;
	int i;
	Lstnode* curr = lst->tail;

	//extract from the command the num of days 
	token = strtok(comm, sp);
	token = strtok(NULL, sp);
	numofdays = charToInt(token);

	//extract the current date to cdate
	time(&currtime);
	//month
	tokencurrtime = strtok(ctime(&currtime), sp);
	tokencurrtime = strtok(NULL, sp);
	cdate.month = monthToNum(tokencurrtime);
	//day
	tokencurrtime = strtok(NULL, sp);
	cdate.day = (short int)charToInt(tokencurrtime);
	//year
	tokencurrtime = strtok(NULL, sp);
	tokencurrtime = strtok(NULL, ent);
	cdate.year = (short int)charToInt(tokencurrtime);

	for (i = 0; curr != NULL && i < numofdays; i++)
	{
		while (curr != NULL && curr->aptn->dateToPool.day == cdate.day
			&& curr->aptn->dateToPool.month == cdate.month
			&& curr->aptn->dateToPool.year == cdate.year)
		{
			//delete from tree
			DeleteFromTree(pt, curr->aptn->price, curr->aptn->code);

			//delete the node
			if (curr = lst->head)
			{
				free(curr->aptn->address);
				free(curr->aptn);
				free(curr);

				lst->head = NULL;
				lst->tail = NULL;

			}
			else
			{
				lst->tail = curr->prev;
				lst->tail->next = NULL;
				free(curr->aptn->address);
				free(curr->aptn);
				free(curr);
			}

			curr = lst->tail;
		}

		//updating c date to previous date
		cdate = findDateBefore1(cdate);
	}

}


void get_an_APT(Aptlst* lst, Pricetree* pt, char comm[])
/*The function takes care of the command get-an-apt
The function prints all apt that matches the user's requirment*/
{
	int arr_comm[7] = { -1,-1,-1,-1,-1,-1,-1 };
	char* token;
	char sp[2] = " ";
	int isnum = 0;
	int index;
	Date date;

	//set the date to hrepsh
	date.day = -1;
	date.month = -1;
	date.year = -1;

	//extract the first word in the command string 
	token = strtok(comm, sp);

	while (token != NULL)
	{
		if (isnum % 2 == 0) // if the string is a word 
		{
			index = get_comm_index(token);
			if (index == 4)// if it is s
			{
				arr_comm[index] = 1;
			}
			else if (index == 5)// if it is sr
			{
				arr_comm[index] = 1;
			}
			else
			{
				isnum++;
			}
		}
		else //if the string is the num
		{

			arr_comm[index] = charToInt(token);
			isnum++;
		}
		token = strtok(NULL, sp);
	}

	if (arr_comm[3] != -1)
		//if the date is in the command it we will create a date struct
	{
		date.year = arr_comm[3] % 10000;
		arr_comm[3] = arr_comm[3] / 10000;
		date.month = arr_comm[3] % 100;
		arr_comm[3] = arr_comm[3] / 100;
		date.day = arr_comm[3];
	}

	//print the apt
	if (arr_comm[6] != -1)
	{
		print_apt_ENTER_lst(lst, arr_comm, date);
	}
	else if (arr_comm[4] == -1 && arr_comm[5] == -1)// no s or sr
	{
		print_apt_lst(lst, arr_comm, date);
	}
	else if (arr_comm[4] != -1)// s
	{
		print_apt_tree_s(pt, arr_comm, date);
	}
	else if (arr_comm[5] != -1)//sr
	{
		print_apt_tree_sr(pt, arr_comm, date);
	}

}

int get_comm_index(char* comm)
/*The function gets a command and returns the index of it in the arr_comm*/
{
	if (strcmp(comm, "-MaximumPrice") == 0)
	{
		return 0;
	}
	else if (strcmp(comm, "-MinimumNumRooms") == 0)
	{
		return 1;
	}
	else if (strcmp(comm, "-MaximumNumRooms") == 0)
	{
		return 2;
	}
	else if (strcmp(comm, "-Date") == 0)
	{
		return 3;
	}
	else if (strcmp(comm, "-s") == 0)
	{
		return 4;
	}
	else if (strcmp(comm, "-sr") == 0)
	{
		return 5;
	}
	else if (strcmp(comm, "-Enter") == 0)
	{
		return 6;
	}
	else
	{
		return -1;
	}
}

int istobe_Printed(Apt* apt, int arrcomm[], Date date)
/*The function gets and apt and returns 1 if the apt matches the user's requirement*/
{
	int isgood = 1;
	Date cdate;
	time_t currtime;
	char* tokencurrtime;
	int i;
	char sp[2] = " ";
	char ent[2] = "\n";

	//extract the current date to cdate
	time(&currtime);
	//month
	tokencurrtime = strtok(ctime(&currtime), sp);
	tokencurrtime = strtok(NULL, sp);
	cdate.month = monthToNum(tokencurrtime);
	//day
	tokencurrtime = strtok(NULL, sp);
	cdate.day = (short int)charToInt(tokencurrtime);
	//year
	tokencurrtime = strtok(NULL, sp);
	tokencurrtime = strtok(NULL, ent);
	cdate.year = (short int)charToInt(tokencurrtime);

	if (isgood == 1 && arrcomm[0] != -1)
	{
		if (apt->price > arrcomm[0])
			isgood = 0;
	}
	if (isgood == 1 && arrcomm[1] != -1)
	{
		if (apt->rooms_num < arrcomm[1])
			isgood = 0;
	}
	if (isgood == 1 && arrcomm[2] != -1)
	{
		if (apt->rooms_num > arrcomm[2])
			isgood = 0;
	}
	if (isgood == 1 && date.day != -1)
	{
		if (is_date1_before_date2(apt->dateOfEnter, date) == 0)
		{
			isgood = 0;
		}
	}
	if (isgood == 1 && arrcomm[6] != -1)
	{
		for (i = 0; i < arrcomm[6] - 1; i++)
		{
			cdate = findDateBefore1(cdate);
		}

		if (is_date1_before_date2(cdate, apt->dateToPool) == 0)
		{
			isgood = 0;
		}
	}
	return isgood;
}

int is_date1_before_date2(Date date1, Date date2)
/*The function check id date1 is before date2 if yes it return 1 else it return 0*/
{
	//int isbefore = 1;
	if (date1.year < date2.year)
		return 1;
	else if (date1.year > date2.year)
		return 0;
	else // if same year
	{
		if (date1.month < date2.month)
			return 1;
		else if (date1.month > date2.month)
			return 0;
		else //if same month
		{
			if (date1.day < date2.day)
				return 1;
			else if (date1.day > date2.day)
				return 0;
			else
				return 1;
		}
	}

}

void print_apt_lst(Aptlst* lst, int arrcomm[], Date date)
/*The function print apts that matches the user's requiements from the list */
{
	Lstnode* curr = lst->head;

	while (curr != NULL)
	{
		if (istobe_Printed(curr->aptn, arrcomm, date) == 1) //if the apt need to be printed
		{
			print_apt(curr->aptn);
		}

		curr = curr->next;
	}
}

void print_apt_tree_sr(Pricetree* pt, int arrcomm[], Date date)
/*The function print apts that matches the user's requiements from the tree in a sorted way
From big to small*/
{
	print_apt_tree_sr_rec(pt->root, arrcomm, date);
}
void print_apt_tree_sr_rec(PriceNode* root, int arrcomm[], Date date)
/*Help function SR*/
{
	if (root != NULL)
	{
		print_apt_tree_sr_rec(root->right, arrcomm, date);
		if (istobe_Printed(root->aptadd, arrcomm, date) == 1)
		{
			print_apt(root->aptadd);
		}
		print_apt_tree_sr_rec(root->left, arrcomm, date);
	}
}

void print_apt_tree_s(Pricetree* pt, int arrcomm[], Date date)
/*The function print apts that matches the user's requiements from the tree in a sorted way
From small to big*/
{
	print_apt_tree_s_rec(pt->root, arrcomm, date);
}
void print_apt_tree_s_rec(PriceNode* root, int arrcomm[], Date date)
/*HELP function S*/
{
	if (root != NULL)
	{
		print_apt_tree_s_rec(root->left, arrcomm, date);
		if (istobe_Printed(root->aptadd, arrcomm, date) == 1)
		{
			print_apt(root->aptadd);
		}
		print_apt_tree_s_rec(root->right, arrcomm, date);
	}
}

void print_apt(Apt* aptn)
/*The function prints all details of an apt*/
{
	printf("Apt details:\n");
	printf("Code: %d\n", aptn->code);
	printf("Address: %s\n", aptn->address);
	printf("Number of rooms: %d\n", aptn->rooms_num);
	printf("Price: %d\n", aptn->price);
	printf("Entry date: %d.%d.%d\n", aptn->dateOfEnter.day, aptn->dateOfEnter.month, aptn->dateOfEnter.year);
	printf("Database entry date: %d.%d.%d\n\n", aptn->dateToPool.day, aptn->dateToPool.month, aptn->dateToPool.year);

}

void print_apt_ENTER_lst(Aptlst* lst, int arrcomm[], Date date)
/*The function prints apts from the list that were inserted to the pools
by X days*/
{
	Lstnode* curr = lst->head;

	while (curr != NULL)
	{
		if (istobe_Printed(curr->aptn, arrcomm, date) == 1) //if the apt need to be printed
		{
			print_apt_code(curr->aptn);
		}

		curr = curr->next;
	}
}

void print_apt_code(Apt* aptn)
/*The function prints a code of an apt*/
{
	printf("Apt code:\n");
	printf("Code: %d\n\n", aptn->code);
}

void freeaptlist(Aptlst* lst)
/*free apt list*/
{
	Lstnode* curr = lst->head;
	Lstnode* tmp;

	while (curr != NULL)
	{
		tmp = curr->next;
		free(curr->aptn->address);
		free(curr->aptn);
		free(curr);
		curr = tmp;
	}
}

void freepricetree(Pricetree* pt)
/*free price tree*/
{
	freepricetreerec(pt->root);

}

void freepricetreerec(PriceNode* root)
/*Help function*/
{
	if (root != NULL)
	{
		freepricetreerec(root->left);
		freepricetreerec(root->right);
		free(root);
	}
}

void aptTofile(Aptlst* lst)
/*The function put all the apt details into binary file*/
{
	FILE* file = fopen("apt.bin", "wb");
	Lstnode* curr = lst->head;
	short int sizeofstr;
	BYTE left = 0x00, middle = 0x00, right = 0x00;
	//BYTE mask = 0x01;
	BYTE lpools = 0x00, rpools = 0x00;

	while (curr != NULL)
	{
		//code
		short int code = (short int)curr->aptn->code;
		fwrite(&(code), sizeof(short int), 1, file);

		//size of address
		sizeofstr = (short int)strlen(curr->aptn->address);
		fwrite(&sizeofstr, sizeof(short int), 1, file);

		//address
		fwrite(curr->aptn->address, sizeof(char), sizeofstr, file);

		//price
		fwrite(&(curr->aptn->price), sizeof(int), 1, file);

		//num of rooms and date of entrance 
		left = left | (curr->aptn->rooms_num << 4);
		left = left | (curr->aptn->dateOfEnter.day >> 1);
		middle = (curr->aptn->dateOfEnter.day << 7);
		middle = middle | (curr->aptn->dateOfEnter.month << 3);
		middle = middle | ((curr->aptn->dateOfEnter.year-2000) >> 4);
		right = (curr->aptn->dateOfEnter.year << 4);
		fwrite(&left, sizeof(BYTE), 1, file);
		fwrite(&middle, sizeof(BYTE), 1, file);
		fwrite(&right, sizeof(BYTE), 1, file);

		//date of pools
		lpools = lpools | (curr->aptn->dateToPool.day << 3);
		lpools = lpools | (curr->aptn->dateToPool.month >> 1);
		rpools = rpools | (curr->aptn->dateToPool.month << 7);
		rpools = rpools | (curr->aptn->dateToPool.year-2000);
		fwrite(&lpools, sizeof(BYTE), 1, file);
		fwrite(&rpools, sizeof(BYTE), 1, file);

		curr = curr->next;
		left = 0x00;
		middle = 0x00;
		right = 0x00;
		lpools = 0x00;
		rpools = 0x00;

	}

	fclose(file);
}

void fileAptToPools(Aptlst* lst, Pricetree* pt)
/*The function extract apt details from the binary file and enter it to pools*/
{
	FILE* file = fopen("apt.bin", "rb");
	PriceNode* pnode;
	Apt* newapt;
	Lstnode* lnode;
	short int sizeofadd;
	BYTE left, middle, right;
	BYTE mask = 0x0F;
	BYTE mask2 = 0x07;
	BYTE mask3 = 0x7F;
	BYTE lpools, rpools;
	long tell = ftell(file);
	while (tell < binfileSize(file))
	{
		//fseek(file, tell, SEEK_SET);
		newapt = createAptNodefromFile(file);
		lnode = createlstnode(newapt);

		//inserting to list of apt
		if (lst->head == NULL)//if the list is empty
		{
			lst->head = lnode;
			lst->tail = lnode;
		}
		else
		{
			lnode->prev = lst->tail;
			lst->tail->next = lnode;
			lst->tail = lnode;
		}

		//updating price tree
		pnode = CreatePriceNode(newapt->price, newapt, newapt->code);
		Insert(pt, pnode);
		tell = ftell(file);
	}

	fclose(file);
}

Apt* createAptNodefromFile(FILE* file)
/*The function creates a node of apt from the binary file*/
{
	Apt* newapt = (Apt*)malloc(sizeof(Apt) * 1);
	short int sizeofadd;
	BYTE left, middle, right;
	BYTE mask = 0x0F;
	BYTE mask2 = 0x07;
	BYTE mask3 = 0x7F;
	BYTE lpools, rpools;

	//code
	short int code;
	fread(&(code), sizeof(short int), 1, file);
	newapt->code = (int)code;

	//address
	fread(&sizeofadd, sizeof(short int), 1, file);
	newapt->address = (char*)malloc(sizeof(char)*(sizeofadd + 1));
	fread(newapt->address, sizeof(char), sizeofadd, file);
	newapt->address[sizeofadd] = '\0';

	//price
	fread(&(newapt->price), sizeof(int), 1, file);

	// num of rooms and date of entrance 
	fread(&left, sizeof(BYTE), 1, file);
	fread(&middle, sizeof(BYTE), 1, file);
	fread(&right, sizeof(BYTE), 1, file);
	newapt->rooms_num = left >> 4;
	left = left & mask;
	left = left << 1;
	left = left | (middle >> 7);
	newapt->dateOfEnter.day = left;
	newapt->dateOfEnter.month = ((middle >> 3)&mask);
	newapt->dateOfEnter.year = (((middle & mask2) << 4) | (right >> 4)) + 2000;

	//date to pools
	fread(&lpools, sizeof(BYTE), 1, file);
	fread(&rpools, sizeof(BYTE), 1, file);
	newapt->dateToPool.day = lpools >> 3;
	newapt->dateToPool.month = (((lpools&mask2) << 1) | (rpools >> 7));
	newapt->dateToPool.year = (rpools&mask3)+ 2000;

	return newapt;

}

long int binfileSize(FILE* file)
{/*returns the size of the file*/
	long int res;
	long int curr = ftell(file);
	fseek(file, 0, SEEK_END);
	res = ftell(file);
	fseek(file, curr, SEEK_SET);

	return res;
}

int binexists(const char *fname)
/*The function check if a file exists*/
{
	FILE *file;
	if ((file = fopen(fname, "rb")))
	{
		fclose(file);
		return 1;
	}
	return 0;
}