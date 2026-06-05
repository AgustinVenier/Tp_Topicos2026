#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

typedef struct {
    int anio, mes, dia;
} t_fecha;

typedef struct {
    long dni;
    char nya[61];
    t_fecha fecha_nac;
    char sexo;
    t_fecha fecha_afi;
    char cat[11];
    t_fecha fecha_cuota;
    char estado;
    char plan[11];
    char email[31];
    char cuil[12];
} t_miembro;

typedef t_miembro t_miembros;

typedef struct {
    int id;
    char titulo[61];
    char genero[21];
    int stock;
    char estado;
}t_titulos;

typedef struct {
    long dni;
    int id_titulo;
    int cantidad_alquileres;
    char estado;
} t_alquiler;

// Estructura para registrar errores de miembros
typedef struct {
    char tipo_error[51];      // Tipo de validacion fallida (DNI, Fecha, Email, etc)
    long dni;                  // DNI asociado al error
    t_fecha fecha;             // Fecha de proceso
    char email[31];            // Email del registro con error
} t_error_miembro;

// Estructura para registrar errores de titulos
typedef struct {
    char tipo_error[51];      // Tipo de validacion fallida
    int id_titulo;            // ID del titulo
    t_fecha fecha;             // Fecha de proceso
    char titulo[61];           // Titulo del registro con error
} t_error_titulo;

#endif // ESTRUCTURAS_H_INCLUDED
