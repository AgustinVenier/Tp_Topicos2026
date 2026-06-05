#include "menu.h"
#include "validaciones.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void limpiarBuffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static int asegurar_capacidad_registros(t_miembro **vec, size_t *capacidad, size_t necesaria)
{
    size_t nueva_capacidad;
    t_miembro *aux;

    if (necesaria <= *capacidad)
        return OK;

    nueva_capacidad = (*capacidad == 0) ? CANT_ELEMENTOS : *capacidad;
    while (nueva_capacidad < necesaria)
        nueva_capacidad = (size_t)((double)nueva_capacidad * INCREMENTO) + 1;

    aux = realloc(*vec, nueva_capacidad * sizeof(t_miembro));
    if (!aux)
        return ERROR;

    *vec = aux;
    *capacidad = nueva_capacidad;
    return OK;
}

static int asegurar_capacidad_titulos(t_titulos **vec, size_t *capacidad, size_t necesaria)
{
    size_t nueva_capacidad;
    t_titulos *aux;

    if (necesaria <= *capacidad)
        return OK;

    nueva_capacidad = (*capacidad == 0) ? CANT_ELEMENTOS : *capacidad;
    while (nueva_capacidad < necesaria)
        nueva_capacidad = (size_t)((double)nueva_capacidad * INCREMENTO) + 1;

    aux = realloc(*vec, nueva_capacidad * sizeof(t_titulos));
    if (!aux)
        return ERROR;

    *vec = aux;
    *capacidad = nueva_capacidad;
    return OK;
}

static int asegurar_capacidad_errores_miembros(t_error_miembro **vec, size_t *capacidad, size_t necesaria)
{
    size_t nueva_capacidad;
    t_error_miembro *aux;

    if (necesaria <= *capacidad)
        return OK;

    nueva_capacidad = (*capacidad == 0) ? CANT_ELEMENTOS : *capacidad;
    while (nueva_capacidad < necesaria)
        nueva_capacidad = (size_t)((double)nueva_capacidad * INCREMENTO) + 1;

    aux = realloc(*vec, nueva_capacidad * sizeof(t_error_miembro));
    if (!aux)
        return ERROR;

    *vec = aux;
    *capacidad = nueva_capacidad;
    return OK;
}

static int asegurar_capacidad_errores_titulos(t_error_titulo **vec, size_t *capacidad, size_t necesaria)
{
    size_t nueva_capacidad;
    t_error_titulo *aux;

    if (necesaria <= *capacidad)
        return OK;

    nueva_capacidad = (*capacidad == 0) ? CANT_ELEMENTOS : *capacidad;
    while (nueva_capacidad < necesaria)
        nueva_capacidad = (size_t)((double)nueva_capacidad * INCREMENTO) + 1;

    aux = realloc(*vec, nueva_capacidad * sizeof(t_error_titulo));
    if (!aux)
        return ERROR;

    *vec = aux;
    *capacidad = nueva_capacidad;
    return OK;
}

static void imprimir_miembro(const t_miembro *m)
{
    printf("DNI: %ld\n", m->dni);
    printf("CUIL: %s\n", m->cuil);
    printf("Nombre y Apellido: %s\n", m->nya);
    printf("Fecha de Nacimiento: %02d/%02d/%04d\n", m->fecha_nac.dia, m->fecha_nac.mes, m->fecha_nac.anio);
    printf("Sexo: %c\n", m->sexo);
    printf("Fecha de Afiliacion: %02d/%02d/%04d\n", m->fecha_afi.dia, m->fecha_afi.mes, m->fecha_afi.anio);
    printf("Categoria: %s\n", m->cat);
    printf("Fecha de ultima Cuota: %02d/%02d/%04d\n", m->fecha_cuota.dia, m->fecha_cuota.mes, m->fecha_cuota.anio);
    printf("Estado: %c\n", m->estado);
    printf("Plan: %s\n", m->plan);
    printf("Email: %s\n", m->email);
}

