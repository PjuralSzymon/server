server: main.c
	gcc -c common.c -o common.o
	gcc -c main.c -o main.o
	gcc -o server main.o common.o
