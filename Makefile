# Compilação do interpretador
CC = gcc
CFLAGS = -Wall -Wextra -g

all: interpretador 

interpretador: interpretador.o comandos_ficheiros.o
	$(CC) $(CFLAGS) -o interpretador interpretador.o comandos_ficheiros.o

interpretador.o: interpretador.c comandos_ficheiros.h
	$(CC) $(CFLAGS) -c interpretador.c

comandos_ficheiros.o: comandos_ficheiros.c comandos_ficheiros.h
	$(CC) $(CFLAGS) -c comandos_ficheiros.c

clean:
	rm -f *.o interpretador