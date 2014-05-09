#include "lexer.h"

extern char readbuf[];

int main()
{
	open_file("./test/test.c");
	readline_file();
	printf("%s\n",readbuf);
}
