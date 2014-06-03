#include "lexer.h"
#include <string.h>

static int line_num = 1;
static char comment_flag = 0;

int get_tokens(char *path)
{

	FILE *filp;
	char readbuf[MAX_LINE_LEN];
	int ret_val;
	
	filp = fopen(path,"r");
	if (filp == NULL) {
		printf("Cannot open source file: %s\n", path);
		return RET_ERR;
	}
	while(fgets(readbuf, MAX_LINE_LEN, filp)) {
		/* scanning the line */
		while(RET_OK == get_a_token(readbuf))
		{
		}
	}
}

static int get_a_token(char *readbuf)
{
	/**
	 * cur_first_index always points to the char just after 
	 * the last token or the first char of the line.
	 */
	static int cur_first_index = 0;
	char first_char;
	
	if (readbuf[cur_first_index] == '\n') {
		line_num ++;
		/* Re-initialize the index for next line */
		cur_first_index = 0;
		printf("\n line%3d:", line_num); //TODO: println only in demo
		return RET_EOL;
	}
	first_char = readbuf[cur_first_index];
	/* If the comment wasn't closed */
	if (comment_flag == 1) {
		if (first_char != '*') {
			cur_first_index ++;
			return RET_OK;
		} else {
			if (readbuf[cur_first_index+1] == '/') {
				cur_first_index += 2;
				clear_comment_flag();
				return RET_OK;
			} else {
				cur_first_index ++;
				return RET_OK;
			}
		}
	} else if (is_alpha(first_char)) {
		cur_first_index = do_alpha(cur_first_index, readbuf);
		return RET_OK;
	} else if (is_digit(first_char)) {
		cur_first_index = do_digit(cur_first_index, readbuf);
		return RET_OK;
	} else if (is_blank(first_char)) {
		cur_first_index ++;
		return RET_OK;
	} else if (first_char == '"') {
		cur_first_index = do_string(cur_first_index, readbuf);
		return RET_OK;
	} else if (first_char == '/') {
		if (readbuf[cur_first_index+1] == '/') {
			line_num ++;
			/* Re-initialize the index for next line */
			cur_first_index = 0;
			return RET_EOL;
		} else if (readbuf[cur_first_index+1] == '*') {
			cur_first_index ++;
			set_comment_flag();
			return RET_OK;
		} 
	} else if (first_char == '\'') {
		cur_first_index = do_char(cur_first_index, readbuf);
		return RET_OK;
	} else {
		/* Yeah, it is nasty! */	
		switch(first_char) {
		case '+':
			if (readbuf[cur_first_index+1] == '+') {		// ++
				add_token("<++,OP>");
				cur_first_index += 2;
			} else if (readbuf[cur_first_index+1] == '=') { // +=
				add_token("<+=,OP>");
				cur_first_index += 2;	
			} else {										// +
				add_token("<+,OP>");
				cur_first_index ++;
			}
			break;
		case '-':
			if (readbuf[cur_first_index+1] == '-') {		// --
				add_token("<--,OP>");
				cur_first_index += 2;
			} else if (readbuf[cur_first_index+1] == '=') { // -=
				add_token("<-=,OP>");
				cur_first_index += 2;
			} else if (readbuf[cur_first_index+1] == '>') { // -> 
				add_token("<->,OP>");
				cur_first_index += 2;
			} else {										// -
				add_token("<-,OP>");
				cur_first_index ++;
			}
			break;
		case '*':
			if (readbuf[cur_first_index+1] == '=') {		// *=
				add_token("<*=,OP>");
				cur_first_index += 2;
			} else {										// *
				add_token("<*,OP>");
				cur_first_index ++;
			}
			break;
		case '/':
			if (readbuf[cur_first_index+1] == '=') {		// /=
				add_token("</=,OP>");
				cur_first_index += 2;
			} else {										// /
				add_token("</,OP>");
				cur_first_index ++;
			}
			break;
		case '%':
			if (readbuf[cur_first_index+1] == '=') {		// %=
				add_token("<%%=,OP>");
				cur_first_index += 2;
			} else {										// %
				add_token("<%%,OP>");
				cur_first_index ++;
			}
			break;
		case '<':
			if (readbuf[cur_first_index+1] == '=') {		// <=
				add_token("<<=,OP>");
				cur_first_index += 2;
			} else if (readbuf[cur_first_index+1] == '<') {	// <<
				add_token("<<<,OP>");
				cur_first_index += 2;
			} else {										// <
				add_token("<<,OP>");
				cur_first_index ++;
			}
			break;
		case '=':
			if (readbuf[cur_first_index+1] == '=') {		// ==
				add_token("<==,OP>");
				cur_first_index += 2;
			} else {										// =
				add_token("<=,OP>");
				cur_first_index ++;
			}
			break;
		case '!':
			if (readbuf[cur_first_index+1] == '=') {		// !=
				add_token("<!=,OP>");
				cur_first_index += 2;
			} else {										// !
				add_token("<!,OP>");
				cur_first_index ++;
			}
			break;
		case '>':
			if (readbuf[cur_first_index+1] == '=') {		// >=
				add_token("<>=,OP>");
				cur_first_index += 2;
			} else if (readbuf[cur_first_index+1] == '>') {	// >>
				add_token("<>>,OP>");
				cur_first_index += 2;
			} else {										// >
				add_token("<>,OP>");
				cur_first_index ++;
			}
			break;
		case '~':											// ~
			add_token("<~,OP>");
			cur_first_index ++;
			break;
		case '(':											// (
			add_token("<(,OP>");
			cur_first_index ++;
			break;
		case ')':											// )
			add_token("<),OP>");
			cur_first_index ++;
			break;
		case '|':
			if (readbuf[cur_first_index+1] == '=')	{		// |=
				add_token("<|=,OP>");
				cur_first_index += 2;
			} else if (readbuf[cur_first_index+1] == '|') {	// ||
				add_token("<||,OP>");
				cur_first_index += 2;
			} else {										// |
				add_token("<|,OP>");
				cur_first_index ++;
			}
			break;
		case '&':
			if (readbuf[cur_first_index+1] == '=')	{		// &=
				add_token("<&=,OP>");
				cur_first_index += 2;
			} else if (readbuf[cur_first_index+1] == '&') {	// &&
				add_token("<&&,OP>");
				cur_first_index += 2;
			} else {										// &
				add_token("<&,OP>");
				cur_first_index ++;
			}
			break;		
		case ',':
				add_token("<,,OP>");
				cur_first_index ++;
			break;
		case '#':
				add_token("<#,SP>");
				cur_first_index ++;
			break;
		case ';':
				add_token("<;,SEP>");
				cur_first_index ++;
			break;
		case '{':
				add_token("<{,SEP>");
				cur_first_index ++;
			break;
		case '}':
				add_token("<},SEP>");
				cur_first_index ++;
			break;
		case '[':
				add_token("<[,OP>");
				cur_first_index ++;
			break;
		case ']':
				add_token("<],OP>");
				cur_first_index ++;
			break;
		case ':':
				add_token("<:,OP>");
				cur_first_index ++;
			break;
		default:
			cur_first_index ++;
			report_error();
		}
		/**
		 * Yean, I know it's strange to return OK when it 
		 * actually run into error. However, this can make
		 * the compiling work continue and we can set up an 
		 * error list or flag to deal with this issue. 
		 */
		return RET_OK;	
	}
}

