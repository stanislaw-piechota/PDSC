all: text

text: text.o
	gcc -g $^ -o $@  -lm

.c.o: 
	gcc -g -Wall -pedantic -c  $<

text.o: text.c

clean:
	-rm text.o text

check: text
	make
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$< < input.txt