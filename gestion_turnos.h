#ifndef GESTION_TURNOS
#define GESTION_TURNOS

#include "csv.h"
#include <stdbool.h>
#include <stddef.h>

/*---------------------------------------------------
                TIPOS Y ESTRUCTURAS
-----------------------------------------------------*/

typedef struct gestion_turnos gestion_turnos_t;

typedef void (*gestion_turnos_destruir_dato_t)(void *);

typedef int (*gestion_turnos_comparar_clave_t)(const void *, const void *);
/*----------------------------------------------------
                PRIMITIVAS DE GESTION TURNOS
------------------------------------------------------*/

/*
    Crea una gestion de turnos, recibe como parámetros la función de 
    destruccion de los atendedores que va a almacenar, la función de 
    comparación entre los que van a ser atendidos, y la funcion de 
    destruccion de los que van a ser atendidos.

    Devuelve la gestión de turnos creada y vacía, en caso contrario
    devuelve NULL.
*/
gestion_turnos_t *
gestion_turnos_crear(gestion_turnos_destruir_dato_t dst_atendedor,
                     gestion_turnos_comparar_clave_t cmp_atendido,
                     gestion_turnos_destruir_dato_t dst_atendido);

/*
    Destruye la gestión de turnos pasada por parámetro.
    Pre: La gestión de turnos fue creada.
    Post: La gestión de turnos fue destruida.
*/
void gestion_turnos_destruir(gestion_turnos_t *gestion_turnos);

/*
    Devuelve la cantidad de elementos que faltan por atender, es decir
    que están "en espera". Recibe por parámetro la categoría a consultar.

    Pre: La gestión de turnos fue creada y la categoría agregada.
*/
size_t cantidad_atendidos_en_espera(gestion_turnos_t *gestion_turnos,
                                    char *categoria);

/*
    Agrega un elemento a los que serán atendidos, recibe por parámetro un
    vector de string donde la primera posición deberá ser el identificador
    del elemento, el elemento a agregar será el resultado de aplicar la función
    constructor al vector de string. 

    Devuelve true si se agregó, false en caso contrario. 

    Pre: La gestión de turnos fue creada y datos posee al menos un elemento.
*/
bool agregar_atendido(gestion_turnos_t *gestion_turnos, char **datos,
                      void *(*constructor)(char **));

/*
    Agrega un elemento a los que serán atendedores, recibe por parámetro un
    vector de string donde la segunda posición deberá ser la categoría
    del elemento, el elemento a agregar será el resultado de aplicar la función
    constructor al vector de string. 

    Devuelve true si se agregó, false en caso contrario. 

    Pre: La gestión de turnos fue creada y datos posee al menos dos elemento.
*/
bool agregar_atendedor(gestion_turnos_t *gestion_turnos, char **datos,
                       void *(*constructor)(char **));

/*
    Devuelve el dato cuyo identificador es nombre.
    Pre: La gestión de turnos fue creada.
*/
void *gestion_turnos_obtener_atendedor(gestion_turnos_t *gestion_turnos,
                                       char *nombre);

/*
    Guarda un turno para nombre, en la categoría y con la prioridad pasadas por parámetro.
    Devuelve true si lo pudo guardar, false en caso contrario.
    Pre: La gestión de turnos fue creada.
*/
bool pedir_turno(gestion_turnos_t *gestion_turnos, char *nombre,
                 char *categoria, char *prioridad);

/*
    Busca al siguiente en ser atendido por nombre en la categoría pasada por parámetro.
    Devuelve true si pudo ser atendido, false en caso contrario.
    Pre: La gestión de turnos fue creada.
*/
bool atender_siguiente(gestion_turnos_t *gestion_turnos, char *nombre,
                       char *categoria);

/*
    Devuelve una lista de elementos que són atendedores dentro del rango especificado.
    Pre: La gestión de turnos fue creada.
*/
lista_t *informe_atendedores(gestion_turnos_t *gestion_turnos, char *inicio,
                             char *fin);

/*
    Devuelve true si el nombre pasado por párámetro existe entre los atendidos.
    Pre: La gestión de turnos fue creada.
*/
bool atendido_existe(gestion_turnos_t *gestion_turnos, char *nombre);

/*
    Devuelve true si la categoría pasada por parámetro existe, false en caso contrario.
    Pre: La gestión de turnos fue creada.
*/
bool categoria_existe(gestion_turnos_t *gestion_turnos, char *categoria);

/*
    Devuelve true si la prioridad pasada por parámetro existe, false en caso contrario.
    Pre: La gestión de turnos fue creada.  
*/
bool prioridad_existe(gestion_turnos_t *gestion_turnos, char *prioridad);


/*
    Devuelve el elemento que está siendo atendido actualmente.
    Pre: La gestión de turnos fue creada.
*/
void *obtener_atendido_actual(gestion_turnos_t *gestion_turnos);

/*
    Devuelve la cantidad de atendedores en el sistema.
    Pre: La gestión de turnos fue creada.
*/
size_t cantidad_atendedores(gestion_turnos_t *gestion_turnos);

#endif