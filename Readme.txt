
SOBRE LO ENTREGADO
    
    El TDA entregado consiste en una lista de nodos simplemente enlazados que incluye ademas funciones de pila y cola, un iterador interno y externo.
    Una de las ventajas de esta implementacion es que la memoria no tiene que ser contigua.
  	Cada nodo contiene un puntero al elemento que contiene y un puntero al nodo siguiente o NULL en caso de ser el ultimo. Una lista en esta
  	implementacion consiste entonces en referencias a su primer y ultimo nodo, y la cantidad de elementos que contiene. Asi, puede ser recorrida
  	secuencialmente accediendo nodo a nodo.
  	En la implementacion de pila tome el primer nodo como el 'tope' asi no debo recorrer toda la pila al desapilar, y en la cola los elementos ingresan
  	por el ultimo nodo y se desencolan por el primero.


COMPILACION Y EJECUCION:

	Con las minipruebas dadas en el enunciado:
		gcc lista.c lista_minipruebas.c -o lista_se -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

	Con mis pruebas:
		gcc lista.c pruebas.c -o lista_se -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

	Ejecucion:
		./lista_se

	Valgrind:
		valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./lista_se


CONCEPTOS TEORICOS:

	¿Qué es lo que entendés por una lista? ¿Cuáles son las diferencias entre ser simple o doblemente enlazada?

		Una lista es un conjunto de elementos ordenados que implementamos mediante nodos enlazados. La diferencia entre nodos simple y
		doblemente enlazados es que los simples solo conocen al nodo siguiente, mientras que los dobles conocen al nodo anterior y al siguiente.

	¿Cuáles son las características fundamentales de las Pilas? ¿Y de las Colas?

		Pilas: la pila es un conjunto de elementos que se puede agrandar (apilar) o achicar (desapilar) por uno de sus extremos, denominado tope.
		Algunas de sus operaciones basicas son crear, apilar, desapilar, tope, destruir.

		Colas: estas estructuras poseen dos extremos, uno es el inicio o frente y el otro el final de la cola. Al agregar un elemento (encolar) este
		entra por el final de la cola, y al quitar uno (desencolar) se quita el elemento del frente, que fue el primero en entrar a la cola.
		Algunas de sus operaciones basicas son: crear, encolar, desencolar, primero, destruir.

	¿Qué es un iterador? ¿Cuál es su función?

		Un iterador es una estructura o tda que nos permite recorrer secuencialmente los elementos en una lista, pila, cola u otros tipos de dato
		sin tener que conocer exactamente su estructura.

	¿En qué se diferencia un iterador interno de uno externo?

		Un iterador interno es parte de la implementacion del tda lista y conoce la implementacion de la misma, mientras que uno externo es un tda
		aparte que provee funciones para recorrer estructuras, dejando al usuario a cargo de controlar el ciclo en el cual se recorre, a diferencia
		del interno.
