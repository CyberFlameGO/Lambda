# MIT License
# 
# Copyright (c) 2023 Adi Salimgereyev (Vertex)
# 
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

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