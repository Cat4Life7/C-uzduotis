test.exe : main.c bigint.h bigint.o
	gcc -std=c99 -o test.exe main.c bigint.o

bigint.o : bigint.c bigint.h
	gcc -c -std=c99 bigint.c