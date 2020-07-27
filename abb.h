#ifndef ABB_H
#define ABB_H

#include <stddef.h>
#include <stdbool.h>
#include "lista.h"

/*---------------------------------------------------
                TIPOS Y ESTRUCTURAS
-----------------------------------------------------*/

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

typedef struct abb_iter abb_iter_t;

/*----------------------------------------------------
                PRIMITIVAS DEL ABB
------------------------------------------------------*/

/* crea el ABB
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);


/* guarda el dato en el ABB, si la clave ya se encuentra en la estructura,
 * la reemplaza. llama a destruir_dato para el dato reemplazado
 * devuelve false si el dato no se pudo guardar
 * pre: la estructura ABB fue inicializada
 * pos: se guardo el par (clave, dato)
*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);


/* borra un elemento de ABB y devuelve el dato correspondiente 
 * a la clave, si la clave no se encuentra devuelve NULL
 * pre: la estructura ABB fue inicializada
 * pos: se borro el elemento y se devolvio el valor en caso 
 * de que estuviera
*/
void *abb_borrar(abb_t *arbol, const char *clave);


/* devuelve el dato correspondiente a la clave pasada por 
 * parametro, devuelve NULL en caso de que no este en el ABB
 * pre: la estructura ABB fue inicializada
 * pos: se devolvio el dato en caso de que estuviera el el ABB
*/
void *abb_obtener(const abb_t *arbol, const char *clave);


/* devuelve true si la clave pasada por parametro esta guardada
 * en el ABB, false en caso contraro
 * pre: la estructura ABB fue inicializada
*/
bool abb_pertenece(const abb_t *arbol, const char *clave);


/* devuelve la cantidad de elementos que contiene el ABB
 * pre: la estructura ABB fue inicializada
 * pos: se devolvio la cantidad de elementos que contiene
 * la estructura
*/
size_t abb_cantidad(abb_t *arbol);


/* destruye el ABB liberando la memoria pedida y llamando a
 * destruir_dato para cada dato
 * pre: la estructura ABB fue inicializada
 * pos: se destruyo el ABB y se libero el espacio
*/
void abb_destruir(abb_t *arbol);

/* devuelve una lista de todos los datos ordenados desde "inicio"
   hasta "fin"
   si "inicio" es vacío, agrega los datos desde el menor elemento hasta "fin"
   si "fin" es vacío, agrega los datos desde "inicio" hasta el mayor elemento
*/
lista_t* abb_ver_lista(abb_t* arbol, char* inicio, char* fin);

/*-------------------------------------------------------
            PRIMITIVA DEL ITERADOR INTERNO
---------------------------------------------------------*/

/* itera sobre los elementos del ABB */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/*-------------------------------------------------------
            PRIMITIVAS DEL ITERADOR EXTERNO
---------------------------------------------------------*/

/* crea el iterador */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* avanza el iterador a la siguiente posicion 
 * pre: el iterador externo fue inicializado
*/
bool abb_iter_in_avanzar(abb_iter_t *iter);


/* devuelve la clave del elemento en el cual esta
 * posicionado el iterador o NULL en caso de estar
 * al final
 * pre: el iterador externo fue inicializado
 * pos: se devolvio la clave del elemento
*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* devuelve true si el iterador ya itero sobre todos los
 * elementos del ABB, false en caso contrario
 * pre: el iterador externo fue inicializado
*/
bool abb_iter_in_al_final(const abb_iter_t *iter);

/*destruye el iterador externo */
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H