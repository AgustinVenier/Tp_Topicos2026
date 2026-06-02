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

static void imprimir_miembro(const t_miembro *m)
{
    printf("DNI: %ld\n", m->dni);
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

void menu(t_miembro **vec, size_t *cantidad, size_t *capacidad, t_indice *ind, const t_fecha *fecha)
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
                 "k. Salir\n",
                 "abcdefghijk"
             );

        switch(op)
        {
        case 'a':
            resultado = AltaMiembro(vec, cantidad, capacidad, ind, fecha);
            printf(resultado == ERROR ? "\nError al dar de alta el miembro.\n" : "\nAlta realizada con exito.\n");
            break;

        case 'b':
        case 'd':
        case 'f':
        case 'h':
            printf("\nOpcion no implementada en esta entrega.\n");
            break;

        case 'c':
            resultado = BajaMiembro(*vec, ind);
            printf(resultado == ERROR ? "\nError al dar la baja del miembro.\n" : "\nBaja realizada con exito.\n");
            break;

        case 'e':
            resultado = ModificacionMiembro(*vec, ind, fecha);
            printf(resultado == ERROR ? "\nError al modificar el miembro.\n" : "\nModificacion realizada con exito.\n");
            break;

        case 'g':
            resultado = MostrarInfoMiembro(*vec, ind);
            printf(resultado == ERROR ? "\nError al mostrar\n" : "\nRealizada con exito.\n");
            system("pause");
            break;

        case 'i':
            resultado = ListadoXDNI(*vec, ind);
            printf(resultado == ERROR ? "\nError al listar\n" : "\nRealizada con exito.\n");
            system("pause");
            break;

        case 'j':
            resultado = ListadoXPlan(*vec, ind);
            printf(resultado == ERROR ? "\nError al Listar\n" : "\nRealizada con exito.\n");
            system("pause");
            break;
        }
    }
    while(op != 'k');
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

int AltaMiembro(t_miembro **vec, size_t *cantidad, size_t *capacidad, t_indice *ind, const t_fecha *fecha)
{
    t_miembro m;
    t_reg_indice reg;
    long dni;
    int validar;

    memset(&m, 0, sizeof(m));

    printf("\nIngrese DNI: ");
    if (scanf("%ld", &dni) != 1)
    {
        limpiarBuffer();
        return ERROR;
    }
    limpiarBuffer();

    if (!dniValido(dni))
        return ERROR;

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
            printf("Ingreso Incorrecto.\n");
    }
    while (validar == ERROR);

    do
    {
        printf("Ingrese fecha de nacimiento (dd/mm/aaaa): ");
        if (scanf("%d/%d/%d", &m.fecha_nac.dia, &m.fecha_nac.mes, &m.fecha_nac.anio) != 3)
            return ERROR;
        limpiarBuffer();
        validar = fNacValido(&m.fecha_nac, fecha);
        if(validar == ERROR)
            printf("Ingreso Incorrecto.\n");
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
            printf("Ingreso Incorrecto.\n");
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
            printf("Ingreso Incorrecto.\n");
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
            printf("Ingreso Incorrecto.\n");
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
            printf("Ingreso Incorrecto.\n");
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
            printf("Ingreso Incorrecto.\n");
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

int ModificacionMiembro(t_miembro *vec, t_indice *ind, const t_fecha *fecha)
{
    long dni;
    t_reg_indice clave;
    int pos;
    char aux;
    t_miembro *m;
    int validar;

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
