#include "archivos.h"
#include "validaciones.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir_if_needed(path) _mkdir(path)
#else
#include <sys/stat.h>
#define mkdir_if_needed(path) mkdir(path, 0777)
#endif

static int asegurar_capacidad(t_miembro **vec, size_t *capacidad, size_t necesaria)
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


static int parsear_miembro(const char *linea, t_miembro *miembro)
{
    char copia[BUFFER + 200];
    char *fin;
    char dni_str[20];

    strncpy(copia, linea, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';

    fin = strpbrk(copia, "\r\n");
    if (fin)
        *fin = '\0';

    memset(miembro, 0, sizeof(*miembro));

    if (sscanf(copia,
               "%ld|%60[^|]|%d/%d/%d|%c|%d/%d/%d|%10[^|]|%d/%d/%d|%c|%10[^|]|%30[^\r\n]",
               &miembro->dni,
               miembro->nya,
               &miembro->fecha_nac.dia, &miembro->fecha_nac.mes, &miembro->fecha_nac.anio,
               &miembro->sexo,
               &miembro->fecha_afi.dia, &miembro->fecha_afi.mes, &miembro->fecha_afi.anio,
               miembro->cat,
               &miembro->fecha_cuota.dia, &miembro->fecha_cuota.mes, &miembro->fecha_cuota.anio,
               &miembro->estado,
               miembro->plan,
               miembro->email) != 16)
    {
        return ERROR;
    }

    snprintf(dni_str, sizeof(dni_str), "%ld", miembro->dni);
    obtenerCuil(dni_str, miembro->sexo, miembro->cuil);

    return OK;
}

static int validar_miembro(const t_miembro *miembro, const t_fecha *fecha_proceso)
{
    if (!dniValido(miembro->dni))
        return ERROR;

    if (!sexValido(miembro->sexo))
        return ERROR;

    if (!estadoValido(miembro->estado))
        return ERROR;

    if (!planValido(miembro->plan))
        return ERROR;

    if (fNacValido(&miembro->fecha_nac, fecha_proceso) == ERROR)
        return ERROR;

    if (fAfiliacionValido(&miembro->fecha_afi, fecha_proceso, &miembro->fecha_nac) == ERROR)
        return ERROR;

    if (fUltCoutaValido(&miembro->fecha_cuota, &miembro->fecha_afi, fecha_proceso) == ERROR)
        return ERROR;

    if (validarFechaCategoria(miembro->cat, &miembro->fecha_nac, fecha_proceso) == ERROR)
        return ERROR;

    if (validarEmail(miembro->email) == ERROR)
        return ERROR;

    return OK;
}

int miembros_cargar_desde_texto(const char *nombre_texto, const t_fecha *fecha_proceso,
                                t_miembro **vec, size_t *cantidad, size_t *capacidad)
{
    FILE *archivo;
    char linea[BUFFER + 200];
    t_miembro miembro;

    archivo = fopen(nombre_texto, "rt");
    if (!archivo)
        return ERROR;

    while (fgets(linea, sizeof(linea), archivo))
    {
        if (parsear_miembro(linea, &miembro) == ERROR)
            continue;

        normalizar(miembro.nya);

        if (validar_miembro(&miembro, fecha_proceso) == ERROR)
            continue;

        if (asegurar_capacidad(vec, capacidad, *cantidad + 1) == ERROR)
        {
            fclose(archivo);
            return ERROR;
        }

        (*vec)[*cantidad] = miembro;
        (*cantidad)++;
    }

    fclose(archivo);
    return OK;
}

int miembros_cargar_desde_binario(const char *nombre_binario,
                                  t_miembro **vec, size_t *cantidad, size_t *capacidad)
{
    FILE *archivo;
    t_miembro miembro;

    archivo = fopen(nombre_binario, "rb");
    if (!archivo)
        return ERROR;

    while (fread(&miembro, sizeof(t_miembro), 1, archivo) == 1)
    {
        if (asegurar_capacidad(vec, capacidad, *cantidad + 1) == ERROR)
        {
            fclose(archivo);
            return ERROR;
        }

        (*vec)[*cantidad] = miembro;
        (*cantidad)++;
    }

    fclose(archivo);
    return OK;
}

int miembros_guardar_en_binario(const char *nombre_binario,
                                const t_miembro *vec, size_t cantidad)
{
    FILE *archivo;
    char carpeta[64];
    const char *slash;

    slash = strrchr(nombre_binario, '/');
    if (slash)
    {
        size_t len = (size_t)(slash - nombre_binario);
        if (len >= sizeof(carpeta))
            return ERROR;

        memcpy(carpeta, nombre_binario, len);
        carpeta[len] = '\0';
        mkdir_if_needed(carpeta);
    }

    archivo = fopen(nombre_binario, "wb");
    if (!archivo)
        return ERROR;

    if (cantidad > 0 && fwrite(vec, sizeof(t_miembro), cantidad, archivo) != cantidad)
    {
        fclose(archivo);
        return ERROR;
    }

    fclose(archivo);
    return OK;
}

int miembros_construir_indice(const t_miembro *vec, size_t cantidad, t_indice *indice)
{
    size_t i;
    t_reg_indice reg;

    if (!indice->vindice)
        indice_crear(indice, CANT_ELEMENTOS, sizeof(t_reg_indice));

    for (i = 0; i < cantidad; i++)
    {
        if (toupper((unsigned char)vec[i].estado) != 'A')
            continue;

        reg.dni = vec[i].dni;
        reg.nro_reg = (unsigned)i;

        if (indice_insertar(indice, &reg, sizeof(t_reg_indice), cmp_por_dni) == ERROR)
            return ERROR;
    }

    return OK;
}

int titulos_cargar_desde_texto(const char *nombre_texto,
                               t_titulos **vec, size_t *cantidad, size_t *capacidad)
{
    FILE *archivo;
    char linea[BUFFER + 200];
    t_titulos titulo;
    int id = 1;

    archivo = fopen(nombre_texto, "rt");
    if (!archivo)
        return ERROR;

    if (!fgets(linea, sizeof(linea), archivo))
    {
        fclose(archivo);
        return OK;
    }

    while (fgets(linea, sizeof(linea), archivo))
    {
        char *fin = strpbrk(linea, "\r\n");
        if (fin)
            *fin = '\0';

        memset(&titulo, 0, sizeof(titulo));
        if (sscanf(linea, "%60[^|]|%20[^|]|%d|%c", titulo.titulo, titulo.genero,
                   &titulo.stock, &titulo.estado) != 4)
        {
            continue;
        }

        normalizarGenero(titulo.titulo);
        normalizarGenero(titulo.genero);

        if (!stockValido(titulo.stock) || !generoValido(titulo.genero) || !estadoValido(titulo.estado))
            continue;

        titulo.id = id++;

        if (asegurar_capacidad_titulos(vec, capacidad, *cantidad + 1) == ERROR)
        {
            fclose(archivo);
            return ERROR;
        }

        (*vec)[*cantidad] = titulo;
        (*cantidad)++;
    }

    fclose(archivo);
    return OK;
}

int titulos_cargar_desde_binario(const char *nombre_binario,
                                 t_titulos **vec, size_t *cantidad, size_t *capacidad)
{
    FILE *archivo;
    t_titulos titulo;

    archivo = fopen(nombre_binario, "rb");
    if (!archivo)
        return ERROR;

    while (fread(&titulo, sizeof(t_titulos), 1, archivo) == 1)
    {
        if (asegurar_capacidad_titulos(vec, capacidad, *cantidad + 1) == ERROR)
        {
            fclose(archivo);
            return ERROR;
        }

        (*vec)[*cantidad] = titulo;
        (*cantidad)++;
    }

    fclose(archivo);
    return OK;
}

int titulos_guardar_en_binario(const char *nombre_binario,
                               const t_titulos *vec, size_t cantidad)
{
    FILE *archivo;
    char carpeta[64];
    const char *slash;

    slash = strrchr(nombre_binario, '/');
    if (slash)
    {
        size_t len = (size_t)(slash - nombre_binario);
        if (len >= sizeof(carpeta))
            return ERROR;

        memcpy(carpeta, nombre_binario, len);
        carpeta[len] = '\0';
        mkdir_if_needed(carpeta);
    }

    archivo = fopen(nombre_binario, "wb");
    if (!archivo)
        return ERROR;

    if (cantidad > 0 && fwrite(vec, sizeof(t_titulos), cantidad, archivo) != cantidad)
    {
        fclose(archivo);
        return ERROR;
    }

    fclose(archivo);
    return OK;
}

int titulos_construir_indice(const t_titulos *vec, size_t cantidad, t_indice *indice)
{
    size_t i;
    t_reg_indice_titulo reg;

    if (!indice->vindice)
        indice_crear(indice, CANT_ELEMENTOS, sizeof(t_reg_indice_titulo));

    for (i = 0; i < cantidad; i++)
    {
        if (toupper((unsigned char)vec[i].estado) != 'A')
            continue;

        reg.id = vec[i].id;
        reg.nro_reg = (unsigned)i;

        if (indice_insertar(indice, &reg, sizeof(t_reg_indice_titulo), cmp_por_id) == ERROR)
            return ERROR;
    }

    return OK;
}

// ============= FUNCIONES DE AUDITORIA =============

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

int miembros_cargar_desde_texto_con_auditoria(const char *nombre_texto, const t_fecha *fecha_proceso,
                                              t_miembro **vec, size_t *cantidad, size_t *capacidad,
                                              t_error_miembro **errores, size_t *cantidad_errores, size_t *capacidad_errores)
{
    FILE *archivo;
    char linea[BUFFER + 200];
    t_miembro miembro;
    t_error_miembro error;

    archivo = fopen(nombre_texto, "rt");
    if (!archivo)
        return ERROR;

    while (fgets(linea, sizeof(linea), archivo))
    {
        memset(&miembro, 0, sizeof(miembro));
        memset(&error, 0, sizeof(error));

        // Intentar parsear
        if (parsear_miembro(linea, &miembro) == ERROR)
        {
            // Error de parseo - registrar en auditoria
            strcpy(error.tipo_error, "Formato");
            error.dni = 0;
            error.fecha = *fecha_proceso;
            strcpy(error.email, "");
            
            if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == ERROR)
            {
                fclose(archivo);
                return ERROR;
            }
            (*errores)[*cantidad_errores] = error;
            (*cantidad_errores)++;
            continue;
        }

        normalizar(miembro.nya);

        // Validar miembro
        if (!dniValido(miembro.dni))
        {
            strcpy(error.tipo_error, "DNI");
            error.dni = miembro.dni;
            error.fecha = *fecha_proceso;
            strcpy(error.email, miembro.email);
        }
        else if (!sexValido(miembro.sexo))
        {
            strcpy(error.tipo_error, "Sexo");
            error.dni = miembro.dni;
            error.fecha = *fecha_proceso;
            strcpy(error.email, miembro.email);
        }
        else if (!estadoValido(miembro.estado))
        {
            strcpy(error.tipo_error, "Estado");
            error.dni = miembro.dni;
            error.fecha = *fecha_proceso;
            strcpy(error.email, miembro.email);
        }
        else if (!planValido(miembro.plan))
        {
            strcpy(error.tipo_error, "Plan");
            error.dni = miembro.dni;
            error.fecha = *fecha_proceso;
            strcpy(error.email, miembro.email);
        }
        else if (fNacValido(&miembro.fecha_nac, fecha_proceso) == ERROR)
        {
            strcpy(error.tipo_error, "Fecha Nacimiento");
            error.dni = miembro.dni;
            error.fecha = *fecha_proceso;
            strcpy(error.email, miembro.email);
        }
        else if (fAfiliacionValido(&miembro.fecha_afi, fecha_proceso, &miembro.fecha_nac) == ERROR)
        {
            strcpy(error.tipo_error, "Fecha Afiliacion");
            error.dni = miembro.dni;
            error.fecha = *fecha_proceso;
            strcpy(error.email, miembro.email);
        }
        else if (fUltCoutaValido(&miembro.fecha_cuota, &miembro.fecha_afi, fecha_proceso) == ERROR)
        {
            strcpy(error.tipo_error, "Fecha Cuota");
            error.dni = miembro.dni;
            error.fecha = *fecha_proceso;
            strcpy(error.email, miembro.email);
        }
        else if (validarFechaCategoria(miembro.cat, &miembro.fecha_nac, fecha_proceso) == ERROR)
        {
            strcpy(error.tipo_error, "Categoria");
            error.dni = miembro.dni;
            error.fecha = *fecha_proceso;
            strcpy(error.email, miembro.email);
        }
        else if (validarEmail(miembro.email) == ERROR)
        {
            strcpy(error.tipo_error, "Email");
            error.dni = miembro.dni;
            error.fecha = *fecha_proceso;
            strcpy(error.email, miembro.email);
        }
        else
        {
            // Miembro valido - agregarlo al vector
            if (asegurar_capacidad(vec, capacidad, *cantidad + 1) == ERROR)
            {
                fclose(archivo);
                return ERROR;
            }

            (*vec)[*cantidad] = miembro;
            (*cantidad)++;
            continue;
        }

        // Si llegamos aqui, es un error - guardarlo en auditoria
        if (error.tipo_error[0] != '\0')
        {
            if (asegurar_capacidad_errores_miembros(errores, capacidad_errores, *cantidad_errores + 1) == ERROR)
            {
                fclose(archivo);
                return ERROR;
            }
            (*errores)[*cantidad_errores] = error;
            (*cantidad_errores)++;
        }
    }

    fclose(archivo);
    return OK;
}

