#define _POSIX_C_SOURCE 200809L
#include "doctor.h"
#include "gestion_turnos.h"
#include "mensajes.h"
#include "paciente.h"
#include "strutil.h"
#include "csv.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

typedef struct clinica {
  gestion_turnos_t *gestion_turnos;
} clinica_t;

void clinica_destruir(clinica_t *clinica) {
  if (clinica->gestion_turnos) {
    gestion_turnos_destruir(clinica->gestion_turnos);
  }
  free(clinica);
}

bool clinica_agregar_paciente(char **datos, void *gestion) {
  for (size_t i = 0; i < 2; i++)
    if (!datos[i])
      return false;

  char *resto = NULL;
  strtol(datos[1], &resto, 10);
  if (strlen(resto) != 0) {
    printf(ENOENT_ANIO, datos[1]);
    return false;
  }

  return agregar_atendido((gestion_turnos_t *)gestion, datos,
                          (void *(*)(char **))paciente_crear);
}

bool clinica_agregar_doctor(char **datos, void *gestion) {
  for (size_t i = 0; i < 2; i++){
    if (!datos[i])
      return false;
  }

  return agregar_atendedor((gestion_turnos_t *)gestion, datos,
                           (void *(*)(char **))doctor_crear);
}

clinica_t *clinica_crear(char *csv_doctores, char *csv_pacientes) {
  clinica_t *clinica = malloc(sizeof(clinica_t));
  if (!clinica)
    return NULL;

  clinica->gestion_turnos =
      gestion_turnos_crear((void (*)(void *))doctor_destruir,
                           (gestion_turnos_comparar_clave_t)paciente_cmp,
                           (void (*)(void *))paciente_destruir);
  if (!clinica->gestion_turnos) {
    free(clinica);
    return NULL;
  }

  if (!csv_cargar_datos(csv_doctores, clinica_agregar_doctor,
                        clinica->gestion_turnos)) {
    clinica_destruir(clinica);
    return NULL;
  }

  if (!csv_cargar_datos(csv_pacientes, clinica_agregar_paciente,
                        clinica->gestion_turnos)) {
    clinica_destruir(clinica);
    return NULL;
  }

  return clinica;
}

void ejecutar_comando_pedir_turno(clinica_t *clinica, char **parametros) {
  bool paciente_existe =
      atendido_existe(clinica->gestion_turnos, parametros[0]);
  bool especialidad_existe =
      categoria_existe(clinica->gestion_turnos, parametros[1]);
  bool urgencia_existe =
      prioridad_existe(clinica->gestion_turnos, parametros[2]);

  if (paciente_existe && especialidad_existe && urgencia_existe) {
    bool turno = pedir_turno(clinica->gestion_turnos, parametros[0], parametros[1],
                        parametros[2]);
    if (turno) {
      printf(PACIENTE_ENCOLADO, parametros[0]);
      size_t cant =
          cantidad_atendidos_en_espera(clinica->gestion_turnos, parametros[1]);
      printf(CANT_PACIENTES_ENCOLADOS, cant, parametros[1]);
    }
  } else {
    if (!paciente_existe)
      printf(ENOENT_PACIENTE, parametros[0]);
    if (!especialidad_existe)
      printf(ENOENT_ESPECIALIDAD, parametros[1]);
    if (!urgencia_existe)
      printf(ENOENT_URGENCIA, parametros[2]);
  }
}

void ejecutar_comando_atender(clinica_t *clinica, char **parametros) {
  doctor_t *doctor =
      gestion_turnos_obtener_atendedor(clinica->gestion_turnos, parametros[0]);
  char *especialidad = doctor_ver_especialidad(doctor);
  bool atender = false;

  if (doctor) {
    atender =
        atender_siguiente(clinica->gestion_turnos, parametros[0], especialidad);
    if (atender) {
      doctor_agregar_atendido(doctor);
      paciente_t *paciente = obtener_atendido_actual(clinica->gestion_turnos);
      char *nombre = obtener_nombre_paciente(paciente);
      printf(PACIENTE_ATENDIDO, nombre);
      size_t cant =
          cantidad_atendidos_en_espera(clinica->gestion_turnos, especialidad);
      printf(CANT_PACIENTES_ENCOLADOS, cant, especialidad);
    } else
      printf(SIN_PACIENTES);
  } else
    printf(ENOENT_DOCTOR, parametros[0]);
}

void ejecutar_comando_informe(clinica_t *clinica, char **parametros) {
  lista_t *lista = informe_atendedores(clinica->gestion_turnos, parametros[0],
                                       parametros[1]);

  printf(DOCTORES_SISTEMA, lista_largo(lista));

  lista_iter_t *iter = lista_iter_crear(lista);
  size_t i = 1;
  while (!lista_iter_al_final(iter)) {
    doctor_t *doctor = lista_iter_ver_actual(iter);
    printf(INFORME_DOCTOR, i, doctor_ver_nombre(doctor),
           doctor_ver_especialidad(doctor), doctor_ver_cant_atendidos(doctor));
    lista_iter_avanzar(iter);
    i++;
  }

  lista_iter_destruir(iter);
  lista_destruir(lista, NULL);
}

bool check_command_params(const char *cmd, char** parametros, size_t cantidad){
  for (size_t i = 0; i < cantidad; i++) {
      if (!parametros[i]) {
        printf(ENOENT_PARAMS, cmd);
        return false;
      }
  }
  return true;
}

void procesar_comando(clinica_t *clinica, const char *comando,
                      char **parametros) {
  if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
    if(!check_command_params(comando, parametros, 3))
        return;
    ejecutar_comando_pedir_turno(clinica, parametros);

  } else if (strcmp(comando, COMANDO_ATENDER) == 0) {
    if(!check_command_params(comando, parametros, 1))
        return;
    ejecutar_comando_atender(clinica, parametros);

  } else if (strcmp(comando, COMANDO_INFORME) == 0) {
    if(!check_command_params(comando, parametros, 2))
        return;
    ejecutar_comando_informe(clinica, parametros);

  } else {
    printf(ENOENT_CMD, comando);
  }
}

void eliminar_fin_linea(char *linea) {
  size_t len = strlen(linea);
  if (linea[len - 1] == '\n') {
    linea[len - 1] = '\0';
  }
}

void procesar_entrada(clinica_t *clinica) {
  char *linea = NULL;
  size_t c = 0;
  while (getline(&linea, &c, stdin) > 0) {
    eliminar_fin_linea(linea);
    char **campos = split(linea, ':');
    if (campos[1] == NULL) {
      printf(ENOENT_FORMATO, linea);
      free_strv(campos);
      continue;
    }
    char **parametros = split(campos[1], ',');
    procesar_comando(clinica, campos[0], parametros);
    free_strv(parametros);
    free_strv(campos);
  }
  free(linea);
}

int main(int argc, char **argv) {

  if (argc != 3) {
    printf(ENOENT_CANT_PARAMS);
    return 1;
  }

  clinica_t *clinica = clinica_crear(argv[1], argv[2]);
  if (!clinica) {
    return 1;
  }

  procesar_entrada(clinica);

  clinica_destruir(clinica);

  return 0;
}
