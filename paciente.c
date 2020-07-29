#define _POSIX_C_SOURCE 200809L
#include "paciente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct paciente {
  char *nombre;
  int ingreso;
};

int paciente_cmp(const paciente_t *pac1, const paciente_t *pac2) {
  if (pac1->ingreso > pac2->ingreso)
    return -1;
  if (pac1->ingreso < pac2->ingreso)
    return 1;
  return 0;
}

paciente_t *paciente_crear(char **datos_paciente) {
  paciente_t *paciente = malloc(sizeof(paciente_t));

  if (!paciente)
    return NULL;

  paciente->nombre = strdup(datos_paciente[0]);
  char *resto = NULL;
  long ingreso_paciente = strtol(datos_paciente[1], &resto, 10);
  paciente->ingreso = (int)ingreso_paciente;

  return paciente;
}

void paciente_destruir(paciente_t *paciente) {
  free(paciente->nombre);
  free(paciente);
}

char *obtener_nombre_paciente(paciente_t *paciente) { return paciente->nombre; }
