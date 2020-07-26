#ifndef __CSV__H_
#define __CSV__H_

#include "lista.h"
#include "strutil.h"
#include "abb.h"

bool csv_cargar_datos(const char* ruta_csv, bool (*cargar) (char**, void*), void* extra);

#endif