void menu(t_miembro **vec_miembros, size_t *cantidad_miembros, size_t *capacidad_miembros, t_indice *ind_miembros,
          t_titulos **vec_titulos, size_t *cantidad_titulos, size_t *capacidad_titulos, t_indice *ind_titulos,
          t_alquiler **vec_alquileres, size_t *cantidad_alquileres, size_t *capacidad_alquileres,
          t_error_miembro **errores_miembros, size_t *cantidad_errores_miembros, size_t *capacidad_errores_miembros,
          t_error_titulo **errores_titulos, size_t *cantidad_errores_titulos, size_t *capacidad_errores_titulos,
          const t_fecha *fecha)
{
    char op;
    int resultado;

    do
    {
        printf("\n");
        op = mostrarMenu(
                 "a. Alta miembro\n"
                 "b. Alta titulo\n"
                 "c. Baja miembro\n"
                 "d. Baja titulo\n"
                 "e. Modificacion miembro\n"
                 "f. Modificacion titulo\n"
                 "g. Mostrar informacion de un miembro\n"
                 "h. Alquiler de un titulo\n"
                 "i. Listado de miembros ordenados por DNI\n"
                 "j. Listado de todos los miembros agrupados por plan\n"
                 "k. Auditoria errores miembros\n"
                 "l. Auditoria errores titulos\n"
                 "m. Salir\n",
                 "abcdefghijklm"
             );

        switch(op)
        {
        case 'a':
            resultado = AltaMiembro(vec_miembros, cantidad_miembros, capacidad_miembros, ind_miembros, fecha,
                                   errores_miembros, cantidad_errores_miembros, capacidad_errores_miembros);
            printf(resultado == ERROR ? "\nError al dar de alta el miembro.\n" : "\nAlta realizada con exito.\n");
            break;

        case 'b':
            resultado = AltaTitulo(vec_titulos, cantidad_titulos, capacidad_titulos, ind_titulos,
                                  errores_titulos, cantidad_errores_titulos, capacidad_errores_titulos, fecha);
            printf(resultado == ERROR ? "\nError al dar de alta el titulo.\n" : "\nAlta realizada con exito.\n");
            break;

        case 'c':
            resultado = BajaMiembro(*vec_miembros, ind_miembros);
            printf(resultado == ERROR ? "\nError al dar la baja del miembro.\n" : "\nBaja realizada con exito.\n");
            break;

        case 'd':
            resultado = BajaTitulo(*vec_titulos, ind_titulos);
            printf(resultado == ERROR ? "\nError al dar la baja del titulo.\n" : "\nBaja realizada con exito.\n");
            break;

        case 'e':
            resultado = ModificacionMiembro(*vec_miembros, ind_miembros, fecha,
                                           errores_miembros, cantidad_errores_miembros, capacidad_errores_miembros);
            printf(resultado == ERROR ? "\nError al modificar el miembro.\n" : "\nModificacion realizada con exito.\n");
            break;

        case 'f':
            resultado = ModificacionTitulo(*vec_titulos, ind_titulos,
                                          errores_titulos, cantidad_errores_titulos, capacidad_errores_titulos, fecha);
            printf(resultado == ERROR ? "\nError al modificar el titulo.\n" : "\nModificacion realizada con exito.\n");
            break;

        case 'g':
            resultado = MostrarInfoMiembro(*vec_miembros, ind_miembros);
            printf(resultado == ERROR ? "\nError al mostrar\n" : "\nRealizada con exito.\n");
            system("pause");
            break;

        case 'h':
            resultado = AlquilerTitulo(*vec_miembros, ind_miembros, *vec_titulos, ind_titulos,
                                      vec_alquileres, cantidad_alquileres, capacidad_alquileres);
            printf(resultado == ERROR ? "\nError al realizar el alquiler.\n" : "");
            system("pause");
            break;

        case 'i':
            resultado = ListadoXDNI(*vec_miembros, ind_miembros);
            printf(resultado == ERROR ? "\nError al listar\n" : "\nRealizada con exito.\n");
            system("pause");
            break;

        case 'j':
            resultado = ListadoXPlan(*vec_miembros, ind_miembros);
            printf(resultado == ERROR ? "\nError al Listar\n" : "\nRealizada con exito.\n");
            system("pause");
            break;

        case 'k':
            mostrar_auditoria_miembros(*errores_miembros, *cantidad_errores_miembros);
            system("pause");
            break;

        case 'l':
            mostrar_auditoria_titulos(*errores_titulos, *cantidad_errores_titulos);
            system("pause");
            break;

        case 'm':
            printf("Saliendo...\n");
            break;

        default:
            printf("Opcion invalida\n");
            break;
        }
    }
    while(op != 'm');
}

void eliminarFinDeLinea(char *cad)
{
    char *p = strchr(cad, '\n');
    if (p)
        *p = '\0';
}

char mostrarMenu(const char *msj, const char *opc)
{
    char opta[32];
    int priVez = 1;

    printf("\n===================================================\n");
    printf("              GESTION DE MIEMBROS\n");
    printf("===================================================\n");

    do
    {
        printf("%s%s",
               priVez ? (priVez = 0, "") : "ERROR - Opcion No valida\n",
               msj);
        printf("\nIngrese seleccion: ");
        if (!fgets(opta, sizeof(opta), stdin))
            return 'k';
        eliminarFinDeLinea(opta);
        if (opta[0] == '\0')
            continue;
    }
    while(strchr(opc, opta[0]) == NULL || opta[1] != '\0');

    return opta[0];
}

void preguntarCambio(const char *cad, char* aux)
{
    printf("Desea modificar %s? (S/N): ", cad);
    scanf(" %c", aux);
    limpiarBuffer();
}

