#include "COMMAND_head.h"

char* get_comm_from_user(int* lsize, int* psize)
/*The function gets the command from the user*/
{
	char* temp = (char*)malloc(*psize);
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
			temp = (char*)realloc(temp, sizeof(char)*(*psize));
			temp[i] = ch;
			(*lsize) = *lsize + 1;
		}
		scanf("%c", &ch);
		i++;
	}

	temp[i] = '\0';
	(*lsize) = *lsize + 1;
	temp = (char*)realloc(temp, sizeof(char)*(*lsize));
	return temp;
}

void send_comm_to_aptfunc(char* comm, Pricetree* pt, Aptlst* lst)
/*The function gets a command and send it to the right apt function*/
{
	char* token;
	char sp[2] = " ";
	char* commcopy = (char*)malloc(sizeof(char)*(strlen(comm) + 1));

	strcpy(commcopy, comm);
	token = strtok(comm, sp);

	if (strcmp(token, "get-an-apt") == 0)
	{
		get_an_APT(lst, pt, commcopy + strlen(token) + 1);
	}
	if (strcmp(token, "add-an-apt") == 0)
	{
		add_APT(lst, pt, commcopy + strlen(token) + 1);
	}
	if (strcmp(token, "buy-an-apt") == 0)
	{
		buy_APT(lst, pt, commcopy + strlen(token) + 1);
	}
	if (strcmp(token, "delete-an-apt") == 0)
	{
		delete_APT(lst, pt, commcopy + strlen(token) + 1);
	}
	
	free(commcopy);
}

void insert_command_ToPools(char* comm, Commlst* lst, char** arrcomm)
/*The function gets a command and insert it to the command pools*/
{
	int is_full = 1;
	int i,j;
	Lstnodecomm* commnode;

	//if the array has available space we add the command to this place
	for (i = 0; i < N && is_full==1; i++)
	{
		if (arrcomm[i] == NULL)
		{
			arrcomm[i] = (char*)malloc(sizeof(char)*(strlen(comm) + 1));
			strcpy(arrcomm[i], comm);
			is_full = 0;
		}
	}

	if (is_full == 1)//the short term array is full we need to use the history list
	{
		//insert the old comm to the list 
		commnode = createcommNode(arrcomm[0]);
		if (lst->head == NULL) // if list is empty
		{
			lst->head = commnode;
			lst->tail = commnode;
			commnode->commindex = 1;
		}
		else
		{
			commnode->prev = lst->tail;
			lst->tail->next = commnode;
			lst->tail = commnode;
			commnode->commindex = (commnode->prev->commindex) + 1;
		}

		//reduce the short term array and add the new comm
		for (j = 0; j < 6; j++)
		{
			arrcomm[j] = arrcomm[j + 1];
		}

		//put the new command in the end of the short term array
		arrcomm[6] = (char*)malloc(sizeof(char)*(strlen(comm) + 1));
		strcpy(arrcomm[6], comm);
	}
}

Lstnodecomm* createcommNode(char* comm)
/*create command node*/
{
	Lstnodecomm* newnode = (Lstnodecomm*)malloc(sizeof(Lstnodecomm) * 1);
	newnode->prev = NULL;
	newnode->next = NULL;
	newnode->commindex = 0;
	newnode->comm = comm;

	return newnode;
}

