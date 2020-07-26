#define _POSIX_C_SOURCE 200809L
#include "paciente.h"
#include <string.h>
#include <stdlib.h>

struct paciente{
	char* nombre;
	char* ingreso;
};

int paciente_cmp(const char *pac1, const char *pac2){
	return strcmp(pac1, pac2);
}

paciente_t* paciente_crear(char** datos_paciente){
	paciente_t* paciente = malloc(sizeof(paciente_t));

	if (!paciente) return NULL;

	paciente->nombre = strdup(datos_paciente[0]);
	paciente->ingreso = strdup(datos_paciente[1]);

	return paciente;
}

void paciente_destruir(paciente_t* paciente) {
	free(paciente->nombre);
	free(paciente->ingreso);
	free(paciente);
}
/*
bool guardar_paciente(abb_t* pacientes, char** datos) {
	paciente_t* paciente = paciente_crear(datos);
	if(!paciente) return false;

	if(!abb_guardar(pacientes, paciente->ingreso, paciente)) {
		paciente_destruir(paciente);
		return false;
	}
	return true;
}*/