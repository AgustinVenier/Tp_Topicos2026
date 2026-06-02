#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include "indice.h"

int miembros_cargar_desde_texto(const char *nombre_texto, const t_fecha *fecha_proceso,
								t_miembro **vec, size_t *cantidad, size_t *capacidad);
int miembros_cargar_desde_binario(const char *nombre_binario,
								  t_miembro **vec, size_t *cantidad, size_t *capacidad);
int miembros_guardar_en_binario(const char *nombre_binario,
								const t_miembro *vec, size_t cantidad);
int miembros_construir_indice(const t_miembro *vec, size_t cantidad, t_indice *indice);

int titulos_cargar_desde_texto(const char *nombre_texto,
							   t_titulo **vec, size_t *cantidad, size_t *capacidad);
int titulos_cargar_desde_binario(const char *nombre_binario,
								 t_titulo **vec, size_t *cantidad, size_t *capacidad);
int titulos_guardar_en_binario(const char *nombre_binario,
							   const t_titulo *vec, size_t cantidad);
int titulos_construir_indice(const t_titulo *vec, size_t cantidad, t_indice *indice);

#endif // ARCHIVOS_H_INCLUDED