void restore_comm(char* comm, Pricetree* pt, Aptlst* lstapt, Commlst* lstcomm, char** arrcomm)
/*This function take care of restore commands*/
{
	int i,j=1;
	int isfound = 0;
	int iscodecommcommand = 1;
	int codecomm;
	int add;
	Lstnodecomm* curr=lstcomm->head;
	char* copycomm=NULL;
	char* token;
	char cat[2] = "^";
	char* str1;
	char* str2;

	if (strcmp(comm, "!!")==0)
	{
		for (i = N - 1; i >= 0 && isfound ==0; i--)
		{
			if (arrcomm[i] != NULL)
			{
				copycomm = (char*)malloc(sizeof(char)*(strlen(arrcomm[i])+1));
				strcpy(copycomm, arrcomm[i]);
				//enter the command to the command pools
				insert_command_ToPools(copycomm,lstcomm, arrcomm);
				//send the command to the right function 
				send_comm_to_aptfunc(copycomm, pt, lstapt);
				isfound = 1;
			}
		}
	}
	
	else if (comm[0] == '!')
	{
		while (iscodecommcommand == 1 && comm[j] != '\0')
		{
			if (comm[j]<'0' || comm[j]>'9')
			{
				iscodecommcommand = 0;
			}
			
			j++;
		}

		if (iscodecommcommand == 1)
		//comm is is from the shape !<num>
		{
			codecomm = charToInt(comm + 1);

			//serach in list
			while (isfound == 0 && curr != NULL)
			{
				if (curr->commindex == codecomm)
				{
					copycomm = (char*)malloc(sizeof(char)*(strlen(curr->comm) + 1));
					strcpy(copycomm, curr->comm);
					//enter the command to the command pools
					insert_command_ToPools(copycomm, lstcomm, arrcomm);
					//send the command to the right function 
					send_comm_to_aptfunc(copycomm, pt, lstapt);
					isfound = 1;
				}

				curr = curr->next;
			}

			//what we add when we calculate the index of a command in the short term array
			if (lstcomm->tail == NULL)
			{
				add = 1;
			}
			else
			{
				add = lstcomm->tail->commindex + 1;
			}

			//search in array 
			for (i = 0; i < N && isfound == 0; i++)
			{
				if (codecomm == i + add)
				{
					copycomm = (char*)malloc(sizeof(char)*(strlen(arrcomm[i]) + 1));
					strcpy(copycomm, arrcomm[i]);
					//enter the command to the command pools
					insert_command_ToPools(copycomm, lstcomm, arrcomm);
					//send the command to the right function 
					send_comm_to_aptfunc(copycomm, pt, lstapt);
					isfound = 1;
				}
			}
		}
		else
		//comm is from the shape !<num>^str1^str2
		{
			//extract the details from the restore command
			//num
			token = strtok(comm + 1, cat);
			codecomm = charToInt(token);
			//str1
			token = strtok(NULL, cat);
			str1 = (char*)malloc(sizeof(char)*(strlen(token) + 1));
			strcpy(str1, token);
			//str2
			token = strtok(NULL, cat);
			str2 = (char*)malloc(sizeof(char)*(strlen(token) + 1));
			strcpy(str2, token);

			//search for the command
			//serach in list
			while (isfound == 0 && curr != NULL)
			{
				if (curr->commindex == codecomm)
				{
					//replace str1 in str2
					copycomm = dull_replace(curr->comm, str1, str2);
					//enter the command to the command pools
					insert_command_ToPools(copycomm, lstcomm, arrcomm);
					//send the command to the right function 
					send_comm_to_aptfunc(copycomm, pt, lstapt);
					isfound = 1;
				}

				curr = curr->next;
			}

			//what we add when we calculate the index of a command in the short term array
			if (lstcomm->tail == NULL)
			{
				add = 1;
			}
			else
			{
				add = lstcomm->tail->commindex + 1;
			}

			//search in array 
			for (i = 0; i < N && isfound == 0; i++)
			{
				if (codecomm == i + add)
				{
					//replace str1 in str2
					copycomm = dull_replace(arrcomm[i], str1, str2);
					//enter the command to the command pools
					insert_command_ToPools(copycomm, lstcomm, arrcomm);
					//send the command to the right function 
					send_comm_to_aptfunc(copycomm, pt, lstapt);
					isfound = 1;
				}
			}
		}
	}
	else if (strcmp(comm, "history") == 0)
	{
		//what we add when we calculate the index of a command in the short term array
		if (lstcomm->tail == NULL)
		{
			add = 1;
		}
		else
		{
			add = lstcomm->tail->commindex + 1;
		}

		//print from list
		while (curr != NULL)
		{
			printf("%d: %s\n", curr->commindex, curr->comm);
			curr = curr->next;
		}

		//print from array
		for (i = 0; i < N; i++) 
		{
			if (arrcomm[i] != NULL)
			{
				printf("%d: %s\n", i+add, arrcomm[i]);
			}

		}

	}
	else if (strcmp(comm, "short_history") == 0)
	{
		//what we add when we calculate the index of a command in the short term array
		if (lstcomm->tail == NULL)
		{
			add = 1;
		}
		else
		{
			add = lstcomm->tail->commindex + 1;
		}

		//print from array
		for (i = 0; i < N; i++)
		{
			if (arrcomm[i] != NULL)
			{
				printf("%d: %s\n", i + add, arrcomm[i]);
			}

		}

	}

	//free copycomm
	if (copycomm != NULL)
		free(copycomm);
}

char* dull_replace(const char *in, const char *pattern, const char *by)
/*The function gets a command and str1 to be replaced with str2*/
{
	size_t outsize = strlen(in) + 1;
	// TODO maybe avoid reallocing by counting the non-overlapping occurences of pattern
	char *res = malloc(outsize);
	// use this to iterate over the output
	size_t resoffset = 0;

	char *needle;
	while (needle = strstr(in, pattern)) {
		// copy everything up to the pattern
		memcpy(res + resoffset, in, needle - in);
		resoffset += needle - in;

		// skip the pattern in the input-string
		in = needle + strlen(pattern);

		// adjust space for replacement
		outsize = outsize - strlen(pattern) + strlen(by);
		res = realloc(res, outsize);

		// copy the pattern
		memcpy(res + resoffset, by, strlen(by));
		resoffset += strlen(by);
	}

	// copy the remaining input
	strcpy(res + resoffset, in);

	return res;
}

