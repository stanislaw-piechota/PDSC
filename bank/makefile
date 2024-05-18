all: bank

bank: bank.o accountOps.o menu.o inputOps.o fileOps.o
	gcc -g bank.o accountOps.o menu.o inputOps.o fileOps.o -o $@  -lm

.c.o: 
	gcc -g -Wall -pedantic -c  $<

bank.o: bank.c
accountOps.o: accountOps.c
menu.o: menu.c
inputOps.o: inputOps.c
fileOps.o: fileOps.c

clean:
	-rm bank.o bank accountOps.o menu.o inputOps.o fileOps.o

check: bank
	make
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$<