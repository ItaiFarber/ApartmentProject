#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "APT_head.h"

#define N 7

typedef struct lstnodecomm {

	struct lstnodecomm* next;
	struct lstnodecomm* prev;
	char* comm;
	int commindex;
}Lstnodecomm;

typedef struct commlst {

	Lstnodecomm* head;
	Lstnodecomm* tail;

}Commlst;

char* get_comm_from_user(int* lsize, int* psize);
void send_comm_to_aptfunc(char* comm,Pricetree* pt,Aptlst* lst);
void insert_command_ToPools(char* comm, Commlst* lst, char** arrcomm);
Lstnodecomm* createcommNode(char* comm);
void restore_comm(char* comm, Pricetree* pt, Aptlst* lstapt, Commlst* lstcomm, char** arrcomm);
char* dull_replace(const char *in, const char *pattern, const char *by);
void freecommlst(Commlst* lst);
void insertcommtoFile(Commlst* lstcomm, char** arrcomm);
int exists(const char *fname);
void insertcommfiletoPools(Commlst* lstcomm, char** arrcomm);
long int fileSize(FILE* file);