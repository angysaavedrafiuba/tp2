#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "csv.h"
#include "mensajes.h"
#define SEPARADOR ','

static void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

bool csv_cargar_datos(const char* ruta_csv, bool (*cargar) (char**, void*), void* extra){
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) {
		printf(ENOENT_ARCHIVO, ruta_csv);
		return false;
	}

	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
		if (!cargar(campos, extra)){
			free_strv(campos);
			free(linea);
			fclose(archivo);
			return false;
		}
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return true;
}
