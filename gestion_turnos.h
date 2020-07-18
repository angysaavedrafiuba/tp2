#ifndef GESTION_TURNOS
#define GESTION_TURNOS

#include <csv.h>
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

void pedir_turno(char* nombre_paciente, char* especialidad, char* urgencia); //quizá hay que cambiarle el valor de retorno dependiendo de como se impriman los mensajes

void atender_siguiente(char* nombre_doctor);

void imprimir_informe_doctores(char* inicio, char* fin);

#endif