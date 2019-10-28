#include "APT_head.h"
#include "COMMAND_head.h"

void main()
{
	char* short_term_history[N];
	char* comm;
	int lsize = 0, psize = 50;
	Aptlst apt_lst;
	Pricetree price_t;
	//int code = 0;
	Commlst comm_lst;
	int k;


	//put NULL in the short comm array
	for (k = 0; k < N; k++)
	{
		short_term_history[k] = NULL;
	}

	//set apt list
	apt_lst.head = NULL;
	apt_lst.tail = NULL;

	//reset price tree
	price_t.root = NULL;

	//reset command list
	comm_lst.head = NULL;
	comm_lst.tail = NULL;

	//insert commands from the command file to the pools, only if the file exists
	if (exists("comm.txt") == 1)
	{
		insertcommfiletoPools(&comm_lst, short_term_history);
	}

	//insert apt from binary file to pools
	if (binexists("apt.bin") == 1)
	{
		fileAptToPools(&apt_lst, &price_t);
	}

	printf("Please enter one of the following commands :\n");
	printf("add-an-apt, get-an-apt, buy-an-apt or delete-an-apt\n");
	printf("For reconstruction commands, please enter :\n");
	printf("!!, !num, history, short_history or !num^str1^str2\n");
	printf("To exit, enter exit.\n\n");

	printf(">> ");
	comm = get_comm_from_user(&lsize, &psize);

	while (strcmp(comm, "exit") != 0)
	{

		if (comm[0] == '!' || strcmp(comm, "history") == 0 || strcmp(comm, "short_history") == 0)
			//if the comm is a restore command 
		{
			//printf("dddd");
			restore_comm(comm, &price_t, &apt_lst, &comm_lst, short_term_history);
		}
		else //if it is a regular command
		{
			//enter the command to the command pools
			insert_command_ToPools(comm, &comm_lst, short_term_history);
			//send the command to the right function 
			send_comm_to_aptfunc(comm, &price_t, &apt_lst);
		}


		//gettimg new command
		lsize = 0;
		printf("\n>> ");
		free(comm);
		comm = get_comm_from_user(&lsize, &psize);
	}

	//Insert commands from pools into the comm file
	insertcommtoFile(&comm_lst, short_term_history);

	//Insert apt to binary file
	aptTofile(&apt_lst);

	//free all
	freepricetree(&price_t);
	freeaptlist(&apt_lst);
	freecommlst(&comm_lst);
}