#ifndef DOCTOR
#define DOCTOR

#include <stdbool.h>
#include "abb.h"

typedef struct doctor doctor_t;

doctor_t* doctor_crear(char** datos_doctor);

void doctor_destruir(doctor_t* doctor);

bool guardar_doctor(abb_t* doctores, char** datos_doctor);

#endif