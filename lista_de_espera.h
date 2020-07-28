#ifndef LISTA_DE_ESPERA
#define LISTA_DE_ESPERA

#include <stddef.h>
#include <stdbool.h>

typedef struct lista_de_espera lista_de_espera_t;

typedef void (*lista_de_espera_destruir_dato_t)(void *);

typedef int (*lista_de_espera_comparar_clave_t) (const void *, const void *);

lista_de_espera_t* lista_de_espera_crear(lista_de_espera_comparar_clave_t comparar);

void lista_de_espera_destruir(lista_de_espera_t* lista_de_espera);

size_t lista_de_espera_cantidad_total(lista_de_espera_t* lista);

// devuelve true si la lista esta vacía, false en caso contrario
bool lista_de_espera_esta_vacia(lista_de_espera_t* lista_de_espera);

// guarda un nuevo turno en la lista de espera
// pre: se validó la urgencia previamente
bool lista_de_espera_guardar(lista_de_espera_t* lista_de_espera, char* urgencia, void* atendido);

// desencola al siguiente a ser atendido, devuelve NULL en caso de que no hayan más
void* lista_de_espera_desencolar(lista_de_espera_t* lista_de_espera);

#endif