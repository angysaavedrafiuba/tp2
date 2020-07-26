#define _POSIX_C_SOURCE 200809L
#include "gestion_turnos.h"
#include "lista_de_espera.h"
#include "abb.h"
#include "hash.h"
#include "heap.h"
#include "cola.h"
//#include "mensajes.h"
#include <stdlib.h>
#include <string.h>

struct gestion_turnos{
	abb_t* atendidos;
	abb_t* atendedores;
	hash_t* turnos;
	void* atendido_actual;
};

void gestion_turnos_destruir(gestion_turnos_t* gestion_turnos) {
	if(gestion_turnos->atendedores)
		abb_destruir(gestion_turnos->atendedores);
	if(gestion_turnos->atendidos)
		abb_destruir(gestion_turnos->atendidos);
	if(gestion_turnos->turnos)
		hash_destruir(gestion_turnos->turnos);
	free(gestion_turnos);
}

//crea el TDA gestion_turnos
gestion_turnos_t* gestion_turnos_crear(gestion_turnos_comparar_clave_t cmp_atendedor,
										gestion_turnos_destruir_dato_t dst_atendedor,
										gestion_turnos_comparar_clave_t cmp_atendido,
										gestion_turnos_destruir_dato_t dst_atendido){
	gestion_turnos_t* gestion_turnos = malloc(sizeof(gestion_turnos_t));
	if(!gestion_turnos) return NULL;

	gestion_turnos->atendedores = abb_crear(cmp_atendedor, dst_atendedor);
	gestion_turnos->atendidos = abb_crear(cmp_atendido, dst_atendido);
	gestion_turnos->turnos = hash_crear((hash_destruir_dato_t)lista_de_espera_destruir);
	gestion_turnos->atendido_actual = NULL;

	if(!gestion_turnos->atendedores || !gestion_turnos->atendidos || !gestion_turnos->turnos) {
		gestion_turnos_destruir(gestion_turnos);
		return NULL;
	}

	return gestion_turnos;
}

size_t cantidad_atendidos_en_espera(gestion_turnos_t* gestion_turnos, char* especialidad){
	lista_de_espera_t* lista = hash_obtener(gestion_turnos->turnos, especialidad);

	return lista_de_espera_cantidad_total(lista);
}

bool agregar_atendido(gestion_turnos_t* gestion_turnos, char** datos, void* (*constructor) (char**)){
	void* dato = constructor(datos);
	if (!dato) return false;
	return abb_guardar(gestion_turnos->atendidos, datos[0], dato);
}

bool agregar_atendedor(gestion_turnos_t* gestion_turnos, char** datos, void* (*constructor) (char**)){
	void* dato = constructor(datos);
	if (!dato) return false;
	return abb_guardar(gestion_turnos->atendedores, datos[0], dato);
}

/*
gestion_turnos_t* gestion_turnos_crear(const char* ruta_csv_atendedores,const char* ruta_csv_atendidos){
	gestion_turnos_t* gestion_turnos = malloc(sizeof(gestion_turnos_t));
	if(!gestion_turnos) return NULL;

	gestion_turnos->atendedores = abb_crear(strcmp, doctor_destruir);
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
}*/
