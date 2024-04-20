all: test
	-./$^

bsearch: bsearch.o
	gcc -g $^ -o $@  -lm
test: test.o bsearch.o
	gcc -g $^ -o $@  -lm

.c.o: 
	gcc -g -Wall -pedantic -c  $<

bsearch.o: bsearch.c
test.o: test.c

clean:
	-rm bsearch.o test test.o