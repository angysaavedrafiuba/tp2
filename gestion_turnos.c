#define _POSIX_C_SOURCE 200809L
#include "gestion_turnos.h"
#include "lista_de_espera.h"
#include "abb.h"
#include "hash.h"
#include "heap.h"
#include "cola.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define URGENCIA_1 "URGENTE"
#define URGENCIA_2 "REGULAR"

struct gestion_turnos{
	abb_t* atendidos;
	abb_t* atendedores;
	hash_t* turnos;
	void* atendido_actual;
	gestion_turnos_destruir_dato_t dst_atendido;
	gestion_turnos_destruir_dato_t dst_atendedor;
	gestion_turnos_comparar_clave_t cmp_atendido;
};

bool imprimir(const char *clave, void *dato, void *extra){
	printf("CLAVE EN ABB: %s\n", clave);
	return true;
}

void gestion_turnos_destruir(gestion_turnos_t* gestion_turnos) {
	abb_in_order(gestion_turnos->atendedores, imprimir, NULL);
	abb_in_order(gestion_turnos->atendidos, imprimir, NULL);

	hash_iter_t* iter = hash_iter_crear(gestion_turnos->turnos);

	while(!hash_iter_al_final(iter)){
		printf("HASH: %s\n", hash_iter_ver_actual(iter));
		hash_iter_avanzar(iter);
	}

	hash_iter_destruir(iter);

	if(gestion_turnos->atendedores)
		abb_destruir(gestion_turnos->atendedores);
	if(gestion_turnos->atendidos)
		abb_destruir(gestion_turnos->atendidos);
	if(gestion_turnos->turnos)
		hash_destruir(gestion_turnos->turnos);
	free(gestion_turnos);
}

//crea el TDA gestion_turnos
gestion_turnos_t* gestion_turnos_crear(gestion_turnos_destruir_dato_t dst_atendedor,
										gestion_turnos_comparar_clave_t cmp_atendido,
										gestion_turnos_destruir_dato_t dst_atendido){
	gestion_turnos_t* gestion_turnos = malloc(sizeof(gestion_turnos_t));
	if(!gestion_turnos) return NULL;

	gestion_turnos->atendedores = abb_crear(strcmp, dst_atendedor);
	gestion_turnos->atendidos = abb_crear(strcmp, dst_atendido);
	gestion_turnos->turnos = hash_crear((hash_destruir_dato_t)lista_de_espera_destruir);
	gestion_turnos->atendido_actual = NULL;
	gestion_turnos->dst_atendido = dst_atendido;
	gestion_turnos->dst_atendedor = dst_atendedor;
	gestion_turnos->cmp_atendido = cmp_atendido;

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
	if (!hash_guardar(gestion_turnos->turnos, datos[1], lista_de_espera_crear(gestion_turnos->cmp_atendido, gestion_turnos->dst_atendido))){
		gestion_turnos->dst_atendedor(dato);
		return false;
	}
	return abb_guardar(gestion_turnos->atendedores, datos[0], dato);
}

int pedir_turno(gestion_turnos_t* gestion_turnos, char** parametros) {
	abb_t* pacientes = gestion_turnos->atendidos;
	hash_t* turnos = gestion_turnos->turnos;

	char* nombre_paciente = parametros[0];
	char* especialidad_paciente = parametros[1];
	char* urgencia_paciente = parametros[2];

	if(!abb_pertenece(pacientes, nombre_paciente))
		return 1;
	if(!hash_pertenece(turnos, especialidad_paciente))
		return 2;
	if(strcmp(urgencia_paciente, URGENCIA_1) != 0 && strcmp(urgencia_paciente, URGENCIA_2) != 0)
		return 3;
	
	void* paciente = abb_obtener(pacientes, nombre_paciente);
	if(!paciente) return -1;

	lista_de_espera_t* lista_de_espera = hash_obtener(turnos, especialidad_paciente);
	if(!lista_de_espera_guardar(lista_de_espera, urgencia_paciente, paciente))
		return -1;

	return 0;
}

int atender_siguiente(gestion_turnos_t* gestion_turnos, char** parametros) {
	char* doctor = parametros[0];
	char* especialidad = doctor_ver_especialidad(doctor); // a implementar en doctor.c
	abb_t* doctores = gestion_turnos->atendedores;
	hash_t* turnos = gestion_turnos->turnos;
	lista_de_espera_t* lista_de_espera = hash_obtener(turnos, especialidad);


	if(!abb_pertenece(doctores, doctor))
		return 1;
	if(lista_de_espera_esta_vacia(lista_de_espera))
		return 2;
	
	gestion_turnos->atendido_actual = lista_de_espera_desencolar(lista_de_espera);
	doctor_agregar_atendido(doctor); // a implementar en doctor.c
	return 0;
}

