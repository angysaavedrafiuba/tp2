#ifndef GESTION_TURNOS
#define GESTION_TURNOS

#include "csv.h"
#include <stddef.h>
#include <stdbool.h>

/*---------------------------------------------------
                TIPOS Y ESTRUCTURAS
-----------------------------------------------------*/

typedef struct gestion_turnos gestion_turnos_t;

/*----------------------------------------------------
                PRIMITIVAS DEL ABB
------------------------------------------------------*/

//crea el TDA gestion_turnos
gestion_turnos_t* gestion_turnos_crear(const char* ruta_csv_doctores,const char* ruta_csv_pacientes);

//destruye el TDA gestion_turnos
void gestion_turnos_destruir(gestion_turnos_t* gestion);

//esta funcion imprime
void pedir_turno(char* nombre_paciente, char* especialidad, char* urgencia); //quizá hay que cambiarle el valor de retorno dependiendo de como se impriman los mensajes

//esta funcion imprime
void atender_siguiente(char* nombre_doctor);

//esta funcion imprime
void informe_doctores(char* inicio, char* fin);

#endif