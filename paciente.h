#ifndef PACIENTE
#define PACIENTE

#include <stdbool.h>

typedef struct paciente paciente_t;

/* Crea un nuevo paciente */
paciente_t* paciente_crear(char** datos_paciente);

/*  Compara dos pacientes por sus nombres
    devuelve un numero positivo si pac1 es mayor a pac2,
    un numero negativo si pac1 es menor a pac2
    y devuelve 0 en caso de que sean iguales
*/
int paciente_cmp(const paciente_t *pac1, const paciente_t *pac2);

/*  Destruye el paciente pasado por parámetro liberando
    la memoria usada
    pre: el paciente fue inicializado
*/
void paciente_destruir(paciente_t* paciente);

/*  Devuelve el nombre del paciente pasado por parámetro
    pre: el paciente fue inicializado
*/
char* obtener_nombre_paciente(paciente_t* paciente);

#endif