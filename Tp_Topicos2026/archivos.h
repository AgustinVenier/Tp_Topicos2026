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
					   t_titulos **vec, size_t *cantidad, size_t *capacidad);
int titulos_cargar_desde_binario(const char *nombre_binario,
					 t_titulos **vec, size_t *cantidad, size_t *capacidad);
int titulos_guardar_en_binario(const char *nombre_binario,
					   const t_titulos *vec, size_t cantidad);
int titulos_construir_indice(const t_titulos *vec, size_t cantidad, t_indice *indice);

// Funciones para manejo de errores (auditoria)
int miembros_cargar_desde_texto_con_auditoria(const char *nombre_texto, const t_fecha *fecha_proceso,
                                              t_miembro **vec, size_t *cantidad, size_t *capacidad,
                                              t_error_miembro **errores, size_t *cantidad_errores, size_t *capacidad_errores);

int titulos_cargar_desde_texto_con_auditoria(const char *nombre_texto,
                                             t_titulos **vec, size_t *cantidad, size_t *capacidad,
                                             t_error_titulo **errores, size_t *cantidad_errores, size_t *capacidad_errores,
                                             const t_fecha *fecha_proceso);

void mostrar_auditoria_miembros(const t_error_miembro *errores, size_t cantidad);
void mostrar_auditoria_titulos(const t_error_titulo *errores, size_t cantidad);

#endif // ARCHIVOS_H_INCLUDED
