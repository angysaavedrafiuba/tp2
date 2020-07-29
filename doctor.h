#ifndef DOCTOR
#define DOCTOR

#include "abb.h"
#include <stdbool.h>

typedef struct doctor doctor_t;

/* Crea un doctor*/
doctor_t* doctor_crear(char** datos_doctor);

/*  Funcion que compara dos doctores por su nombre
    devuelve un numero positivo si doc1 es mayor a doc2,
    un numero negativo si doc2 es mayor a doc1 y 
    devuelve 0 en caso de que sean iguales.
*/
int doctor_cmp(const char *doc1, const char *doc2);

/*  Destruye un doctor y libera el espacio usado*/
void doctor_destruir(doctor_t* doctor);

/*  Devuelve la especialidad del doctor pasado por
    parámetro o NULL en caso de que el doctor no exista.
*/
char* doctor_ver_especialidad(doctor_t* doctor);

/*  Le suma 1 a la cantidad de pacientes atendidos que
    tiene el doctor
    pre: el doctor fue inicializado
*/
void doctor_agregar_atendido(doctor_t* doctor);

/*  Devuelve el nombre del doctor pasado por parámetro
    pre: el doctor fue inicializado
*/
char* doctor_ver_nombre(doctor_t* doctor);

/*  Devuelve la cantidad de pacientes que el doctor 
    atendió hasta el momento
    pre: el doctor fue inicializado
*/
size_t doctor_ver_cant_atendidos(doctor_t* doctor);

#endif