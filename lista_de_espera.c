#define _POSIX_C_SOURCE 200809L
#include "lista_de_espera.h"
#include "cola.h"
#include "heap.h"
#include <stdlib.h>
#include <string.h>
#define URGENCIA_1 "URGENTE"
#define URGENCIA_2 "REGULAR"

struct lista_de_espera {
  cola_t *urgencias;
  heap_t *regulares;
  size_t total_espera;
  lista_de_espera_comparar_clave_t cmp;
};

void lista_de_espera_destruir(lista_de_espera_t *lista) {
  if (lista->urgencias)
    cola_destruir(lista->urgencias, NULL);
  if (lista->regulares)
    heap_destruir(lista->regulares, NULL);
  free(lista);
}

size_t lista_de_espera_cantidad_total(lista_de_espera_t *lista) {
  return lista->total_espera;
}

lista_de_espera_t *lista_de_espera_crear(lista_de_espera_comparar_clave_t cmp) {
  lista_de_espera_t *lista_de_espera = malloc(sizeof(lista_de_espera_t));
  if (!lista_de_espera)
    return NULL;

  lista_de_espera->urgencias = cola_crear();
  lista_de_espera->regulares = heap_crear(cmp);

  if (!lista_de_espera->urgencias || !lista_de_espera->regulares) {
    lista_de_espera_destruir(lista_de_espera);
    return NULL;
  }

  lista_de_espera->total_espera = 0;
  lista_de_espera->cmp = cmp;
  return lista_de_espera;
}

bool lista_de_espera_esta_vacia(lista_de_espera_t *lista_de_espera) {
  return lista_de_espera->total_espera == 0;
}

bool lista_de_espera_guardar(lista_de_espera_t *lista_de_espera, char *urgencia,
                             void *atendido) {
  lista_de_espera->total_espera++;
  if (strcmp(urgencia, URGENCIA_1) == 0)
    return cola_encolar(lista_de_espera->urgencias, atendido);
  else
    return heap_encolar(lista_de_espera->regulares, atendido);
}

void *lista_de_espera_desencolar(lista_de_espera_t *lista_de_espera) {
  if (lista_de_espera_esta_vacia(lista_de_espera))
    return NULL;

  lista_de_espera->total_espera--;
  if (!cola_esta_vacia(lista_de_espera->urgencias))
    return cola_desencolar(lista_de_espera->urgencias);
  else
    return heap_desencolar(lista_de_espera->regulares);
}
