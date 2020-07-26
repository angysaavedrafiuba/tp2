#ifndef PACIENTE
#define PACIENTE

#include <stdbool.h>
//#include "abb.h"

typedef struct paciente paciente_t;

paciente_t* paciente_crear(char** datos_paciente);

int paciente_cmp(const char *pac1, const char *pac2);

void paciente_destruir(paciente_t* paciente);

//bool guardar_paciente(abb_t* pacientes, char** datos_paciente);

#endif