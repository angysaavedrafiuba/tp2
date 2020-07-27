#ifndef DOCTOR
#define DOCTOR

#include <stdbool.h>
#include "abb.h"

typedef struct doctor doctor_t;

doctor_t* doctor_crear(char** datos_doctor);

int doctor_cmp(const char *pac1, const char *pac2);

void doctor_destruir(doctor_t* doctor);

char* doctor_ver_especialidad(doctor_t* doctor);

void doctor_agregar_atendido(doctor_t* doctor);

#endif