/**
 * token_char is a string for legal token. In this function
 * We will turn token_char into struct token_t and add it to
 * the right token list. 
 * TODO No I lied, as a simple demo, it only do some prints :P
 */
void add_token(char* token_char)
{
	printf("%s,", token_char);
}

/**
 * The return value of do_XXX is the cur_first_index which
 * always points to the char just after the last token.
 */
static int do_alpha(int cur_first_index, char *readbuf)
{
	int cur_index;
	char cur_char;
	char token_buf[MAX_ID_LEN];
	char temp[MAX_ID_LEN+8];  //TODO
	
	cur_index = cur_first_index;
	while(1){
		cur_char = readbuf[cur_index];
		if (is_alpha(cur_char) || isdigit(cur_char) ||
				cur_char == '_') {
			cur_index ++;
			continue;
		} else
			break;
	}
	/**
	 * readbuf[cur_first_index ... cur_index-1] 
	 * is what we want
	 */
	memset(token_buf, 0, sizeof(token_buf));
	strncpy(token_buf, &(readbuf[cur_first_index]),
				cur_index - cur_first_index);
	if (is_keyword(token_buf)) {
		temp[0] = '<';
		strcpy(temp+1, token_buf);
		strcpy(temp+strlen(token_buf)+1, ",KEY>");
		add_token(temp);
	} else {
		temp[0] = '<';
		strcpy(temp+1, token_buf);
		strcpy(temp+strlen(token_buf)+1, ",IDF>");
		add_token(temp);
	}
	return cur_index;
}

