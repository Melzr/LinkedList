CC=gcc
CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
CVAL=--leak-check=full --track-origins=yes --show-reachable=yes

all: lista_prueba probar

lista_prueba: lista.c lista.h lista_minipruebas.c pa2m.h
	$(CC) lista.h lista.c lista_minipruebas.c pa2m.h $(CFLAGS) -o lista_prueba	

mias: lista.c lista.h pruebas.c
	$(CC) lista.h lista.c pruebas.c $(CFLAGS) -o lista
	valgrind $(CVAL) ./lista

correr: lista_prueba
	./lista_prueba

probar: lista_prueba
	valgrind $(CVAL) ./lista_prueba

compilar: lista.h lista.c
	$(CC) lista.h lista.c $(CFLAGS) -o lista_se

.PHONY: clean
clean:
	rm lista_prueba
	rm lista_se
	rm lista