int titulos_cargar_desde_texto_con_auditoria(const char *nombre_texto,
                                             t_titulos **vec, size_t *cantidad, size_t *capacidad,
                                             t_error_titulo **errores, size_t *cantidad_errores, size_t *capacidad_errores,
                                             const t_fecha *fecha_proceso)
{
    FILE *archivo;
    char linea[BUFFER + 200];
    t_titulos titulo;
    t_error_titulo error;
    int id = 1;
    char *fin;

    archivo = fopen(nombre_texto, "rt");
    if (!archivo)
        return ERROR;

    if (!fgets(linea, sizeof(linea), archivo))
    {
        fclose(archivo);
        return OK;
    }

    while (fgets(linea, sizeof(linea), archivo))
    {
        memset(&titulo, 0, sizeof(titulo));
        memset(&error, 0, sizeof(error));

        fin = strpbrk(linea, "\r\n");
        if (fin)
            *fin = '\0';

        if (sscanf(linea, "%60[^|]|%20[^|]|%d|%c", titulo.titulo, titulo.genero,
                   &titulo.stock, &titulo.estado) != 4)
        {
            strcpy(error.tipo_error, "Formato");
            error.id_titulo = 0;
            error.fecha = *fecha_proceso;
            strcpy(error.titulo, "");

            if (asegurar_capacidad_errores_titulos(errores, capacidad_errores, *cantidad_errores + 1) == ERROR)
            {
                fclose(archivo);
                return ERROR;
            }
            (*errores)[*cantidad_errores] = error;
            (*cantidad_errores)++;
            continue;
        }

        normalizarGenero(titulo.titulo);
        normalizarGenero(titulo.genero);

        titulo.id = id++;

        // Validar titulo
        if (!stockValido(titulo.stock))
        {
            strcpy(error.tipo_error, "Stock");
            error.id_titulo = titulo.id;
            error.fecha = *fecha_proceso;
            strncpy(error.titulo, titulo.titulo, 60);
        }
        else if (!generoValido(titulo.genero))
        {
            strcpy(error.tipo_error, "Genero");
            error.id_titulo = titulo.id;
            error.fecha = *fecha_proceso;
            strncpy(error.titulo, titulo.titulo, 60);
        }
        else if (!estadoValido(titulo.estado))
        {
            strcpy(error.tipo_error, "Estado");
            error.id_titulo = titulo.id;
            error.fecha = *fecha_proceso;
            strncpy(error.titulo, titulo.titulo, 60);
        }
        else
        {
            // Titulo valido - agregarlo al vector
            if (asegurar_capacidad_titulos(vec, capacidad, *cantidad + 1) == ERROR)
            {
                fclose(archivo);
                return ERROR;
            }

            (*vec)[*cantidad] = titulo;
            (*cantidad)++;
            continue;
        }

        // Si llegamos aqui, es un error - guardarlo en auditoria
        if (error.tipo_error[0] != '\0')
        {
            if (asegurar_capacidad_errores_titulos(errores, capacidad_errores, *cantidad_errores + 1) == ERROR)
            {
                fclose(archivo);
                return ERROR;
            }
            (*errores)[*cantidad_errores] = error;
            (*cantidad_errores)++;
        }
    }

    fclose(archivo);
    return OK;
}

