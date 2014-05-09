#include "lexer.h"

static FILE* filp;
char readbuf[MAX_LINE_LEN];


int open_file(char *path)
{
	filp = fopen(path,"r");
	if (filp == NULL) {
		printf("Cannot open source file: %s\n",path);
		return STATUS_ERR;
	}
	return STATUS_OK;
}

int readline_file()
{
	if (filp == NULL) {
		printf("Cannot read source file.\n");
		return STATUS_ERR;
	}
	fgets(readbuf,MAX_LINE_LEN,filp);
}

int get_token()
{
	if (readbuf == NULL)
		printf("Empty\n"); //TODO
}