int AltaMiembro(t_miembro **vec, size_t *cantidad, size_t *capacidad, t_indice *ind, const t_fecha *fecha,
                t_error_miembro **errores, size_t *cantidad_errores, size_t *capacidad_errores)
{
    t_miembro m;
    t_error_miembro error;
    t_reg_indice reg;
    long dni;
    int validar;

    memset(&m, 0, sizeof(m));
    memset(&error, 0, sizeof(error));

    printf("\nIngrese DNI: ");
    if (scanf("%ld", &dni) != 1)
    {
        limpiarBuffer();
        return ERROR;
    }
    limpiarBuffer();

    if (!dniValido(dni))
    {
        strcpy(error.tipo_error, "DNI");
        error.dni = dni;
        error.fecha = *fecha;
        strcpy(error.email, "");
        
        if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
        {
            (*errores)[*cantidad_errores] = error;
            (*cantidad_errores)++;
        }
        return ERROR;
    }

    reg.dni = dni;
    if (indice_buscar(ind, &reg, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni) != NO_EXISTE)
    {
        printf("Error: ya existe un miembro con DNI %ld.\n", dni);
        return ERROR;
    }

    m.dni = dni;

    printf("Ingrese Apellido y Nombre: ");
    if (!fgets(m.nya, sizeof(m.nya), stdin))
        return ERROR;
    eliminarFinDeLinea(m.nya);
    normalizar(m.nya);

    do
    {
        printf("Ingrese sexo (M/F): ");
        if (scanf(" %c", &m.sexo) != 1)
            return ERROR;
        limpiarBuffer();
        validar = sexValido(m.sexo) ? OK : ERROR;
        if (validar == ERROR)
        {
            strcpy(error.tipo_error, "Sexo");
            error.dni = dni;
            error.fecha = *fecha;
            strcpy(error.email, "");
            if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
            {
                (*errores)[*cantidad_errores] = error;
                (*cantidad_errores)++;
            }
            printf("Ingreso Incorrecto.\n");
        }
    }
    while (validar == ERROR);

    {
        char dni_str[20];
        snprintf(dni_str, sizeof(dni_str), "%ld", m.dni);
        obtenerCuil(dni_str, m.sexo, m.cuil);
    }

    do
    {
        printf("Ingrese fecha de nacimiento (dd/mm/aaaa): ");
        if (scanf("%d/%d/%d", &m.fecha_nac.dia, &m.fecha_nac.mes, &m.fecha_nac.anio) != 3)
            return ERROR;
        limpiarBuffer();
        validar = fNacValido(&m.fecha_nac, fecha);
        if(validar == ERROR)
        {
            strcpy(error.tipo_error, "Fecha Nacimiento");
            error.dni = dni;
            error.fecha = *fecha;
            strcpy(error.email, "");
            if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
            {
                (*errores)[*cantidad_errores] = error;
                (*cantidad_errores)++;
            }
            printf("Ingreso Incorrecto.\n");
        }
    }
    while(validar == ERROR);

    do
    {
        printf("Ingrese fecha de afiliacion (dd/mm/aaaa): ");
        if (scanf("%d/%d/%d", &m.fecha_afi.dia, &m.fecha_afi.mes, &m.fecha_afi.anio) != 3)
            return ERROR;
        limpiarBuffer();
        validar = fAfiliacionValido(&m.fecha_afi, fecha, &m.fecha_nac);
        if(validar == ERROR)
        {
            strcpy(error.tipo_error, "Fecha Afiliacion");
            error.dni = dni;
            error.fecha = *fecha;
            strcpy(error.email, "");
            if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
            {
                (*errores)[*cantidad_errores] = error;
                (*cantidad_errores)++;
            }
            printf("Ingreso Incorrecto.\n");
        }
    }
    while(validar == ERROR);

    do
    {
        printf("Ingrese categoria (MENOR/ADULTO): ");
        if (!fgets(m.cat, sizeof(m.cat), stdin))
            return ERROR;
        eliminarFinDeLinea(m.cat);
        validar = validarFechaCategoria(m.cat, &m.fecha_nac, fecha);
        if(validar == ERROR)
        {
            strcpy(error.tipo_error, "Categoria");
            error.dni = dni;
            error.fecha = *fecha;
            strcpy(error.email, "");
            if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
            {
                (*errores)[*cantidad_errores] = error;
                (*cantidad_errores)++;
            }
            printf("Ingreso Incorrecto.\n");
        }
    }
    while(validar == ERROR);

    do
    {
        printf("Ingrese fecha de ultima cuota paga (dd/mm/aaaa): ");
        if (scanf("%d/%d/%d", &m.fecha_cuota.dia, &m.fecha_cuota.mes, &m.fecha_cuota.anio) != 3)
            return ERROR;
        limpiarBuffer();
        validar = fUltCoutaValido(&m.fecha_cuota, &m.fecha_afi, fecha);
        if(validar == ERROR)
        {
            strcpy(error.tipo_error, "Fecha Cuota");
            error.dni = dni;
            error.fecha = *fecha;
            strcpy(error.email, "");
            if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
            {
                (*errores)[*cantidad_errores] = error;
                (*cantidad_errores)++;
            }
            printf("Ingreso Incorrecto.\n");
        }
    }
    while(validar == ERROR);

    do
    {
        printf("Ingrese plan (BASIC/PREMIUM/VIP/FAMILY): ");
        if (!fgets(m.plan, sizeof(m.plan), stdin))
            return ERROR;
        eliminarFinDeLinea(m.plan);
        validar = planValido(m.plan) ? OK : ERROR;
        if(validar == ERROR)
        {
            strcpy(error.tipo_error, "Plan");
            error.dni = dni;
            error.fecha = *fecha;
            strcpy(error.email, "");
            if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
            {
                (*errores)[*cantidad_errores] = error;
                (*cantidad_errores)++;
            }
            printf("Ingreso Incorrecto.\n");
        }
    }
    while(validar == ERROR);

    do
    {
        printf("Ingrese email: ");
        if (!fgets(m.email, sizeof(m.email), stdin))
            return ERROR;
        eliminarFinDeLinea(m.email);
        validar = validarEmail(m.email);
        if(validar == ERROR)
        {
            strcpy(error.tipo_error, "Email");
            error.dni = dni;
            error.fecha = *fecha;
            strcpy(error.email, m.email);
            if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
            {
                (*errores)[*cantidad_errores] = error;
                (*cantidad_errores)++;
            }
            printf("Ingreso Incorrecto.\n");
        }
    }
    while(validar == ERROR);

    m.estado = 'A';

    if (asegurar_capacidad_registros(vec, capacidad, *cantidad + 1) == ERROR)
        return ERROR;

    reg.nro_reg = (unsigned)(*cantidad);
    if (indice_insertar(ind, &reg, sizeof(t_reg_indice), cmp_por_dni) == ERROR)
        return ERROR;

    (*vec)[*cantidad] = m;
    (*cantidad)++;
    return OK;
}

