all: bank
	-./$^

bank: bank.o
	gcc -g $^ -o $@  -lm

.c.o: 
	gcc -g -Wall -pedantic -c  $<

bank.o: bank.c

clean:
	-rm bank bank.o