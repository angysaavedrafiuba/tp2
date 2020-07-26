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
		printf("ENTRA EN DESTRUCCION DE GETSION TURNOS\n");
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
	printf("ENTRA A AGREGAR UN DOCTOR \n");
	for(size_t i = 0; i<2; i++ )
		if(!datos[i]) return false;

	printf("VALIDA LOS DATOS OK Y LLAMA A AGREGAR ATENDEDOR\n");

    return agregar_atendedor((gestion_turnos_t*)gestion, datos,(void * (*)(char **))doctor_crear);
}

clinica_t* clinica_crear(char* csv_doctores, char* csv_pacientes){
	clinica_t* clinica = malloc(sizeof(clinica_t));
	if(!clinica) return NULL;

	clinica->gestion_turnos = gestion_turnos_crear(doctor_cmp, (void (*)(void *))doctor_destruir, strcmp, (void (*)(void *))paciente_destruir);
	if (!clinica->gestion_turnos){
		free(clinica);
		return NULL;
	}

	printf("en crear clinica antes de cargar los archivos csv\n");
	if(!csv_cargar_datos(csv_doctores, clinica_agregar_doctor, clinica->gestion_turnos)){
		printf("no pudo cargar la primera tanda de datos\n");
		clinica_destruir(clinica);
		return NULL;
	}
	printf("Creado el primer archivo, antes de crear el segundo\n");
	if(!csv_cargar_datos(csv_pacientes, clinica_agregar_paciente, clinica->gestion_turnos)){
		clinica_destruir(clinica);
		return NULL;
	}

	return clinica;
}

void ejecutar_comando(const char* comando, char** parametros){
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		
	}
}

void procesar_comando(const char* comando, char** parametros) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		for (int i = 0; i < 3; i++){
			if (!parametros[i]){
				printf(ENOENT_PARAMS, comando);
				return;
			}
		}
		ejecutar_comando(comando, parametros);

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (!parametros[0]){
			printf(ENOENT_PARAMS, comando);
			return;
		}
		ejecutar_comando(comando, parametros);

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		for (int i = 0; i < 2; i++){
			if (!parametros[i]){
				printf(ENOENT_PARAMS, comando);
				return;
			}
		}
		ejecutar_comando(comando, parametros);

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

void procesar_entrada() {
	printf("PROCESANDO ENTRADA\n");
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
		procesar_comando(campos[0], parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}


int main(int argc, char** argv) {

	printf("Comienza el programa\n");
	if (argc != 3){
		printf(ENOENT_CANT_PARAMS);
		return 1;
	}

	printf("ARCHIVO 1: %s\n",argv[1] );
	printf("ARCHIVO 2: %s\n",argv[2] );
	printf("ANTES DE crear clinica\n");
	clinica_t* clinica = clinica_crear(argv[1], argv[2]);
	if (!clinica) return 1;

	procesar_entrada();

	printf("ANTES DE DESTRUIR\n");
	clinica_destruir(clinica);

	return 0;
}