int BajaMiembro(t_miembro *vec, t_indice *ind)
{
    long dni;
    t_reg_indice clave;
    int pos;

    printf("DNI a dar de baja: ");
    if (scanf("%ld", &dni) != 1)
        return ERROR;
    limpiarBuffer();

    clave.dni = dni;
    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if (pos == NO_EXISTE)
        return ERROR;

    vec[((t_reg_indice *)ind->vindice)[pos].nro_reg].estado = 'B';
    return indice_eliminar(ind, &clave, sizeof(t_reg_indice), cmp_por_dni);
}

int ModificacionMiembro(t_miembro *vec, t_indice *ind, const t_fecha *fecha,
                        t_error_miembro **errores, size_t *cantidad_errores, size_t *capacidad_errores)
{
    long dni;
    t_reg_indice clave;
    int pos;
    char aux;
    t_miembro *m;
    int validar;
    t_error_miembro error;

    printf("\n=== MODIFICACION DE MIEMBRO ===\n");
    printf("Ingrese DNI a modificar: ");
    if (scanf("%ld", &dni) != 1)
        return ERROR;
    limpiarBuffer();

    clave.dni = dni;
    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if(pos == NO_EXISTE)
        return ERROR;

    m = &vec[((t_reg_indice *)ind->vindice)[pos].nro_reg];
    printf("Modificando a: %s\n", m->nya);

    preguntarCambio("apellido y nombre", &aux);
    if(toupper((unsigned char)aux) == 'S')
    {
        if (!fgets(m->nya, sizeof(m->nya), stdin))
            return ERROR;
        eliminarFinDeLinea(m->nya);
        normalizar(m->nya);
    }

    preguntarCambio("fecha de nacimiento", &aux);
    if(toupper((unsigned char)aux) == 'S')
    {
        do
        {
            printf("Ingrese fecha de nacimiento (dd/mm/aaaa): ");
            if (scanf("%d/%d/%d", &m->fecha_nac.dia, &m->fecha_nac.mes, &m->fecha_nac.anio) != 3)
                return ERROR;
            limpiarBuffer();
            validar = fNacValido(&m->fecha_nac, fecha);
            if(validar == ERROR)
            {
                memset(&error, 0, sizeof(error));
                strcpy(error.tipo_error, "Fecha Nacimiento");
                error.dni = m->dni;
                error.fecha = *fecha;
                strcpy(error.email, m->email);
                
                if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
                {
                    (*errores)[*cantidad_errores] = error;
                    (*cantidad_errores)++;
                }
            }
        }
        while(validar == ERROR);
    }

    preguntarCambio("sexo", &aux);
    if(toupper((unsigned char)aux) == 'S')
    {
        do
        {
            printf("Ingrese sexo (M/F): ");
            if (scanf(" %c", &m->sexo) != 1)
                return ERROR;
            limpiarBuffer();
            validar = sexValido(m->sexo) ? OK : ERROR;
            if(validar == ERROR)
            {
                memset(&error, 0, sizeof(error));
                strcpy(error.tipo_error, "Sexo");
                error.dni = m->dni;
                error.fecha = *fecha;
                strcpy(error.email, m->email);
                
                if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
                {
                    (*errores)[*cantidad_errores] = error;
                    (*cantidad_errores)++;
                }
            }
        }
        while(validar == ERROR);
    }

    preguntarCambio("fecha de afiliacion", &aux);
    if(toupper((unsigned char)aux) == 'S')
    {
        do
        {
            printf("Ingrese fecha de afiliacion (dd/mm/aaaa): ");
            if (scanf("%d/%d/%d", &m->fecha_afi.dia, &m->fecha_afi.mes, &m->fecha_afi.anio) != 3)
                return ERROR;
            limpiarBuffer();
            validar = fAfiliacionValido(&m->fecha_afi, fecha, &m->fecha_nac);
            if(validar == ERROR)
            {
                memset(&error, 0, sizeof(error));
                strcpy(error.tipo_error, "Fecha Afiliacion");
                error.dni = m->dni;
                error.fecha = *fecha;
                strcpy(error.email, m->email);
                
                if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
                {
                    (*errores)[*cantidad_errores] = error;
                    (*cantidad_errores)++;
                }
            }
        }
        while(validar == ERROR);
    }

    preguntarCambio("categoria", &aux);
    if(toupper((unsigned char)aux) == 'S')
    {
        do
        {
            printf("Ingrese categoria (MENOR/ADULTO): ");
            if (!fgets(m->cat, sizeof(m->cat), stdin))
                return ERROR;
            eliminarFinDeLinea(m->cat);
            validar = validarFechaCategoria(m->cat, &m->fecha_nac, fecha);
            if(validar == ERROR)
            {
                memset(&error, 0, sizeof(error));
                strcpy(error.tipo_error, "Categoria");
                error.dni = m->dni;
                error.fecha = *fecha;
                strcpy(error.email, m->email);
                
                if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
                {
                    (*errores)[*cantidad_errores] = error;
                    (*cantidad_errores)++;
                }
            }
        }
        while(validar == ERROR);
    }

    preguntarCambio("fecha de ultima cuota", &aux);
    if(toupper((unsigned char)aux) == 'S')
    {
        do
        {
            printf("Ingrese fecha de ultima cuota paga (dd/mm/aaaa): ");
            if (scanf("%d/%d/%d", &m->fecha_cuota.dia, &m->fecha_cuota.mes, &m->fecha_cuota.anio) != 3)
                return ERROR;
            limpiarBuffer();
            validar = fUltCoutaValido(&m->fecha_cuota, &m->fecha_afi, fecha);
            if(validar == ERROR)
            {
                memset(&error, 0, sizeof(error));
                strcpy(error.tipo_error, "Fecha Cuota");
                error.dni = m->dni;
                error.fecha = *fecha;
                strcpy(error.email, m->email);
                
                if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
                {
                    (*errores)[*cantidad_errores] = error;
                    (*cantidad_errores)++;
                }
            }
        }
        while(validar == ERROR);
    }

    preguntarCambio("plan", &aux);
    if(toupper((unsigned char)aux) == 'S')
    {
        do
        {
            printf("Ingrese plan (BASIC/PREMIUM/VIP/FAMILY): ");
            if (!fgets(m->plan, sizeof(m->plan), stdin))
                return ERROR;
            eliminarFinDeLinea(m->plan);
            validar = planValido(m->plan) ? OK : ERROR;
            if(validar == ERROR)
            {
                memset(&error, 0, sizeof(error));
                strcpy(error.tipo_error, "Plan");
                error.dni = m->dni;
                error.fecha = *fecha;
                strcpy(error.email, m->email);
                
                if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
                {
                    (*errores)[*cantidad_errores] = error;
                    (*cantidad_errores)++;
                }
            }
        }
        while(validar == ERROR);
    }

    preguntarCambio("email", &aux);
    if(toupper((unsigned char)aux) == 'S')
    {
        do
        {
            printf("Ingrese email: ");
            if (!fgets(m->email, sizeof(m->email), stdin))
                return ERROR;
            eliminarFinDeLinea(m->email);
            validar = validarEmail(m->email);
            if(validar == ERROR)
            {
                memset(&error, 0, sizeof(error));
                strcpy(error.tipo_error, "Email");
                error.dni = m->dni;
                error.fecha = *fecha;
                strcpy(error.email, m->email);
                
                if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == OK)
                {
                    (*errores)[*cantidad_errores] = error;
                    (*cantidad_errores)++;
                }
            }
        }
        while(validar == ERROR);
    }

    return OK;
}

