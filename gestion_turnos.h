#ifndef GESTION_TURNOS
#define GESTION_TURNOS

#include "csv.h"
#include <stdbool.h>
#include <stddef.h>

/*---------------------------------------------------
                TIPOS Y ESTRUCTURAS
-----------------------------------------------------*/

typedef struct gestion_turnos gestion_turnos_t;

typedef void (*gestion_turnos_destruir_dato_t)(void *);

typedef int (*gestion_turnos_comparar_clave_t)(const void *, const void *);
/*----------------------------------------------------
                PRIMITIVAS DE GESTION TURNOS
------------------------------------------------------*/

// crea el TDA gestion_turnos
gestion_turnos_t *
gestion_turnos_crear(gestion_turnos_destruir_dato_t dst_atendedor,
                     gestion_turnos_comparar_clave_t cmp_atendido,
                     gestion_turnos_destruir_dato_t dst_atendido);

void gestion_turnos_destruir(gestion_turnos_t *gestion_turnos);

size_t cantidad_atendidos_en_espera(gestion_turnos_t *gestion_turnos,
                                    char *especialidad);

bool agregar_atendido(gestion_turnos_t *gestion_turnos, char **datos,
                      void *(*constructor)(char **));

bool agregar_atendedor(gestion_turnos_t *gestion_turnos, char **datos,
                       void *(*constructor)(char **));

void *gestion_turnos_obtener_atendedor(gestion_turnos_t *gestion_turnos,
                                       char *nombre);

bool pedir_turno(gestion_turnos_t *gestion_turnos, char *nombre,
                 char *categoria, char *prioridad);

bool atender_siguiente(gestion_turnos_t *gestion_turnos, char *nombre,
                       char *categoria);

lista_t *informe_atendedores(gestion_turnos_t *gestion_turnos, char *inicio,
                             char *fin);

bool atendido_existe(gestion_turnos_t *gestion_turnos, char *nombre);

bool categoria_existe(gestion_turnos_t *gestion_turnos, char *categoria);

bool prioridad_existe(gestion_turnos_t *gestion_turnos, char *prioridad);

void *obtener_atendido_actual(gestion_turnos_t *gestion_turnos);

size_t cantidad_atendedores(gestion_turnos_t *gestion_turnos);
#endif