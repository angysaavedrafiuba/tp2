#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gestion_turnos.h"
#include "strutil.h"
#include "mensajes.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

void procesar_comando(const char* comando, char** parametros) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		for (int i = 0; i < 3; i++){
			if (!parametros[i]){
				printf(ENOENT_PARAMS, comando);
				return;
			}
		}
		//pedir_turno(parametros[0], parametros[1], parametros[2]);

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (!parametros[0]){
			printf(ENOENT_PARAMS, comando);
			return;
		}
		//atender_siguiente(parametros[0]);

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		for (int i = 0; i < 2; i++){
			if (!parametros[i]){
				printf(ENOENT_PARAMS, comando);
				return;
			}
		}
		//informe_doctores(parametros[0], parametros[1]);

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

	if (argc != 3)
		return 1;

	//gestion_turnos_t* gestion_turnos = gestion_turnos_crear(argv[0], argv[1]);
	procesar_entrada();

	return 0;
}