static int do_digit(int cur_first_index, char *readbuf)
{
	int cur_index;
	char cur_char;
	char token_buf[MAX_ID_LEN];
	char temp[MAX_ID_LEN+8]; //TODO
	
	cur_index = cur_first_index;
	while(1){
		cur_char = readbuf[cur_index];
		if (is_digit(cur_char)){  
			//TODO add error detector for x.xxx.x
			cur_index ++;
			continue;
		} else
			break;
	}
	/**
	 * readbuf[cur_first_index ... cur_index-1] 
	 * is what we want
	 */
	memset(token_buf, 0, sizeof(token_buf));
	strncpy(token_buf, &(readbuf[cur_first_index]), 
				cur_index - cur_first_index);
	temp[0] = '<';
	strcpy(temp+1, token_buf);
	strcpy(temp+strlen(token_buf)+1, ",NUM>");
	add_token(temp);
	return cur_index;
}

static int do_string(int cur_first_index, char *readbuf)
{
	int cur_index;
	char cur_char;
	char token_buf[MAX_STRING_LEN];	
	int time_out;
	char temp[MAX_STRING_LEN+8]; //TODO
	
	time_out = MAX_STRING_LEN;
	cur_index = cur_first_index + 1;
	while(time_out --) {
		cur_char = readbuf[cur_index];
		if (cur_char != '"') {
			cur_index ++;
			continue;
		} else
			goto out;
	}
	/* We find no end token for the string */
	report_error();
	return cur_index+1;
out:
	memset(token_buf, 0, sizeof(token_buf));
	strncpy(token_buf, &(readbuf[cur_first_index+1]), 
				cur_index-cur_first_index-1);
	temp[0] = '<';
	strcpy(temp+1, token_buf);
	strcpy(temp+strlen(token_buf)+1, ",IDF>");
	add_token(temp);
	return cur_index+1;
}

static int do_char(int cur_first_index, char *readbuf)
{
	printf("%c,", readbuf[cur_first_index+1]);
	return (cur_first_index + 3); 
}

static int is_alpha(char c)
{
	return (isalpha(c) || (c == '_'));
}

static int is_digit(char c)
{
	//TODO tell if '.' is the operator
	return (isdigit(c) || (c == '.'));
}

static int is_blank(char c)
{
	return (isblank(c) || (c == '\t'));
}

#define KEYWORD_TBL_LEN 20
char keyword_tbl[KEYWORD_TBL_LEN][10] = {
"if","else","while","do","int","float","double",
"void","char","short","long","static","extern",
"struct","enum","switch","case","for","break","return"
};

static int is_keyword(char *c)
{
	int i;
	
	for (i=0; i<KEYWORD_TBL_LEN; i++)
		if (!strcmp(c, keyword_tbl[i]))
			return 1;
	return 0;
}

static void set_comment_flag()
{
	comment_flag = 1;
}

static void clear_comment_flag()
{
	comment_flag = 0;
}

static void report_error()
{
	printf("\n[error] in line:%d\n", line_num);
}
