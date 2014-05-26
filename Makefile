HEADERS = lexer.h 
CC = gcc

#Set LOG_ON=1 if logs are wanted, or clear it with 0
#CFLAGS += -D LOG_ON=1

lexer_test.a: lexer.o lexer_test.c $(HEADERS)
	$(CC) lexer_test.c lexer.o -o lexer_test.a
	
lexer.o: lexer.c $(HEADERS)
	$(CC) -c lexer.c -o lexer.o
	
clean:
	rm *.o *.a
	
	
