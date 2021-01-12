#include "pa2m.h"
#include "lista.h"
#include <string.h>
#include <stdio.h>

const int EXITO = 0;
const int ERROR = -1;	
const int ELEMENTOS_A_INSERTAR = 10000;
const int VECES_A_ITERAR = 100;


void lista_destruir_elemento(void* elemento) {
	return;
}

void pruebas_crear_lista() {
	lista_t* lista = NULL;
	pa2m_afirmar((lista=lista_crear(lista_destruir_elemento)) != NULL, "Se puede crear una lista");
	pa2m_afirmar(lista_vacia(lista) && (lista_elementos(lista) == 0), "La lista esta vacia");
	pa2m_afirmar(lista_ultimo(lista) == NULL, "El ultimo elemento es NULL");
	pa2m_afirmar( lista_elemento_en_posicion(lista, 10) == NULL, "No pudo devolver el elemento de una posicion inexistente");
	lista_destruir(lista);
}

void pruebas_insertar_lista(){
	lista_t* lista = NULL;
	int elemento_1 = 0, elemento_2 = 34, elemento_3 = -5;
	pa2m_afirmar(lista_insertar(lista, &elemento_1) == ERROR, "No se puede insertar un elemento en una lista nula");
	lista = lista_crear(lista_destruir_elemento);
	pa2m_afirmar(lista_insertar(lista, &elemento_1) == EXITO, "Se puede insertar al final de la lista vacia");
	pa2m_afirmar(lista_elementos(lista) == 1, "La lista tiene un elemento");
	pa2m_afirmar(lista_primero(lista) == &elemento_1, "El primer elemento es correcto");
	pa2m_afirmar(lista_ultimo(lista) == &elemento_1, "Su ultimo elemento es el insertado");
	pa2m_afirmar(lista->nodo_inicio == lista->nodo_fin, "Su nodo inicial y final son el mismo (unico nodo que tiene)");
	pa2m_afirmar(lista_insertar(lista, &elemento_2) == EXITO, "Se puede insertar al final de la lista con un elemento");
	pa2m_afirmar(lista_insertar(lista, &elemento_3) == EXITO, "Se puede insertar al final de la lista con varios elementos");
	pa2m_afirmar(!lista_vacia(lista), "La lista no esta vacia");
	pa2m_afirmar( lista_elementos(lista) == 3, "La lista tiene 3 elementos");
	pa2m_afirmar( (lista_insertar_en_posicion(lista, NULL, 1) == EXITO), "Se puede insertar NULL en la posicion 1");
	pa2m_afirmar( lista_elementos(lista) == 4, "La lista tiene 4 elementos");
	pa2m_afirmar( lista_elemento_en_posicion(lista, 1) == NULL, "El elemento en la posicion 1 es NULL");
	pa2m_afirmar( (lista_insertar_en_posicion(lista, &elemento_3, 57) == EXITO) 
				   && (lista_ultimo(lista) == &elemento_3), "Se intento insertar en una posicion inexistente, inserto al final");
	lista_destruir(lista);
}

