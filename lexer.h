#ifndef LEXER_H_H
#define LEXER_H_H

#include <stdio.h>
#include "log.h"
#define STATUS_ERR -1
#define STATUS_OK 0

/* The length of single token is limited to 80 */
#define MAX_SYM_LEN 80
/* The length of single line is limited to 256 */
#define MAX_LINE_LEN 256
/**
 * |-----------------BYTE1---------------|
 * |is_spec|is_desc|is_oper|nouse|prior*4|
 */
typedef struct _attri_word_t {
	short type;
	char symbol[MAX_SYM_LEN];
} attri_word;
/*
char **key_word_table = 
{"void","int","float","char","short","if","else","while","do","return"};
*/

#endif
