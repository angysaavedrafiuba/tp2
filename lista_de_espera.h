#ifndef LISTA_DE_ESPERA
#define LISTA_DE_ESPERA

#include <stdbool.h>
#include <stddef.h>

typedef struct lista_de_espera lista_de_espera_t;

typedef void (*lista_de_espera_destruir_dato_t)(void *);

typedef int (*lista_de_espera_comparar_clave_t)(const void *, const void *);

/*
    Crea una lista de espera vacía, recibe por parámetro la funcion de
    comparación de sus elementos. Devuelve la lista de espera creada, NULL en
    caso contrario.
*/
lista_de_espera_t *
lista_de_espera_crear(lista_de_espera_comparar_clave_t comparar);

/*
	Destruye la lista de espera que recibe por parámetro.
	Pre: La lista fue creada
	Post: La lista fue destruida
*/
void lista_de_espera_destruir(lista_de_espera_t *lista_de_espera);

/*
    Devuelve la cantidad de elementos en la lista de espera.
    Pre: La lista fue creada.
*/
size_t lista_de_espera_cantidad_total(lista_de_espera_t *lista);

/*
    Devuelve true si la lista esta vacía, false en caso contrario.
    Pre: La lista fue creada.
*/
bool lista_de_espera_esta_vacia(lista_de_espera_t *lista_de_espera);

/*
    Guarda un turno en la lista de espera. Recibe por parámetro la prioridad
    del turno y el elemento que será atendido. Pre: La lista fue creada y se
    validó la urgencia.
*/
bool lista_de_espera_guardar(lista_de_espera_t *lista_de_espera, char *urgencia,
                             void *atendido);

/*
    Desencola al siguiente a ser atendido.
    Devuelve NULL en caso de que no hayan más.
    Pre: La lista fue creada.
*/
void *lista_de_espera_desencolar(lista_de_espera_t *lista_de_espera);

#endif