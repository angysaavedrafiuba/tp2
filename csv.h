#ifndef __CSV__H_
#define __CSV__H_

#include "abb.h"
#include "lista.h"
#include "strutil.h"

bool csv_cargar_datos(const char *ruta_csv, bool (*cargar)(char **, void *),
                      void *extra);

#endif
