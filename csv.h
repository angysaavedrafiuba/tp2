#ifndef __CSV__H_
#define __CSV__H_

#include "abb.h"
#include "lista.h"
#include "strutil.h"

/*  Se encarga de guardar los datos leidos de los archivos csv con 
    la funcion que se pasa por parámetro
*/
bool csv_cargar_datos(const char *ruta_csv, bool (*cargar)(char **, void *),
                      void *extra);

#endif