int MostrarInfoMiembro(t_miembro *vec, t_indice *ind)
{
    long dni;
    t_reg_indice clave;
    int pos;

    printf("\n============= INFORMACION DE MIEMBRO =============\n");
    printf("Ingrese DNI a visualizar: ");
    if (scanf("%ld", &dni) != 1)
        return ERROR;
    limpiarBuffer();

    clave.dni = dni;
    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_por_dni);
    if(pos == NO_EXISTE)
        return ERROR;

    imprimir_miembro(&vec[((t_reg_indice *)ind->vindice)[pos].nro_reg]);
    return OK;
}

int ListadoXDNI(t_miembro *vec, t_indice *ind)
{
    int i;
    t_reg_indice *vecOrig;

    if (indice_vacio(ind) != ERROR)
        return ERROR;

    vecOrig = (t_reg_indice *)ind->vindice;

    printf("\n\n================ LISTADO POR DNI ================\n");
    for(i = 0; i < (int)ind->cantidad_elementos_actual; i++)
    {
        printf("- ");
        imprimir_miembro(&vec[(vecOrig + i)->nro_reg]);
        printf("--------------------------------------------------\n");
    }
    return OK;
}

int ListadoXPlan(t_miembro *vec, t_indice *ind)
{
    int i;
    static const char *planes[] = {"BASIC", "PREMIUM", "VIP", "FAMILY"};

    if (ind->cantidad_elementos_actual == 0)
        return ERROR;

    for(i = 0; i < 4; i++)
    {
        int n;

        printf("\n\n============= PLAN %s =============\n", planes[i]);
        for(n = 0; n < (int)ind->cantidad_elementos_actual; n++)
        {
            t_miembro *m = &vec[((t_reg_indice *)ind->vindice)[n].nro_reg];
            if (toupper((unsigned char)m->estado) == 'A' && strcmpi(planes[i], m->plan) == 0)
            {
                imprimir_miembro(m);
                printf("-------------------------------------\n");
            }
        }
    }

    return OK;
}

