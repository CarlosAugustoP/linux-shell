all:main.c
	gcc -o shell main.c -pthread 
clear:
	rm shell 