void pruebas_borrar_lista() {
	lista_t* lista = NULL;
	pa2m_afirmar(lista_borrar(lista) == ERROR, "No se puede borrar un elemento en una lista nula");
	lista = lista_crear(lista_destruir_elemento);
	pa2m_afirmar(lista_borrar(lista) == ERROR, "No se puede borrar un elemento en una lista vacia");
	int elemento_1 = 0, elemento_2 = 34, elemento_3 = -5;

	lista_insertar(lista, &elemento_1);
	lista_insertar(lista, NULL);
	lista_insertar(lista, &elemento_2);
	lista_insertar(lista, &elemento_3);

	pa2m_afirmar( lista_borrar(lista) == EXITO, "Se puede borrar el ultimo elemento");
	pa2m_afirmar( lista_elementos(lista) == 3, "La lista quedo con 3 elementos");
	pa2m_afirmar( ( *(int*)lista_elemento_en_posicion(lista, 0) == elemento_1) 
			     && (lista_elemento_en_posicion(lista, 1) == NULL)
			     && ( lista_ultimo(lista) == &elemento_2), "Los elementos son los correctos");
	pa2m_afirmar( lista_borrar_de_posicion(lista, 2) == EXITO, "Se puede borrar un elemento de una posicion pedida");
	pa2m_afirmar( lista_elementos(lista) == 2, "La lista quedo con dos elementos");
	pa2m_afirmar( ( *(int*)lista_elemento_en_posicion(lista, 0) == elemento_1) 
			     && (lista_ultimo(lista) == NULL), "Los elementos son los correctos");
	pa2m_afirmar( lista_borrar(lista) == EXITO, "Se puede borrar un elemento nulo de la ultima posicion");
	pa2m_afirmar( lista_elementos(lista) == 1, "La lista tiene un elemento");
	pa2m_afirmar( lista_ultimo(lista) == &elemento_1, "Su ultimo elemento es el unico que tiene");
	pa2m_afirmar( lista_borrar_de_posicion(lista, 10) == EXITO, "Se intento borrar un elemento de una posicion inexistente, se borro el ultimo");
	pa2m_afirmar(lista_vacia(lista) && (lista_elementos(lista) == 0), "La lista esta vacia");
	pa2m_afirmar(lista_ultimo(lista) == NULL, "No tiene ultimo elemento");

	lista_destruir(lista);
}

void pruebas_lista_nula() {
	int elemento = 0;

	pa2m_afirmar( (lista_insertar(NULL, &elemento) == ERROR) 
			   && (lista_insertar_en_posicion(NULL, &elemento, 0) == ERROR), "No se puede insertar en una lista nula");
	pa2m_afirmar( (lista_borrar(NULL) == ERROR) && (lista_borrar_de_posicion(NULL, 0) == ERROR), "No se puede borrar elementos de una lista nula");
	pa2m_afirmar( (lista_elemento_en_posicion(NULL, 0) == NULL) && (lista_ultimo(NULL) == NULL) && (lista_elementos(NULL) == 0), "No hay elementos en una lista nula");
}

/*
 * Pre: Recibe una lista no nula, un puntero a un elemento y la cantidad de veces a insertarlo en la lista
 * Post: devuelve 0 si pudo o -1 si no pudo insertar
 */
int insertar_varias_veces_lista(lista_t* lista, void* elemento, int cantidad){

	if (cantidad <= 0)
		return EXITO;

	if (lista_insertar(lista, elemento) == ERROR)
		return ERROR;

	return (insertar_varias_veces_lista(lista, elemento, cantidad-1));
}

/*
 * Pre: lista valida no nula, recibe un elmeento y la cantidad de veces a chequear que se encuentra en la lista
 * Post: devuelve true si el elemento esta en la lista la cantidad de veces indicada (seguidas desde el comienzo) o false en caso contrario
 */
bool elementos_correctos(lista_t* lista, void* elemento, int cantidad) {

	int i = 1;
	bool error = false;
	nodo_t* nodo = lista->nodo_inicio;

	while ( (i <= lista->cantidad) && (!error) && (nodo != NULL) ) {
		if (nodo->elemento != elemento)
			error = true;
		nodo = nodo->siguiente;
		i++;
	}

	if ( (i < cantidad) || error)
		return false;

	return true;
}

void pruebas_muchos_elementos_lista() {
	lista_t* lista = lista_crear(lista_destruir_elemento);
	int elemento = 5;
	int cantidad = ELEMENTOS_A_INSERTAR;

	pa2m_afirmar( insertar_varias_veces_lista(lista, &elemento, cantidad) == EXITO, "Se pueden insertar muchos elementos en la lista");
	pa2m_afirmar( elementos_correctos(lista, &elemento, cantidad), "La informacion de la lista es correcta");

	pa2m_afirmar( (lista_borrar(lista) == EXITO) && (lista_elementos(lista) == cantidad-1), "Se elimina un elemento y queda con la cantidad correcta");

	lista_destruir(lista);
}