static void imprimir_titulo(const t_titulos *t)
{
    printf("ID: %d\n", t->id);
    printf("Titulo: %s\n", t->titulo);
    printf("Genero: %s\n", t->genero);
    printf("Stock: %d\n", t->stock);
    printf("Estado: %c\n", t->estado);
}

int AltaTitulo(t_titulos **vec, size_t *cantidad, size_t *capacidad, t_indice *ind,
               t_error_titulo **errores, size_t *cantidad_errores, size_t *capacidad_errores, const t_fecha *fecha)
{
    t_titulos t;
    t_error_titulo error;
    t_reg_indice_titulo reg;
    int validar;
    int max_id = 0;
    unsigned i;

    memset(&t, 0, sizeof(t));
    memset(&error, 0, sizeof(error));

    if (ind->cantidad_elementos_actual > 0)
    {
        for (i = 0; i < *cantidad; i++)
        {
            if ((*vec)[i].id > max_id)
                max_id = (*vec)[i].id;
        }
    }

    t.id = max_id + 1;

    printf("Ingrese Titulo: ");
    if (!fgets(t.titulo, sizeof(t.titulo), stdin))
        return ERROR;
    eliminarFinDeLinea(t.titulo);

    if (strlen(t.titulo) == 0)
        return ERROR;

    do
    {
        printf("Ingrese genero (ACCION/DRAMA/COMEDIA/TERROR): ");
        if (!fgets(t.genero, sizeof(t.genero), stdin))
            return ERROR;
        eliminarFinDeLinea(t.genero);
        normalizarGenero(t.genero);
        validar = generoValido(t.genero) ? OK : ERROR;
        if (validar == ERROR)
        {
            strcpy(error.tipo_error, "Genero");
            error.id_titulo = t.id;
            error.fecha = *fecha;
            strncpy(error.titulo, t.titulo, 60);
            if (asegurar_capacidad_errores_titulos(errores, capacidad_errores, *cantidad_errores + 1) == OK)
            {
                (*errores)[*cantidad_errores] = error;
                (*cantidad_errores)++;
            }
            printf("Ingreso Incorrecto.\n");
        }
    }
    while (validar == ERROR);

    do
    {
        printf("Ingrese stock (>=0): ");
        if (scanf("%d", &t.stock) != 1)
            return ERROR;
        limpiarBuffer();
        validar = stockValido(t.stock) ? OK : ERROR;
        if (validar == ERROR)
        {
            strcpy(error.tipo_error, "Stock");
            error.id_titulo = t.id;
            error.fecha = *fecha;
            strncpy(error.titulo, t.titulo, 60);
            if (asegurar_capacidad_errores_titulos(errores, capacidad_errores, *cantidad_errores + 1) == OK)
            {
                (*errores)[*cantidad_errores] = error;
                (*cantidad_errores)++;
            }
            printf("Ingreso Incorrecto.\n");
        }
    }
    while (validar == ERROR);

    t.estado = 'A';

    if (asegurar_capacidad_titulos(vec, capacidad, *cantidad + 1) == ERROR)
        return ERROR;

    reg.id = t.id;
    reg.nro_reg = (unsigned)(*cantidad);
    if (indice_insertar(ind, &reg, sizeof(t_reg_indice_titulo), cmp_por_id) == ERROR)
        return ERROR;

    (*vec)[*cantidad] = t;
    (*cantidad)++;
    return OK;
}

