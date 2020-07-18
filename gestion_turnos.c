#include "gestion_turnos.h"
#include "abb.h"
#include <stdlib.h>
#include <string.h>

typedef struct paciente{
	char* nombre;
	size_t ingreso;
}paciente_t;

typedef struct doctor{
	char* nombre;
	char* especialidad;
	size_t pacientes_atendidos;
}doctor_t;

typedef struct lista_de_espera{
	cola_t* urgencias:
	heap_t* regulares;
	size_t total_pacientes;
}lista_de_espera_t;

struct gestion_turnos{
	abb_t* pacientes;
	abb_t* doctores;
	hash_t* turnos;
};

gestion_turnos_t* gestion_turnos_crear(const char* ruta_csv_doctores,const char* ruta_csv_pacientes){
	//gestion_turnos_t* gestion_turnos = malloc...
	//pacientes = csv_crear_arbol_pacientes...
	//doctores = csv_crear_arbol_doctores... dentro de esta funcion no se si ir rellenando el hash de especialidades
}

