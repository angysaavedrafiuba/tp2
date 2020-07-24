#ifndef LISTA_DE_ESPERA
#define LISTA_DE_ESPERA

#include <stdbool.h>
#include "cola.h"
#include "heap.h"
#include "hash.h"

typedef struct lista_de_espera lista_de_espera_t;

typedef void (*lista_de_espera_destruir_dato_t)(void *);

typedef int (*lista_de_espera_comparar_clave_t) (const void *, const void *);

lista_de_espera_t* lista_de_espera_crear(lista_de_espera_comparar_clave_t comparar, lista_de_espera_destruir_dato_t destruir);

void lista_de_espera_destruir();

bool guardar_lista_de_espera(hash_t* listas_de_espera, char** datos_lista_de_espera);

#endif