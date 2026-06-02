#ifndef VALIDACIONES_H_INCLUDED
#define VALIDACIONES_H_INCLUDED

#include "estructuras.h"
#include <ctype.h>
#include <string.h>

#define ERROR 1
#define OK 0
#define BUFFER 250
#define ANIOS_DESDE_NACIMIENTO 10

#ifdef _WIN32
#define strcmpi _stricmp
#endif

#define dniValido(a) (1000000<(a)&&(a)<100000000)
#define sexValido(a) (toupper(a)=='F'||toupper(a)=='M')
#define estadoValido(a) (toupper(a)=='A'||toupper(a)=='B')
#define planValido(a) ((strcmpi((a),"BASIC")==0)||(strcmpi((a),"PREMIUM")==0)||(strcmpi((a),"VIP")==0)||(strcmpi((a),"FAMILY")==0))

void ingresarFechaProceso(t_fecha *fecha_proceso);
int validarFecha(t_fecha *f);
int compararFecha(const t_fecha *a, const t_fecha *b);
int validarEmail(const char s[]);
char * normalizar(char * cad);
int validarFechaCategoria(const char * categ,const t_fecha* fechaNac,const t_fecha * f_proceso);
int fNacValido(const t_fecha* fechaNac,const t_fecha * f_proceso);
int fAfiliacionValido(const t_fecha* fechaAfi, const t_fecha* fechaProc, const t_fecha* fechaNac);
int fUltCoutaValido(const t_fecha* fechaCuota, const t_fecha* fechaAfi, const t_fecha* fechaProc);


#endif // VALIDACIONES_H_INCLUDED
