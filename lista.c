#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

static const int EXITO = 0;
static const int ERROR = -1;
static const size_t ERROR_ITERADOR = 0;



/****************************************** LISTA *********************************************/


/*
 * Devuelve -1 si la lista es NULL o 0 si se pudo inicializar con nodo_inicio y nodo_fin apuntando a NULL y cantidad = 0
 */
int inicializar_lista(lista_t* lista) {

	if (lista == NULL)
		return ERROR;

	lista->nodo_inicio = NULL;
	lista->nodo_fin = NULL;
	lista->cantidad = 0;

	return EXITO;
}


lista_t* lista_crear(lista_destructor destructor) {

	lista_t* lista = malloc(sizeof(lista_t));
	lista->destructor = destructor;
	inicializar_lista(lista);
	
	return lista;
}

/*
 * Post: crea un nodo en la memoria dinamica inicializado con su siguiente en NULL y el elemento recibido
 *		 devuelve NULL en caso de no poder reservar la memoria necesaria
 */
nodo_t* crear_nodo(void* elemento){

	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) 
		return NULL;

	nodo->elemento = elemento;
	nodo->siguiente = NULL;

	return nodo;
}

int lista_insertar(lista_t* lista, void* elemento) {

	if (lista == NULL) 
		return ERROR;

	nodo_t* nuevo_nodo = crear_nodo(elemento);
	if (nuevo_nodo == NULL)
		return ERROR;

	if (lista->cantidad == 0) {
		lista->nodo_inicio = nuevo_nodo;
	} else if (lista->nodo_fin == NULL) {
		free(nuevo_nodo);
		return ERROR;
	} else {
		lista->nodo_fin->siguiente = nuevo_nodo;
	}

	lista->nodo_fin = nuevo_nodo;
	lista->cantidad++;

	return EXITO;
}

/*
 * Pre: todos los parametros recibidos son validos, estan inicializados
 * Post: devuelve 0 si pudo insertar o -1 en caso contrario
 */
int insertar_nodo_aux(nodo_t* nuevo_nodo, nodo_t* nodo_lista, size_t posicion_actual, size_t posicion_buscada) {
	if (nodo_lista == NULL)
		return ERROR;

	if (posicion_actual == posicion_buscada-1) {
		nuevo_nodo->siguiente = nodo_lista->siguiente;
		nodo_lista->siguiente = nuevo_nodo;
		return EXITO;
	}

	return (insertar_nodo_aux(nuevo_nodo, nodo_lista->siguiente, posicion_actual+1, posicion_buscada));
}

/*
 * Pre: recibe la lista donde se insertara, el nodo y posicion donde se insertara. (Todos los datos validos)
 * 		LA POSICION PUEDE SER CUALQUIERA MENOS LA 0
 * Post: devuelve 0 si pudo insertar o -1 en caso contrario
 */
int insertar_nodo(lista_t* lista, size_t posicion, nodo_t* nuevo_nodo) {

	if (lista == NULL || nuevo_nodo == NULL)
		return ERROR;

	if( insertar_nodo_aux(nuevo_nodo, lista->nodo_inicio, 0, posicion) == ERROR )
		return ERROR;

	lista->cantidad++;
	return EXITO;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion) {

	if (lista == NULL) 
		return ERROR;

	if (posicion >= lista->cantidad)
		return lista_insertar(lista, elemento);

	nodo_t* nuevo_nodo = crear_nodo(elemento);
	if (nuevo_nodo == NULL) 
		return ERROR;

	if (posicion == 0) {
		nuevo_nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo_nodo;
		lista->cantidad++;
		return EXITO;
	}

	if (insertar_nodo(lista, posicion, nuevo_nodo) == ERROR) {
		free(nuevo_nodo);
		return ERROR;
	}

	return EXITO;
}

