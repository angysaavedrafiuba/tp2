#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gestion_turnos.h"
#include "paciente.h"
#include "doctor.h"
#include "strutil.h"
#include "mensajes.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

typedef struct clinica{
	gestion_turnos_t* gestion_turnos;
}clinica_t;

void clinica_destruir(clinica_t* clinica){
	if(clinica->gestion_turnos){
		gestion_turnos_destruir(clinica->gestion_turnos);
	}
	free(clinica);
}

bool clinica_agregar_paciente(char** datos, void* gestion){
	for(size_t i = 0; i<2; i++ )
		if(!datos[i]) return false;

    char *resto = NULL;
    strtol(datos[1], &resto, 10);
    if(strlen(resto) != 0){
    	printf(ENOENT_ANIO, datos[1]);
    	return false;
    }

    return agregar_atendido((gestion_turnos_t*)gestion, datos,(void * (*)(char **))paciente_crear);
}

bool clinica_agregar_doctor(char** datos, void* gestion){
	for(size_t i = 0; i<2; i++ )
		if(!datos[i]) return false;

    return agregar_atendedor((gestion_turnos_t*)gestion, datos,(void * (*)(char **))doctor_crear);
}

clinica_t* clinica_crear(char* csv_doctores, char* csv_pacientes){
	clinica_t* clinica = malloc(sizeof(clinica_t));
	if(!clinica) return NULL;

	clinica->gestion_turnos = gestion_turnos_crear((void (*)(void *))doctor_destruir, (gestion_turnos_comparar_clave_t)paciente_cmp, (void (*)(void *))paciente_destruir);
	if (!clinica->gestion_turnos){
		free(clinica);
		return NULL;
	}

	if(!csv_cargar_datos(csv_doctores, clinica_agregar_doctor, clinica->gestion_turnos)){
		clinica_destruir(clinica);
		return NULL;
	}

	if(!csv_cargar_datos(csv_pacientes, clinica_agregar_paciente, clinica->gestion_turnos)){
		clinica_destruir(clinica);
		return NULL;
	}

	return clinica;
}

void ejecutar_comando_pedir_turno(clinica_t* clinica, char** parametros){
	int mensaje = pedir_turno(clinica->gestion_turnos, parametros[0], parametros[1], parametros[2]);

	switch(mensaje){
		case 0: break;
		case 1: break;
		case 2: break;
		case 3: break;
	}
}
void ejecutar_comando_atender(clinica_t* clinica, char** parametros){
	doctor_t* doctor = gestion_turnos_obtener_atendedor(clinica->gestion_turnos, parametros[0]);
	char* especialidad = doctor_ver_especialidad(doctor);
	int mensaje = atender_siguiente(clinica->gestion_turnos, parametros[0], especialidad);

	switch(mensaje){
		case 0:
		doctor_agregar_atendido(doctor);
		break;
		case 1: break;
		case 2: break;
		case 3: break;

	}
}


void ejecutar_comando_informe(clinica_t* clinica, char** parametros){
	
}

void ejecutar_comando(clinica_t* clinica, const char* comando, char** parametros){
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		ejecutar_comando_pedir_turno( clinica, parametros);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		ejecutar_comando_atender(clinica, parametros);
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		ejecutar_comando_informe(clinica, parametros);
	}
}

void procesar_comando(clinica_t* clinica, const char* comando, char** parametros) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		for (int i = 0; i < 3; i++){
			if (!parametros[i]){
				printf(ENOENT_PARAMS, comando);
				return;
			}
		}
		ejecutar_comando(clinica, comando, parametros);

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (!parametros[0]){
			printf(ENOENT_PARAMS, comando);
			return;
		}
		ejecutar_comando(clinica, comando, parametros);

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		for (int i = 0; i < 2; i++){
			if (!parametros[i]){
				printf(ENOENT_PARAMS, comando);
				return;
			}
		}
		ejecutar_comando(clinica, comando, parametros);

	} else {
		printf(ENOENT_CMD, comando);
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(clinica_t* clinica) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(clinica, campos[0], parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}


int main(int argc, char** argv) {

	if (argc != 3){
		printf(ENOENT_CANT_PARAMS);
		return 1;
	}

	clinica_t* clinica = clinica_crear(argv[1], argv[2]);
	if (!clinica) return 1;

	procesar_entrada(clinica);

	clinica_destruir(clinica);

	return 0;
}
