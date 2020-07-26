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

//int pedir_turno(char* nombre_paciente, char* especialidad, char* urgencia); //quizá hay que cambiarle el valor de retorno dependiendo de como se impriman los mensajes

//int atender_siguiente(char* nombre_doctor);

//int informe_doctores(char* inicio, char* fin);

#endif