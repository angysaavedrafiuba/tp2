#define _POSIX_C_SOURCE 200809L
#include "gestion_turnos.h"
#include "abb.h"
#include "cola.h"
#include "hash.h"
#include "heap.h"
#include "lista_de_espera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define URGENCIA_1 "URGENTE"
#define URGENCIA_2 "REGULAR"

struct gestion_turnos {
  hash_t *atendidos;
  abb_t *atendedores;
  hash_t *turnos;
  void *atendido_actual;
  gestion_turnos_destruir_dato_t dst_atendido;
  gestion_turnos_destruir_dato_t dst_atendedor;
  gestion_turnos_comparar_clave_t cmp_atendido;
};

void gestion_turnos_destruir(gestion_turnos_t *gestion_turnos) {
  if (gestion_turnos->atendedores)
    abb_destruir(gestion_turnos->atendedores);
  if (gestion_turnos->atendidos)
    hash_destruir(gestion_turnos->atendidos);
  if (gestion_turnos->turnos)
    hash_destruir(gestion_turnos->turnos);
  free(gestion_turnos);
}

// crea el TDA gestion_turnos
gestion_turnos_t *
gestion_turnos_crear(gestion_turnos_destruir_dato_t dst_atendedor,
                     gestion_turnos_comparar_clave_t cmp_atendido,
                     gestion_turnos_destruir_dato_t dst_atendido) {
  gestion_turnos_t *gestion_turnos = malloc(sizeof(gestion_turnos_t));
  if (!gestion_turnos)
    return NULL;

  gestion_turnos->atendedores = abb_crear(strcmp, dst_atendedor);
  gestion_turnos->atendidos = hash_crear((hash_destruir_dato_t)dst_atendido);
  gestion_turnos->turnos =
      hash_crear((hash_destruir_dato_t)lista_de_espera_destruir);
  gestion_turnos->atendido_actual = NULL;
  gestion_turnos->dst_atendido = dst_atendido;
  gestion_turnos->dst_atendedor = dst_atendedor;
  gestion_turnos->cmp_atendido = cmp_atendido;

  if (!gestion_turnos->atendedores || !gestion_turnos->atendidos ||
      !gestion_turnos->turnos) {
    gestion_turnos_destruir(gestion_turnos);
    return NULL;
  }

  return gestion_turnos;
}

size_t cantidad_atendidos_en_espera(gestion_turnos_t *gestion_turnos,
                                    char *especialidad) {
  lista_de_espera_t *lista = hash_obtener(gestion_turnos->turnos, especialidad);

  return lista_de_espera_cantidad_total(lista);
}

bool agregar_atendido(gestion_turnos_t *gestion_turnos, char **datos,
                      void *(*constructor)(char **)) {
  void *dato = constructor(datos);
  if (!dato)
    return false;
  return hash_guardar(gestion_turnos->atendidos, datos[0], dato);
}

bool agregar_atendedor(gestion_turnos_t *gestion_turnos, char **datos,
                       void *(*constructor)(char **)) {
  void *dato = constructor(datos);
  if (!dato)
    return false;
  if (!hash_guardar(gestion_turnos->turnos, datos[1],
                    lista_de_espera_crear(gestion_turnos->cmp_atendido))) {
    gestion_turnos->dst_atendedor(dato);
    return false;
  }
  return abb_guardar(gestion_turnos->atendedores, datos[0], dato);
}

void *gestion_turnos_obtener_atendedor(gestion_turnos_t *gestion_turnos,
                                       char *nombre) {
  return abb_obtener(gestion_turnos->atendedores, nombre);
}

bool pedir_turno(gestion_turnos_t *gestion_turnos, char *nombre,
                 char *categoria, char *prioridad) {
  hash_t *pacientes = gestion_turnos->atendidos;
  hash_t *turnos = gestion_turnos->turnos;

  void *paciente = hash_obtener(pacientes, nombre);
  lista_de_espera_t *lista_de_espera = hash_obtener(turnos, categoria);

  return lista_de_espera_guardar(lista_de_espera, prioridad, paciente);
}

bool atender_siguiente(gestion_turnos_t *gestion_turnos, char *nombre,
                       char *categoria) {
  hash_t *turnos = gestion_turnos->turnos;
  lista_de_espera_t *lista_de_espera = hash_obtener(turnos, categoria);

  gestion_turnos->atendido_actual = lista_de_espera_desencolar(lista_de_espera);

  return gestion_turnos->atendido_actual;
}

lista_t *informe_atendedores(gestion_turnos_t *gestion_turnos, char *inicio,
                             char *fin) {
  return abb_ver_lista(gestion_turnos->atendedores, inicio, fin);
}

bool atendido_existe(gestion_turnos_t *gestion_turnos, char *nombre) {
  return hash_pertenece(gestion_turnos->atendidos, nombre);
}

bool categoria_existe(gestion_turnos_t *gestion_turnos, char *categoria) {
  return hash_pertenece(gestion_turnos->turnos, categoria);
}

bool prioridad_existe(gestion_turnos_t *gestion_turnos, char *prioridad) {
  return !strcmp(prioridad, URGENCIA_1) || !strcmp(prioridad, URGENCIA_2);
}

void *obtener_atendido_actual(gestion_turnos_t *gestion_turnos) {
  return gestion_turnos->atendido_actual;
}

size_t cantidad_atendedores(gestion_turnos_t *gestion_turnos) {
  return abb_cantidad(gestion_turnos->atendedores);
}