void pruebas_lista() {
	pruebas_crear_lista();
	pruebas_insertar_lista();
	pruebas_borrar_lista();
	pruebas_lista_nula();
	pruebas_muchos_elementos_lista();
}

void pruebas_pila() {

	lista_t* pila = lista_crear(lista_destruir_elemento);
	char elemento[30];
	strcpy(elemento, "abcdefghijklmnopqrstuvwxyz");

	pa2m_afirmar( lista_apilar(NULL, &elemento[0]) == ERROR, "No se puede apilar en una pila nula");
	pa2m_afirmar( lista_desapilar(NULL) == ERROR, "No se puede desapilar en una pila nula");
	pa2m_afirmar( lista_tope(NULL) == NULL, "El tope de una pila nula es NULL");
	pa2m_afirmar( lista_tope(pila) == NULL, "Se puede crear una pila vacia");
	pa2m_afirmar( lista_tope(pila) == NULL, "El tope de una pila vacia es NULL");
	pa2m_afirmar( lista_desapilar(pila) == ERROR, "No se puede desapilar en una pila vacia");
	pa2m_afirmar( lista_apilar(pila, &(elemento[0])) == EXITO, "Se puede apilar un elemento");
	pa2m_afirmar( lista_tope(pila) == &elemento[0], "El elemento guardado es correcto");
	pa2m_afirmar( lista_desapilar(pila) == EXITO, "Se puede desapilar el elemento");

	bool error = false;
	for (int i = 0; i < strlen(elemento); i++) {
		if ( (lista_apilar(pila, &elemento[i]) == ERROR) || (lista_tope(pila) != &elemento[i]) )
			error = true;
	}

	pa2m_afirmar(!error, "Se puede apilar varios elementos");

	error = false;
	for (int i = 0; i < strlen(elemento); i++) {
		if(lista_desapilar(pila) == ERROR)
			error = true;
	}

	pa2m_afirmar(!error && (lista_tope(pila) == 0), "Se puede desapilar todos los elementos de la pila");
	pa2m_afirmar( (lista_apilar(pila, NULL) == EXITO) && (lista_tope(pila) == NULL), "Se puede apilar un elemento nulo");

	lista_destruir(pila);
}

void pruebas_cola() {

	lista_t* cola = lista_crear(lista_destruir_elemento);
	int elementos[5] = {1, 2, 3, 4, 5};

	pa2m_afirmar( lista_encolar(NULL, &elementos[0]) == ERROR, "No se puede encolar en una cola nula");
	pa2m_afirmar( lista_desencolar(NULL) == ERROR, "No se puede desencolar en una cola nula");
	pa2m_afirmar( lista_primero(cola) == NULL, "Se creo una cola vacia");
	pa2m_afirmar( lista_desencolar(cola) == ERROR, "No se puede desencolar de una cola vacia");
	pa2m_afirmar( lista_encolar(cola, &elementos[0]) == EXITO, "Se puede encolar un elemento en una cola vacia");
	pa2m_afirmar( lista_primero(cola) == &elementos[0], "El primer elemento de la cola es su unico elemento");
	pa2m_afirmar( lista_desencolar(cola) == EXITO, "Se puede desencolar su unico elemento");

	bool error = false;
	for (int i = 0; i < 5; i++) {
		if (lista_encolar(cola, &elementos[i]) == ERROR)
			error = true;
	}

	pa2m_afirmar(!error, "Se puede encolar 5 elementos");

	error = false;
	bool elementos_correctos = true;
	for (int i = 0; i < 5; i ++) {
		if (lista_primero(cola) != &elementos[i])
			elementos_correctos = false;
		if (lista_desencolar(cola) == ERROR)
			error = true;
	}

	pa2m_afirmar(elementos_correctos, "Los elementos en la cola son correctos");
	pa2m_afirmar(!error, "Se puede desencolar todos los elementos");
	pa2m_afirmar( lista_encolar(cola, NULL) == EXITO, "Se puede encolar un elemento nulo");
	pa2m_afirmar( lista_primero(cola) == NULL, "El elemento nulo esta en la cola");

	lista_destruir(cola);
}

