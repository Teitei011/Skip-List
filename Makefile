all: main.o skiplist.o
	gcc -o dicionario *.c
run:
	./dicionario