int BajaTitulo(t_titulos *vec, t_indice *ind)
{
    int id;
    t_reg_indice_titulo clave;
    int pos;

    printf("ID de titulo a dar de baja: ");
    if (scanf("%d", &id) != 1)
        return ERROR;
    limpiarBuffer();

    clave.id = id;
    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice_titulo), cmp_por_id);
    if (pos == NO_EXISTE)
        return ERROR;

    vec[((t_reg_indice_titulo *)ind->vindice)[pos].nro_reg].estado = 'B';
    return indice_eliminar(ind, &clave, sizeof(t_reg_indice_titulo), cmp_por_id);
}

int ModificacionTitulo(t_titulos *vec, t_indice *ind,
                       t_error_titulo **errores, size_t *cantidad_errores, size_t *capacidad_errores, const t_fecha *fecha)
{
    int id;
    t_reg_indice_titulo clave;
    int pos;
    char aux;
    t_titulos *t;
    int validar;
    t_error_titulo error;

    printf("\n=== MODIFICACION DE TITULO ===\n");
    printf("Ingrese ID a modificar: ");
    if (scanf("%d", &id) != 1)
        return ERROR;
    limpiarBuffer();

    clave.id = id;
    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice_titulo), cmp_por_id);
    if (pos == NO_EXISTE)
        return ERROR;

    t = &vec[((t_reg_indice_titulo *)ind->vindice)[pos].nro_reg];
    printf("Modificando: %s\n", t->titulo);

    preguntarCambio("titulo", &aux);
    if (toupper((unsigned char)aux) == 'S')
    {
        if (!fgets(t->titulo, sizeof(t->titulo), stdin))
            return ERROR;
        eliminarFinDeLinea(t->titulo);
    }

    preguntarCambio("genero", &aux);
    if (toupper((unsigned char)aux) == 'S')
    {
        do
        {
            printf("Ingrese genero (ACCION/DRAMA/COMEDIA/TERROR): ");
            if (!fgets(t->genero, sizeof(t->genero), stdin))
                return ERROR;
            eliminarFinDeLinea(t->genero);
            normalizarGenero(t->genero);
            validar = generoValido(t->genero) ? OK : ERROR;
            if (validar == ERROR)
            {
                memset(&error, 0, sizeof(error));
                strcpy(error.tipo_error, "Genero");
                error.id_titulo = t->id;
                error.fecha = *fecha;
                strncpy(error.titulo, t->titulo, 60);
                
                if (asegurar_capacidad_errores_titulos(errores, capacidad_errores, *cantidad_errores + 1) == OK)
                {
                    (*errores)[*cantidad_errores] = error;
                    (*cantidad_errores)++;
                }
                printf("Ingreso Incorrecto.\n");
            }
        }
        while (validar == ERROR);
    }

    preguntarCambio("stock", &aux);
    if (toupper((unsigned char)aux) == 'S')
    {
        do
        {
            printf("Ingrese stock (>=0): ");
            if (scanf("%d", &t->stock) != 1)
                return ERROR;
            limpiarBuffer();
            validar = stockValido(t->stock) ? OK : ERROR;
            if (validar == ERROR)
            {
                memset(&error, 0, sizeof(error));
                strcpy(error.tipo_error, "Stock");
                error.id_titulo = t->id;
                error.fecha = *fecha;
                strncpy(error.titulo, t->titulo, 60);
                
                if (asegurar_capacidad_errores_titulos(errores, capacidad_errores, *cantidad_errores + 1) == OK)
                {
                    (*errores)[*cantidad_errores] = error;
                    (*cantidad_errores)++;
                }
                printf("Ingreso Incorrecto.\n");
            }
        }
        while (validar == ERROR);
    }

    return OK;
}

int MostrarInfoTitulo(t_titulos *vec, t_indice *ind)
{
    int id;
    t_reg_indice_titulo clave;
    int pos;

    printf("\n============= INFORMACION DE TITULO =============\n");
    printf("Ingrese ID a visualizar: ");
    if (scanf("%d", &id) != 1)
        return ERROR;
    limpiarBuffer();

    clave.id = id;
    pos = indice_buscar(ind, &clave, ind->cantidad_elementos_actual, sizeof(t_reg_indice_titulo), cmp_por_id);
    if (pos == NO_EXISTE)
        return ERROR;

    imprimir_titulo(&vec[((t_reg_indice_titulo *)ind->vindice)[pos].nro_reg]);
    return OK;
}



static int asegurar_capacidad_alquileres(t_alquiler **vec, size_t *capacidad, size_t necesaria)
{
    size_t nueva_capacidad;
    t_alquiler *aux;

    if (necesaria <= *capacidad)
        return OK;

    nueva_capacidad = (*capacidad == 0) ? CANT_ELEMENTOS : *capacidad;
    while (nueva_capacidad < necesaria)
        nueva_capacidad = (size_t)((double)nueva_capacidad * INCREMENTO) + 1;

    aux = realloc(*vec, nueva_capacidad * sizeof(t_alquiler));
    if (!aux)
        return ERROR;

    *vec = aux;
    *capacidad = nueva_capacidad;
    return OK;
}

