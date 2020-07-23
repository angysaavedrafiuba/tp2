#define _POSIX_C_SOURCE 200809L
#include "gestion_turnos.h"
#include "abb.h"
#include "hash.h"
#include "heap.h"
#include "cola.h"
#include <stdlib.h>
#include <string.h>

typedef struct paciente{
	char* nombre;
	char* ingreso;
}paciente_t;

typedef struct doctor{
	char* nombre;
	char* especialidad;
	size_t pacientes_atendidos;
}doctor_t;

typedef struct lista_de_espera{
	cola_t* urgencias;
	heap_t* regulares;
	size_t total_pacientes;
}lista_de_espera_t;

struct gestion_turnos{
	abb_t* pacientes;
	abb_t* doctores;
	hash_t* turnos;
};

int doctor_cmp(const char* doc1, const char* doc2){
	return strcmp(doc1, doc2);
}

int paciente_cmp(const void *pac1, const void *pac2){
	if(((paciente_t*)pac1)->ingreso < ((paciente_t*)pac2)->ingreso)
		return -1;
	if(((paciente_t*)pac1)->ingreso > ((paciente_t*)pac2)->ingreso)
		return 1;
	return 0;
}

void paciente_destruir(void* _paciente) {
	paciente_t* paciente = (paciente_t*)_paciente;
	free(paciente->nombre);
	free(paciente->ingreso);
	free(paciente);
}

void doctor_destruir(void* _doctor) {
	doctor_t* doctor = (doctor_t*)_doctor;
	free(doctor->nombre);
	free(doctor->especialidad);
	free(doctor);
}

void lista_de_espera_destruir(void* _lista) {
	lista_de_espera_t* lista = (lista_de_espera_t*)_lista;
	if(lista->urgencias)
		cola_destruir(lista->urgencias, paciente_destruir);
	if(lista->regulares)
		heap_destruir(lista->regulares, paciente_destruir);
	free(lista);
}

paciente_t* paciente_crear(char** strv){ //constructor pasado en la funcion csv_crear_estructura
	paciente_t* paciente = malloc(sizeof(paciente_t));

	if (!paciente) return NULL;

	paciente->nombre = strdup(strv[0]);
	paciente->ingreso = strdup(strv[1]);

	return paciente;
}

doctor_t* doctor_crear(char** strv){ //constructor pasado en la funcion csv_crear_estructura
	doctor_t* doctor = malloc(sizeof(doctor_t));

	if (!doctor) return NULL;

	doctor->nombre = strdup(strv[0]);
	doctor->especialidad = strdup(strv[1]);
	doctor->pacientes_atendidos = 0;

	return doctor;
}

lista_de_espera_t* lista_de_espera_crear(char** strv) {
	lista_de_espera_t* lista_de_espera = malloc(sizeof(lista_de_espera_t));
	if(!lista_de_espera) return NULL;

	lista_de_espera->urgencias = cola_crear();
	lista_de_espera->regulares = heap_crear(paciente_cmp);

	if(!lista_de_espera->urgencias || !lista_de_espera->regulares) {
		lista_de_espera_destruir(lista_de_espera);
		return false;
	}

	lista_de_espera->total_pacientes = 0;
	return lista_de_espera;
}

bool guardar_doctor(void* _doctores, char** datos) {
	abb_t* doctores = (abb_t*)_doctores;
	doctor_t* doctor = doctor_crear(datos);
	if(!doctor) return false;

	if(!abb_guardar(doctores, doctor->nombre, doctor)) {
		doctor_destruir(doctor);
		return false;
	}
	return true;
}

bool guardar_paciente(void* _pacientes, char** datos) {
	abb_t* pacientes = (abb_t*)_pacientes;
	paciente_t* paciente = paciente_crear(datos);
	if(!paciente) return false;

	if(!abb_guardar(pacientes, paciente->ingreso, paciente)) {
		paciente_destruir(paciente);
		return false;
	}
	return true;
}

bool guardar_lista_de_espera(void* _turnos, char** datos) {
	hash_t* turnos = (hash_t*)_turnos;
	lista_de_espera_t* lista_de_espera = lista_de_espera_crear(datos);
	if(!lista_de_espera) return false;

	if(!hash_guardar(turnos, datos[1], lista_de_espera)) {
		lista_de_espera_destruir(lista_de_espera);
		return false;
	}
	return true;
}

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
