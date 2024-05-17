all: bank
	-./$^

bank: bank.o accountOps.o menu.o inputOps.o
	gcc -g bank.o accountOps.o menu.o inputOps.o -o $@  -lm

.c.o: 
	gcc -g -Wall -pedantic -c  $<

bank.o: bank.c
accountOps.o: accountOps.c
menu.o: menu.c
inputOps.o: inputOps.c

clean:
	-rm bank.o bank accountOps.o menu.o inputOps.o