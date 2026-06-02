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

static int asegurar_capacidad_titulos(t_titulo **vec, size_t *capacidad, size_t necesaria)
{
    size_t nueva_capacidad;
    t_titulo *aux;

    if (necesaria <= *capacidad)
        return OK;

    nueva_capacidad = (*capacidad == 0) ? CANT_ELEMENTOS : *capacidad;
    while (nueva_capacidad < necesaria)
        nueva_capacidad = (size_t)((double)nueva_capacidad * INCREMENTO) + 1;

    aux = realloc(*vec, nueva_capacidad * sizeof(t_titulo));
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
                               t_titulo **vec, size_t *cantidad, size_t *capacidad)
{
    FILE *archivo;
    char linea[BUFFER + 200];
    t_titulo titulo;
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
                                 t_titulo **vec, size_t *cantidad, size_t *capacidad)
{
    FILE *archivo;
    t_titulo titulo;

    archivo = fopen(nombre_binario, "rb");
    if (!archivo)
        return ERROR;

    while (fread(&titulo, sizeof(t_titulo), 1, archivo) == 1)
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
                               const t_titulo *vec, size_t cantidad)
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

    if (cantidad > 0 && fwrite(vec, sizeof(t_titulo), cantidad, archivo) != cantidad)
    {
        fclose(archivo);
        return ERROR;
    }

    fclose(archivo);
    return OK;
}

int titulos_construir_indice(const t_titulo *vec, size_t cantidad, t_indice *indice)
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
