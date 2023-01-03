sources = $(filter-out lambda_lex.c lambda.c, $(wildcard *.c))
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic

main:
	make lambda.out
	make lambda_lex.out

lambda.out: $(objects) lambda.o
	gcc -c $(flags) lambda.c -o lambda.o 
	gcc $(objects) lambda.o $(flags) -o lambda.out

lambda_lex.out: $(objects) lambda_lex.o
	gcc -c $(flags) lambda_lex.c -o lambda_lex.o 
	gcc $(objects) lambda_lex.o $(flags) -o lambda_lex.out

liblambda.a: $(objects)
	ar rcs $@ $^

%.o: %.c %.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm *.a