void freecommlst(Commlst* lst)
/*free comm list*/
{
	Lstnodecomm* curr = lst->head;
	Lstnodecomm* tmp;

	while (curr != NULL)
	{
		tmp = curr->next;
		free(curr->comm);
		free(curr);
		curr = tmp;
	}
}

void insertcommtoFile(Commlst* lstcomm, char** arrcomm)
/*Insert commands from pools into the comm file*/
{
	FILE* f;
	int i;
	Lstnodecomm* curr = lstcomm->tail;

	//of there are commnd in pools
	if (arrcomm[0] != NULL)
	{
		f = fopen("comm.txt", "wt");

		//insert command in the array to the file
		for (i = N - 1; i >= 0; i--)
		{
			if (arrcomm[i] != NULL)
			{
				fputs(arrcomm[i], f);
				fputc('\n', f);
			}
		}

		//inser commands in the list to the file
		while (curr != NULL)
		{
			fputs(curr->comm, f);
			fputc('\n', f);
			curr = curr->prev;
		}

		fclose(f);
	}
}

int exists(const char *fname)
/*The function check if a file exists*/
{
	FILE *file;
	if ((file = fopen(fname, "r")))
	{
		fclose(file);
		return 1;
	}
	return 0;
}

void insertcommfiletoPools(Commlst* lstcomm, char** arrcomm)
{
	FILE* f = fopen("comm.txt", "rt");
	long int fSize = fileSize(f);
	int isless7 = 0;
	int count=0;
	char c;
	char* comm = NULL;
	int index = N-1;
	int lSize = 0;
	int pSize = 30;
	Lstnodecomm* commnode;
	
	//check if there are less than 7 commands in file
	// Extract characters from file and store in character c
	for (c = getc(f); c != EOF && count<10; c = getc(f))
		if (c == '\n') // Increment count if this character is newline
			count++;
	rewind(f);

	if (count >= 7)//there are more than 7 commands in the file
	{
		for (int i = 0; i < N; i++)
			arrcomm[i] = (char*)malloc(pSize);
		while (ftell(f) < fSize && index >=0)
		{
			if (lSize == pSize)
			{
				pSize *= 2;
				arrcomm[index] = realloc(arrcomm[index], pSize);
			}
			fread(&c, sizeof(char), 1, f);
			if (c != '\n')
			{
				arrcomm[index][lSize] = c;
				lSize++;
			}
			else
			{
				if (lSize < pSize - 1)
					arrcomm[index] = realloc(arrcomm[index], lSize + 1);
				arrcomm[index][lSize] = '\0';
				lSize = 0;
				pSize = 30;
				index--;
			}
		}
		while (ftell(f) < fSize)
		{
			if (lSize == 0)
				comm = (char*)malloc(pSize);
			if (lSize == pSize)
			{
				pSize *= 2;
				comm = realloc(comm, pSize);
			}
			fread(&c, sizeof(char), 1, f);
			if (c != '\n')
			{
				comm[lSize] = c;
				lSize++;
			}
			else
			{
				if (lSize < pSize - 1)
					comm = realloc(comm, lSize + 1);
				comm[lSize]= '\0';
				commnode = createcommNode(comm);
				if (lstcomm->head == NULL) // if list is empty
				{
					lstcomm->head = commnode;
					lstcomm->tail = commnode;
				}
				else
				{
					commnode->next = lstcomm->head;
					lstcomm->head->prev = commnode;
					lstcomm->head = commnode;
				}
				lSize = 0;
				pSize = 30;
			}
		}
		Lstnodecomm* curr = lstcomm->head;
		int commIndex = 1;
		while (curr != NULL)
		{
			curr->commindex = commIndex;
			commIndex++;
			curr = curr->next;
		}

	}
	else
	{
		index = count - 1;
		for (int i = 0; i < count; i++)
			arrcomm[i] = (char*)malloc(pSize);
		while (ftell(f) < fSize && index >= 0)
		{
			if (lSize == pSize)
			{
				pSize *= 2;
				arrcomm[index] = realloc(arrcomm[index], pSize);
			}
			fread(&c, sizeof(char), 1, f);
			if (c != '\n')
			{
				arrcomm[index][lSize] = c;
				lSize++;
			}
			else
			{
				if (lSize < pSize - 1)
					arrcomm[index] = realloc(arrcomm[index], lSize + 1);
				arrcomm[index][lSize] = '\0';
				lSize = 0;
				pSize = 30;
				index--;
			}
		}
	}
}

long int fileSize(FILE* file)
{/*returns the size of the file*/
	long int res;

	fseek(file, 0, SEEK_END);
	res = ftell(file);
	fseek(file, 0, SEEK_SET);

	return res;
}