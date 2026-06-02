#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stddef.h>
#include "archivos.h"

void menu(t_miembro **vec, size_t *cantidad, size_t *capacidad, t_indice *ind, const t_fecha *fecha);
char mostrarMenu(const char *msj, const char *opc);
void preguntarCambio(const char * cad, char* aux);
void eliminarFinDeLinea(char *cad);
int AltaMiembro(t_miembro **vec, size_t *cantidad, size_t *capacidad, t_indice *ind, const t_fecha *fecha);
int BajaMiembro(t_miembro *vec, t_indice *ind);
int ModificacionMiembro(t_miembro *vec, t_indice *ind, const t_fecha *fecha);
int MostrarInfoMiembro(t_miembro *vec, t_indice *ind);
int ListadoXDNI(t_miembro *vec, t_indice *ind);
int ListadoXPlan(t_miembro *vec, t_indice *ind);

#endif // MENU_H_INCLUDED
