#define _POSIX_C_SOURCE 200809L
#include "lista_de_espera.h"
#include "cola.h"
#include "heap.h"
#include <string.h>
#include <stdlib.h>

struct lista_de_espera{
	cola_t* urgencias;
	heap_t* regulares;
	size_t total_espera;
	lista_de_espera_comparar_clave_t cmp;
	lista_de_espera_destruir_dato_t funcion_destruir;
};

void lista_de_espera_destruir(lista_de_espera_t* lista) {
	if(lista->urgencias)
		cola_destruir(lista->urgencias, lista->funcion_destruir);
	if(lista->regulares)
		heap_destruir(lista->regulares, lista->funcion_destruir);
	free(lista);
}

size_t lista_de_espera_cantidad_total(lista_de_espera_t* lista){
	return lista->total_espera;
}

lista_de_espera_t* lista_de_espera_crear(lista_de_espera_comparar_clave_t cmp, lista_de_espera_destruir_dato_t funcion_destruir) {
	lista_de_espera_t* lista_de_espera = malloc(sizeof(lista_de_espera_t));
	if(!lista_de_espera) return NULL;

	lista_de_espera->urgencias = cola_crear();
	lista_de_espera->regulares = heap_crear(cmp);

	if(!lista_de_espera->urgencias || !lista_de_espera->regulares) {
		lista_de_espera_destruir(lista_de_espera);
		return false;
	}

	lista_de_espera->total_espera = 0;
	lista_de_espera->cmp = cmp;
	lista_de_espera->funcion_destruir = funcion_destruir;
	return lista_de_espera;
}

/*
bool guardar_lista_de_espera(hash_t* listas, char** datos_lista_de_espera) {
	lista_de_espera_t* lista_de_espera = lista_de_espera_crear();
	if(!lista_de_espera) return false;

	if(!hash_guardar(turnos, datos_especialidad[1], lista_de_espera)) {
		lista_de_espera_destruir(lista_de_espera);
		return false;
	}
	return true;
}*/
