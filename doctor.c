#define _POSIX_C_SOURCE 200809L
#include "doctor.h"
#include <stdlib.h>
#include <string.h>

struct doctor {
  char *nombre;
  char *especialidad;
  size_t pacientes_atendidos;
};

int doctor_cmp(const char *doc1, const char *doc2) {
  return strcmp(doc1, doc2);
}

doctor_t *doctor_crear(char **datos_doctor) {
  doctor_t *doctor = malloc(sizeof(doctor_t));

  if (!doctor)
    return NULL;

  doctor->nombre = strdup(datos_doctor[0]);
  doctor->especialidad = strdup(datos_doctor[1]);
  doctor->pacientes_atendidos = 0;

  return doctor;
}

void doctor_destruir(doctor_t *doctor) {
  free(doctor->nombre);
  free(doctor->especialidad);
  free(doctor);
}

char *doctor_ver_especialidad(doctor_t *doctor) {
  if (!doctor)
    return NULL;

  return doctor->especialidad;
}

void doctor_agregar_atendido(doctor_t *doctor) {
  doctor->pacientes_atendidos++;
}

char *doctor_ver_nombre(doctor_t *doctor) { return doctor->nombre; }

size_t doctor_ver_cant_atendidos(doctor_t *doctor) {
  return doctor->pacientes_atendidos;
}