static int contar_alquileres_activos(const t_alquiler *vec_alquileres, size_t cantidad, long dni)
{
    size_t i;
    int contador = 0;

    for (i = 0; i < cantidad; i++)
    {
        if (vec_alquileres[i].dni == dni && vec_alquileres[i].estado == 'A')
            contador++;
    }

    return contador;
}

static int buscar_alquiler(const t_alquiler *vec_alquileres, size_t cantidad, long dni, int id_titulo)
{
    size_t i;

    for (i = 0; i < cantidad; i++)
    {
        if (vec_alquileres[i].dni == dni && vec_alquileres[i].id_titulo == id_titulo)
            return (int)i;
    }

    return NO_EXISTE;
}

int AlquilerTitulo(t_miembro *vec_miembros, t_indice *ind_miembros,
                   t_titulos *vec_titulos, t_indice *ind_titulos,
                   t_alquiler **vec_alquileres, size_t *cantidad_alquileres, size_t *capacidad_alquileres)
{
    long dni;
    int id_titulo;
    t_reg_indice clave_miembro;
    t_reg_indice_titulo clave_titulo;
    int pos_miembro, pos_titulo;
    int pos_alquiler;
    t_miembro *miembro;
    t_titulos *titulo;
    int alquileres_activos;

    printf("\n============= ALQUILER DE TITULO =============\n");

    printf("Ingrese DNI del miembro: ");
    if (scanf("%ld", &dni) != 1)
    {
        limpiarBuffer();
        return ERROR;
    }
    limpiarBuffer();

    clave_miembro.dni = dni;
    pos_miembro = indice_buscar(ind_miembros, &clave_miembro, ind_miembros->cantidad_elementos_actual,
                                sizeof(t_reg_indice), cmp_por_dni);
    if (pos_miembro == NO_EXISTE)
    {
        printf("Error: Miembro no encontrado.\n");
        return ERROR;
    }

    miembro = &vec_miembros[((t_reg_indice *)ind_miembros->vindice)[pos_miembro].nro_reg];

    if (toupper((unsigned char)miembro->estado) != 'A')
    {
        printf("Error: Miembro no activo.\n");
        return ERROR;
    }

    printf("Ingrese ID del titulo: ");
    if (scanf("%d", &id_titulo) != 1)
    {
        limpiarBuffer();
        return ERROR;
    }
    limpiarBuffer();

    clave_titulo.id = id_titulo;
    pos_titulo = indice_buscar(ind_titulos, &clave_titulo, ind_titulos->cantidad_elementos_actual,
                               sizeof(t_reg_indice_titulo), cmp_por_id);
    if (pos_titulo == NO_EXISTE)
    {
        printf("Error: Titulo no encontrado.\n");
        return ERROR;
    }

    titulo = &vec_titulos[((t_reg_indice_titulo *)ind_titulos->vindice)[pos_titulo].nro_reg];

    if (toupper((unsigned char)titulo->estado) != 'A')
    {
        printf("Error: Titulo no disponible.\n");
        return ERROR;
    }

    if (titulo->stock <= 0)
    {
        printf("Error: No hay stock disponible.\n");
        return ERROR;
    }

    alquileres_activos = contar_alquileres_activos(*vec_alquileres, *cantidad_alquileres, dni);

    if (strcmpi(miembro->plan, "BASIC") == 0 && alquileres_activos >= 2)
    {
        printf("Error: Miembros BASIC no pueden alquilar mas de 2 peliculas simultaneamente.\n");
        return ERROR;
    }

    pos_alquiler = buscar_alquiler(*vec_alquileres, *cantidad_alquileres, dni, id_titulo);

    if (pos_alquiler != NO_EXISTE)
    {
        (*vec_alquileres)[pos_alquiler].cantidad_alquileres++;
        (*vec_alquileres)[pos_alquiler].estado = 'A';
    }
    else
    {
        if (asegurar_capacidad_alquileres(vec_alquileres, capacidad_alquileres, *cantidad_alquileres + 1) == ERROR)
            return ERROR;

        (*vec_alquileres)[*cantidad_alquileres].dni = dni;
        (*vec_alquileres)[*cantidad_alquileres].id_titulo = id_titulo;
        (*vec_alquileres)[*cantidad_alquileres].cantidad_alquileres = 1;
        (*vec_alquileres)[*cantidad_alquileres].estado = 'A';
        (*cantidad_alquileres)++;
    }

    titulo->stock--;

    printf("\nAlquiler realizado exitosamente.\n");
    printf("Miembro: %s\n", miembro->nya);
    printf("Titulo: %s\n", titulo->titulo);
    printf("Stock restante: %d\n", titulo->stock);

    return OK;
}
