#define _POSIX_C_SOURCE 200809L
#include "gestion_turnos.h"
#include "paciente.h"
#include "abb.h"
#include "hash.h"
#include "heap.h"
#include "cola.h"
#include "mensajes.h"
#include <stdlib.h>
#include <string.h>

struct gestion_turnos{
	abb_t* pacientes;
	abb_t* doctores;
	hash_t* turnos;
};

void gestion_turnos_destruir(gestion_turnos_t* gestion_turnos) {
	if(gestion_turnos->doctores)
		abb_destruir(gestion_turnos->doctores);
	if(gestion_turnos->pacientes)
		abb_destruir(gestion_turnos->pacientes);
	if(gestion_turnos->turnos)
		hash_destruir(gestion_turnos->turnos);
	free(gestion_turnos);
}

gestion_turnos_t* gestion_turnos_crear(const char* ruta_csv_doctores,const char* ruta_csv_pacientes){
	gestion_turnos_t* gestion_turnos = malloc(sizeof(gestion_turnos_t));
	if(!gestion_turnos) return NULL;

	gestion_turnos->doctores = abb_crear(strcmp, doctor_destruir);
	gestion_turnos->pacientes = abb_crear(strcmp, paciente_destruir);
	gestion_turnos->turnos = hash_crear(lista_de_espera_destruir);

	if(!gestion_turnos->doctores || !gestion_turnos->pacientes || !gestion_turnos->turnos) {
		gestion_turnos_destruir(gestion_turnos);
		return NULL;
	}

	bool doctores_ok = csv_guardar_datos(ruta_csv_doctores, gestion_turnos->doctores, guardar_doctor);
	bool pacientes_ok = csv_guardar_datos(ruta_csv_pacientes, gestion_turnos->pacientes, guardar_paciente);
	bool listas_de_espera_ok = csv_guardar_datos(ruta_csv_doctores, gestion_turnos->turnos, guardar_lista_de_espera);

	if(!doctores_ok || !pacientes_ok || !listas_de_espera_ok) {
		gestion_turnos_destruir(gestion_turnos);
		return NULL;
	}
	
	return gestion_turnos;
}
