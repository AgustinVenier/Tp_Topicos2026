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

#endif // ESTRUCTURAS_H_INCLUDED