int lista_borrar(lista_t* lista) {

	if (lista == NULL)
		return ERROR;

	if (lista->cantidad == 0)
		return ERROR;

	if (lista->cantidad == 1) {
		free(lista->nodo_inicio);
		inicializar_lista(lista);
		return EXITO;
	}

	if (lista->destructor != NULL)
		lista->destructor(lista->nodo_fin->elemento);

	int i = 1;
	bool error = false;
	lista->cantidad--;
	free(lista->nodo_fin);
	nodo_t* nodo_actual = lista->nodo_inicio;

	while ( (i <= lista->cantidad) && !error ) {
		if (nodo_actual == NULL)
			error = true;
		if (i == lista->cantidad) {
			lista->nodo_fin = nodo_actual;
			lista->nodo_fin->siguiente = NULL;
		} else {
			nodo_actual = nodo_actual->siguiente;
		}
		i++;
	}

	return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion) {

	if (lista == NULL)
		return ERROR;

	if (posicion+1 >= lista->cantidad)
		return lista_borrar(lista);

	lista->cantidad--;

	if (posicion == 0) {
		nodo_t* nodo_aux = lista->nodo_inicio;
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
		free(nodo_aux);
		return EXITO;
	}

	int i = 0;
	bool error = false;
	nodo_t* nodo_actual = lista->nodo_inicio;

	while ( (i < posicion) && !error ) {
		if (nodo_actual == NULL)
			error = true;
		if (i == posicion-1) {
			nodo_t* nodo_aux = nodo_actual->siguiente;
			nodo_actual->siguiente = nodo_actual->siguiente->siguiente;
			if (lista->destructor != NULL)
				lista->destructor(nodo_aux->elemento);
			free(nodo_aux);
		} else
			nodo_actual = nodo_actual->siguiente;
		i++;
	}

	return EXITO;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion) {

	if ( (lista == NULL) || (posicion+1 > lista->cantidad) )
		return NULL;

	int i = 0;
	bool error = false;
	void* elemento = NULL;
	nodo_t* nodo_actual = lista->nodo_inicio;

	while ( (i <= posicion) && !error ) {
		if (nodo_actual == NULL)
			error = true;
		if (i == posicion)
			elemento = nodo_actual->elemento;
		else
			nodo_actual = nodo_actual->siguiente;

		i++;
	}

	return elemento;
}

void* lista_ultimo(lista_t* lista) {

	if(lista == NULL)
		return NULL;

	if( (lista->cantidad == 0) || (lista->nodo_fin == NULL) )
		return NULL;

	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista) {
	if (lista == NULL)
		return true;

	if(lista->cantidad == 0)
		return true;

	return false;
}

size_t lista_elementos(lista_t* lista) {

	if (lista == NULL)
		return 0;

	return lista->cantidad;
}

void lista_destruir(lista_t* lista) {

	if (lista == NULL)
		return;

	while(lista->cantidad > 0){
		lista_borrar_de_posicion(lista, 0);
	}

	free(lista);
}



/****************************************** PILA *********************************************/


int lista_apilar(lista_t* lista, void* elemento) {
	return lista_insertar_en_posicion(lista, elemento, 0);
}

int lista_desapilar(lista_t* lista) {
	return lista_borrar_de_posicion(lista, 0);
}

void* lista_tope(lista_t* lista) {
	return lista_elemento_en_posicion(lista, 0);
}



/****************************************** COLA *********************************************/


int lista_encolar(lista_t* lista, void* elemento) {
	return lista_insertar(lista, elemento);
}

int lista_desencolar(lista_t* lista) {
	return lista_borrar_de_posicion(lista, 0);
}

void* lista_primero(lista_t* lista) {
	return lista_elemento_en_posicion(lista, 0);
}



/*************************************** ITERADORES ******************************************/


lista_iterador_t* lista_iterador_crear(lista_t* lista) {

	if (!lista)
		return NULL;

	lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador) 
		return NULL;

	iterador->lista = lista;
	iterador->corriente = lista->nodo_inicio;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador) {
	if (!iterador)
		return false;
	if (!iterador->corriente)
		return false;

	return (iterador->corriente != NULL);
}

bool lista_iterador_avanzar(lista_iterador_t* iterador) {
	if (!iterador)
		return false;
	if (!iterador->corriente)
		return false;

	iterador->corriente = iterador->corriente->siguiente;
	return(iterador->corriente != NULL);
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador) {
	if(!iterador)
		return NULL;
	if(!iterador->corriente)
		return NULL;

	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador) {
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto) {
	if (!lista)
		return ERROR_ITERADOR;

	if (funcion == NULL)
		return ERROR_ITERADOR;

	size_t iteraciones = 0;
	nodo_t* nodo = lista->nodo_inicio;

	while ( (nodo != NULL) && (*funcion)(nodo->elemento, contexto) ) {
		nodo = nodo->siguiente;
		iteraciones++;
	}

	return iteraciones;
}