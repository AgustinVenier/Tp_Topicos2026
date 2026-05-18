#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

typedef struct {
    int anio, mes, dia;
}t_fecha;

typedef struct {
    int dni;
    //char cuil[];
    char ayn[61];
    t_fecha fecha_nac;
    char sexo;
    t_fecha fecha_afi;
    char cat[11];
    t_fecha fecha_ult;
    char estado;
    char plan[11];
    char email[31];
}t_miembros;

typedef struct{
    int id;
    char titulo[61];
    char genero[21];
    int stock;
}t_titulos;

//preguntar como es la estructura para los miebros

#endif // ESTRUCTURAS_H_INCLUDED
