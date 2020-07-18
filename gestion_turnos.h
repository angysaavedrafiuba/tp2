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

#endif