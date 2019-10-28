#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef unsigned char BYTE;

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
	int code;
	struct pricenode* right;
	struct pricenode* left;
}PriceNode;

typedef struct pricetree {
	PriceNode* root;
}Pricetree;

typedef struct lstnode {

	struct lstnode* next;
	struct lstnode* prev;
	Apt* aptn;
}Lstnode;

typedef struct aptlst {

	Lstnode* head;
	Lstnode* tail;

}Aptlst;



void add_APT(Aptlst* lst, Pricetree* ptree, char comm[]);
Lstnode* createlstnode(Apt* node);
int charToInt(char* num);
short int monthToNum(char* month);
PriceNode* CreatePriceNode(int price, Apt* add, int code);
void buy_APT(Aptlst* lst, Pricetree* pt, char comm[]);
void Insert(Pricetree* pt, PriceNode* node);
PriceNode* FindPlace(PriceNode* t, int val);
void DeleteFromTree(Pricetree* pt, int price, int code);
PriceNode* deleteNode(PriceNode* currentNode, int price, int code);
PriceNode* findMinimum(PriceNode *currentNode);
Date findDateBefore1(Date d);
int isleap(int year);
void delete_APT(Aptlst* lst, Pricetree* pt, char comm[]);
void get_an_APT(Aptlst* lst, Pricetree* pt, char comm[]);
int get_comm_index(char* comm);
int istobe_Printed(Apt* apt, int arrcomm[], Date date);
int is_date1_before_date2(Date date1, Date date2);
void print_apt_lst(Aptlst* lst, int arrcomm[], Date date);
void print_apt_tree_sr(Pricetree* pt, int arrcomm[], Date date);
void print_apt_tree_sr_rec(PriceNode* root, int arrcomm[], Date date);
void print_apt_tree_s(Pricetree* pt, int arrcomm[], Date date);
void print_apt_tree_s_rec(PriceNode* root, int arrcomm[], Date date);
void print_apt(Apt* aptn);
void print_apt_ENTER_lst(Aptlst* lst, int arrcomm[], Date date);
void print_apt_code(Apt* aptn);
void freeaptlist(Aptlst* lst);
void freepricetree(Pricetree* pt);
void freepricetreerec(PriceNode* root);
void aptTofile(Aptlst* lst);
void fileAptToPools(Aptlst* lst, Pricetree* pt);
long int binfileSize(FILE* file);
Apt* createAptNodefromFile(FILE* file);
int binexists(const char *fname);







