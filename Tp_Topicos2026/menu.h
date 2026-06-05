#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stddef.h>
#include "archivos.h"

void menu(t_miembro **vec_miembros, size_t *cantidad_miembros, size_t *capacidad_miembros, t_indice *ind_miembros,
          t_titulos **vec_titulos, size_t *cantidad_titulos, size_t *capacidad_titulos, t_indice *ind_titulos,
          t_alquiler **vec_alquileres, size_t *cantidad_alquileres, size_t *capacidad_alquileres,
          t_error_miembro **errores_miembros, size_t *cantidad_errores_miembros, size_t *capacidad_errores_miembros,
          t_error_titulo **errores_titulos, size_t *cantidad_errores_titulos, size_t *capacidad_errores_titulos,
          const t_fecha *fecha);
char mostrarMenu(const char *msj, const char *opc);
void preguntarCambio(const char * cad, char* aux);
void eliminarFinDeLinea(char *cad);
int AltaMiembro(t_miembro **vec, size_t *cantidad, size_t *capacidad, t_indice *ind, const t_fecha *fecha,
                t_error_miembro **errores, size_t *cantidad_errores, size_t *capacidad_errores);
int BajaMiembro(t_miembro *vec, t_indice *ind);
int ModificacionMiembro(t_miembro *vec, t_indice *ind, const t_fecha *fecha,
                        t_error_miembro **errores, size_t *cantidad_errores, size_t *capacidad_errores);
int MostrarInfoMiembro(t_miembro *vec, t_indice *ind);
int ListadoXDNI(t_miembro *vec, t_indice *ind);
int ListadoXPlan(t_miembro *vec, t_indice *ind);

int AltaTitulo(t_titulos **vec, size_t *cantidad, size_t *capacidad, t_indice *ind,
               t_error_titulo **errores, size_t *cantidad_errores, size_t *capacidad_errores, const t_fecha *fecha);
int BajaTitulo(t_titulos *vec, t_indice *ind);
int ModificacionTitulo(t_titulos *vec, t_indice *ind,
                       t_error_titulo **errores, size_t *cantidad_errores, size_t *capacidad_errores, const t_fecha *fecha);
int MostrarInfoTitulo(t_titulos *vec, t_indice *ind);
int ListadoTitulos(t_titulos *vec, t_indice *ind);

int AlquilerTitulo(t_miembro *vec_miembros, t_indice *ind_miembros,
                   t_titulos *vec_titulos, t_indice *ind_titulos,
                   t_alquiler **vec_alquileres, size_t *cantidad_alquileres, size_t *capacidad_alquileres);

#endif // MENU_H_INCLUDED