/*
 * Funcion que siempre devuelve true para comprobar que el iterador interno itera todos los elementos en una lista
 */
bool iterar_todos(void* elemento, void* contexto){
    return true;
}

void pruebas_iterador_interno() {
	lista_t* lista = lista_crear(lista_destruir_elemento);
	int elemento = 10, iteraciones = 0;
	bool (*funcion)(void*, void*) = iterar_todos;
	pa2m_afirmar(lista_con_cada_elemento(lista, funcion, NULL) == 0, "El iterador interno itera 0 veces una lista vacia");
	insertar_varias_veces_lista(lista, &elemento, VECES_A_ITERAR);

	iteraciones = (int)lista_con_cada_elemento(lista, funcion, NULL);
	pa2m_afirmar(VECES_A_ITERAR == iteraciones, "El iterador interno itero todos los elementos de una lista");
	pa2m_afirmar(lista_con_cada_elemento(NULL, funcion, NULL) == 0, "El iterador interno itera 0 veces una lista nula");
	pa2m_afirmar(lista_con_cada_elemento(lista, NULL, NULL) == 0, "El iterador interno no itera con una funcion nula");

	lista_destruir(lista);
}

void pruebas_iterador_sin_lista(){
	lista_t* lista_vacia = lista_crear(lista_destruir_elemento);
	pa2m_afirmar(lista_iterador_crear(NULL) == NULL, "No se puede crear un iterador con una lista nula");
	lista_iterador_t* iterador = lista_iterador_crear(lista_vacia);
	pa2m_afirmar(iterador != NULL, "Se puede crear un iterador con una lista vacia");
	pa2m_afirmar(!lista_iterador_tiene_siguiente(iterador), "El iterador no tiene siguiente");
	pa2m_afirmar(!lista_iterador_avanzar(iterador), "El iterador no puede avanzar");
	pa2m_afirmar(lista_iterador_elemento_actual(iterador) == NULL, "No hay elemento actual");

	lista_iterador_destruir(iterador);
	lista_destruir(lista_vacia);
}

void pruebas_iterador_externo() {
	
	lista_t* lista = lista_crear(lista_destruir_elemento);
	int elemento = 10, iteraciones = 0;
	bool elementos_correctos = true;
	insertar_varias_veces_lista(lista, &elemento, VECES_A_ITERAR);

	lista_iterador_t* iterador = lista_iterador_crear(lista);
	pa2m_afirmar(iterador != NULL, "Se puede crear un iterador");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador), "El iterador tiene siguiente");

	while (lista_iterador_tiene_siguiente(iterador)) {
		iteraciones++;
		if (lista_iterador_elemento_actual(iterador) != &elemento)
			elementos_correctos = false;
		lista_iterador_avanzar(iterador);
	}

	pa2m_afirmar(!lista_iterador_avanzar(iterador), "Iterador llega al final de la lista, avanzar devuelve false");
	pa2m_afirmar(!lista_iterador_tiene_siguiente(iterador), "El iterador no tiene siguiente");
	pa2m_afirmar(iteraciones == VECES_A_ITERAR, "Se itero tantas veces como elementos hay en la lista");
	pa2m_afirmar(elementos_correctos, "Los elementos devueltos por el iterador coinciden con los de la lista");

	lista_iterador_destruir(iterador);
	lista_destruir(lista);
}


int main(){

	pa2m_nuevo_grupo("PRUEBAS DE LISTA");
	pruebas_lista();
	pa2m_nuevo_grupo("PRUEBAS DE PILA");
	pruebas_pila();
	pa2m_nuevo_grupo("PRUEBAS DE COLA");
	pruebas_cola();
	pa2m_nuevo_grupo("PRUEBAS DE ITERADORES");
	pruebas_iterador_interno();
	pruebas_iterador_sin_lista();
	pruebas_iterador_externo();

	pa2m_mostrar_reporte();

	return 0;
}