#ifndef VALIDACIONES_H_INCLUDED
#define VALIDACIONES_H_INCLUDED

#include "estructuras.h"

#define ERROR 1
#define OK 0

void ingresarFechaProceso(t_fecha *fecha_proceso);
int validarFecha(t_fecha *f);
int validarEmail(const char s[]);

#endif // VALIDACIONES_H_INCLUDED
