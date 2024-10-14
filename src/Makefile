.PHONY: clean

CFLAGS=-Wall -g

driver: driver.o rbtree.o

clean:
	rm -f driver *.o
test:\n\tgcc -o test_runner test_runner.c driver.c rbtree.c -I.\n\t./test_runner

.PHONY: clean test

test:
	gcc -o test_runner test_runner.c driver.c rbtree.c -I.
	./test_runner