void mostrar_auditoria_miembros(const t_error_miembro *errores, size_t cantidad)
{
    size_t i, j;
    size_t count[10];
    char tipos[10][51];
    int num_tipos = 0;

    if (cantidad == 0)
    {
        printf("\nNo hay errores registrados en miembros.\n");
        return;
    }

    // Contar incidencias por tipo
    memset(count, 0, sizeof(count));
    memset(tipos, 0, sizeof(tipos));

    for (i = 0; i < cantidad; i++)
    {
        int encontrado = 0;
        for (j = 0; j < (size_t)num_tipos; j++)
        {
            if (strcmp(tipos[j], errores[i].tipo_error) == 0)
            {
                count[j]++;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado && num_tipos < 10)
        {
            strcpy(tipos[num_tipos], errores[i].tipo_error);
            count[num_tipos] = 1;
            num_tipos++;
        }
    }

    // Mostrar tabla
    printf("\n===============================================\n");
    printf("AUDITORIA DE ERRORES - MIEMBROS\n");
    printf("===============================================\n");
    printf("Tipo de Error                    | Cantidad\n");
    printf("-----------------------------------------------\n");

    for (i = 0; i < (size_t)num_tipos; i++)
    {
        printf("%-32s | %8ld\n", tipos[i], (long)count[i]);
    }

    printf("===============================================\n");
    printf("Total de errores: %ld\n", (long)cantidad);
    printf("===============================================\n");
}

void mostrar_auditoria_titulos(const t_error_titulo *errores, size_t cantidad)
{
    size_t i, j;
    size_t count[10];
    char tipos[10][51];
    int num_tipos = 0;

    if (cantidad == 0)
    {
        printf("\nNo hay errores registrados en titulos.\n");
        return;
    }

    // Contar incidencias por tipo
    memset(count, 0, sizeof(count));
    memset(tipos, 0, sizeof(tipos));

    for (i = 0; i < cantidad; i++)
    {
        int encontrado = 0;
        for (j = 0; j < (size_t)num_tipos; j++)
        {
            if (strcmp(tipos[j], errores[i].tipo_error) == 0)
            {
                count[j]++;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado && num_tipos < 10)
        {
            strcpy(tipos[num_tipos], errores[i].tipo_error);
            count[num_tipos] = 1;
            num_tipos++;
        }
    }

    // Mostrar tabla
    printf("\n===============================================\n");
    printf("AUDITORIA DE ERRORES - TITULOS\n");
    printf("===============================================\n");
    printf("Tipo de Error                    | Cantidad\n");
    printf("-----------------------------------------------\n");

    for (i = 0; i < (size_t)num_tipos; i++)
    {
        printf("%-32s | %8ld\n", tipos[i], (long)count[i]);
    }

    printf("===============================================\n");
    printf("Total de errores: %ld\n", (long)cantidad);
    printf("===============================================\n");
}

