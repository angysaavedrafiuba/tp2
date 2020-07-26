#ifndef GESTION_TURNOS
#define GESTION_TURNOS

#include "csv.h"
#include <stddef.h>
#include <stdbool.h>

/*---------------------------------------------------
                TIPOS Y ESTRUCTURAS
-----------------------------------------------------*/

typedef struct gestion_turnos gestion_turnos_t;

typedef void (*gestion_turnos_destruir_dato_t)(void *);

typedef int (*gestion_turnos_comparar_clave_t) (const void *, const void *);
/*----------------------------------------------------
                PRIMITIVAS DE GESTION TURNOS
------------------------------------------------------*/

//crea el TDA gestion_turnos
gestion_turnos_t* gestion_turnos_crear(gestion_turnos_destruir_dato_t dst_atendedor, gestion_turnos_comparar_clave_t cmp_atendido, gestion_turnos_destruir_dato_t dst_atendido);

void gestion_turnos_destruir(gestion_turnos_t* gestion_turnos);

size_t cantidad_pacientes_en_espera(gestion_turnos_t* gestion_turnos, char* especialidad);

bool agregar_atendido(gestion_turnos_t* gestion_turnos, char** datos, void* (*constructor) (char**));

bool agregar_atendedor(gestion_turnos_t* gestion_turnos, char** datos, void* (*constructor) (char**));

// devuelve 0 si se pudo pedir el turno, 1 si el paciente no existe, 2 si no existe la especialidad,
// devuelve 3 si no se identifica el grado de urgencia y si no se pudo guardar devuelve -1
int pedir_turno(gestion_turnos_t* gestion_turnos, char** parametros);

// devuelve 0 si se pudo atender otro paciente, 1 en caso de que el doctor no exista
// y devuelve 2 si no hay pacientes para atender
int atender_siguiente(gestion_turnos_t* gestion_turnos, char** parametros);

//int informe_doctores(char* inicio, char* fin);

#endif