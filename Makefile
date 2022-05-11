server: main.c
	gcc -pthread -c common.c -o common.o
	gcc -pthread -c main.c -o main.o
	gcc -pthread -o server main.o common.o
