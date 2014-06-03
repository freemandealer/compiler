#ifndef LEXER_H_H
#define LEXER_H_H

#include <stdio.h>
#include "log.h"

#define RET_ERR -1
#define RET_EOL -2		/* End of line */
#define RET_EOF -2		/* End of file */
#define RET_OK 0

#define MAX_ID_LEN 80		/* The length of single token is limited to 80 */
#define MAX_STRING_LEN 256
#define MAX_LINE_LEN 256	/* The length of single line is limited to 256 */

/**
 * |-----------------BYTE1---------------|
 * |is_spec|is_desc|is_oper|nouse|prior*4|
 */
enum type {identifier, keyword, number, string, operator, divider};
 
typedef struct _token_t {
	int type;
	union {
		char symbol;
		char content[2];
		void *entry;
	};
} token_t;

int get_tokens(char *path);
static get_a_token(char *readbuf);
void add_token(char* token_char);
static int do_alpha(int cur_first_index, char *readbuf);
static int do_digit(int cur_first_index, char *readbuf);
static int do_string(int cur_first_index, char *readbuf);
static int do_char(int cur_first_index, char *readbuf);
static int is_alpha(char c);
static int is_digit(char c);
static int is_blank(char c);
static int is_keyword(char *c);
static void set_comment_flag();
static void clear_comment_flag();
static void report_error();

#endif
