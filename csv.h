#ifndef __CSV__H_
#define __CSV__H_

#include "lista.h"
#include "strutil.h"

/**
Haciendo uso de strutil (split) lee un archivo csv y, línea a línea,
va llamando al constructor que se pasa por parámetro. Dicho constructor se invoca
con la línea separada por split, sin fines de línea ni ninguna otra consideración,
y con el puntero extra que se pasa por parámetro.
Importante: la función constructor no debe guardar las referencias a las cadenas
dentro de arreglo de cadenas pasado por parámetros (hacer copias en caso de ser 
necesario); luego de invocarse el constructor se invoca a free_strv.

Se devuelve una lista con todos los elementos construidos. NULL en caso que el archivo
csv (indicado por la ruta pasada por parámetro) no exista. 
**/
lista_t* csv_crear_estructura(const char* ruta_csv, void* (*constructor) (char**, void*), void* extra);

abb_t* csv_crear_abb(const char* ruta_csv, int (*abb_comparar_clave_t) (const char *, const char *), void (*abb_destruir_dato_t) (void *), void* (*constructor) (char**));

#endif
