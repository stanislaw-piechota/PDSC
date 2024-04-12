all: text

text: text.o rand_malloc.o
	gcc -g $^ -o $@  -lm

.c.o: 
	gcc -g -Wall -pedantic -c  $<

text.o: text.c

rand_malloc.o: rand_malloc.c

clean:
	-rm text.o text rand_malloc.o

check: text
	make
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$< < tests/0.txt