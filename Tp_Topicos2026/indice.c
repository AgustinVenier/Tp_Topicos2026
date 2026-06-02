#include "indice.h"
#include "validaciones.h"

void indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    if (nmemb == 0)
        nmemb = CANT_ELEMENTOS;

    indice->vindice = malloc(nmemb * tamanyo);
    if(!indice->vindice)
    {
        printf("No se ha podido asignar memoria\n");
        exit(ERROR);
    }
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = nmemb;
}

void indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    size_t nueva_capacidad = (size_t)((double)nmemb * INCREMENTO);

    if (nueva_capacidad <= nmemb)
        nueva_capacidad = nmemb + 1;

    indice->vindice = realloc(indice->vindice, nueva_capacidad * tamanyo);
    if(!indice->vindice)
    {
        printf("No se ha podido asignar memoria\n");
        exit(ERROR);
    }
    indice->cantidad_elementos_maxima = (unsigned)nueva_capacidad;
}
int indice_insertar (t_indice *indice, const void *registro, size_t tamanyo,
                     int (*cmp)(const void *, const void *))
{
    char *base;

    if (!indice || !registro)
        return ERROR;

    base = (char*)indice->vindice;

    if(indice_buscar(indice,registro,indice->cantidad_elementos_actual,tamanyo,cmp)!=NO_EXISTE)
    {
        return ERROR;
    }

    if (indice_lleno(indice) == OK)
    {
        indice_redimensionar(indice,indice->cantidad_elementos_maxima,tamanyo);
        base = (char*)indice->vindice;
    }

    memcpy(base + indice->cantidad_elementos_actual * tamanyo, registro, tamanyo);
    indice->cantidad_elementos_actual+=1;

    qsort(indice->vindice,indice->cantidad_elementos_actual,tamanyo,cmp);
    return OK;
}

int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    if (!indice || !indice->vindice || indice->cantidad_elementos_actual == 0)
        return ERROR;

    int pos = indice_buscar(indice, registro, indice->cantidad_elementos_actual, tamanyo, cmp);

    if (pos == NO_EXISTE)
        return ERROR;

    size_t ult = indice->cantidad_elementos_actual - 1;

    if ((size_t)pos < ult)
    {
        char *base = (char *)indice->vindice;
        memmove(base + pos * tamanyo, base + (pos + 1) * tamanyo, (ult - pos) * tamanyo);
    }

    indice->cantidad_elementos_actual--;
    return OK;
}

int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb,
                   size_t tamanyo, int (*cmp)(const void *, const void *))
{
    if (!indice || !indice->vindice || nmemb == 0)
        return NO_EXISTE;

    return busquedaBinaria(indice->vindice,registro,nmemb,tamanyo,cmp);
}

int indice_vacio(const t_indice *indice)
{
    if(indice->cantidad_elementos_actual == 0)
        return OK;
    return ERROR;
}
int indice_lleno(const t_indice *indice)
{
    if(indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima)
        return OK;
    return ERROR;
}

void indice_vaciar(t_indice *indice)
{
    if (!indice || !indice->vindice)
        return ;
    free(indice->vindice);
    indice->vindice = NULL;
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = 0;
}


int indice_cargar(const char* path, t_indice* indice, size_t tamanyo,
                  int (*cmp)(const void *, const void *))
{
    unsigned pos = 0;
    t_miembro m;
    t_reg_indice reg;

    FILE *arch = fopen(path, "rb");
    if(!arch)
    {
        return ERROR;
    }

    if (!indice->vindice)
        indice_crear(indice, CANT_ELEMENTOS, tamanyo);

    while(fread(&m, sizeof(t_miembro), 1, arch))
    {
        reg.dni = m.dni;
        reg.nro_reg = pos;
        if(toupper((unsigned char)m.estado)!='B')
        {
            indice_insertar(indice,&reg,tamanyo,cmp);
        }
        pos++;
    }
    fclose(arch);
    return OK;
}

int cmp_por_dni(const void *a, const void *b)
{
    const t_reg_indice *r1 = (const t_reg_indice *)a;
    const t_reg_indice *r2 = (const t_reg_indice *)b;

    if (r1->dni < r2->dni)
        return -1;
    if (r1->dni > r2->dni)
        return 1;
    return 0;
}

int cmp_por_id(const void *a, const void *b)
{
    const t_reg_indice_titulo *r1 = (const t_reg_indice_titulo *)a;
    const t_reg_indice_titulo *r2 = (const t_reg_indice_titulo *)b;

    if (r1->id < r2->id)
        return -1;
    if (r1->id > r2->id)
        return 1;
    return 0;
}

int busquedaBinaria(const void *vec, const void *buscado, unsigned cantelem, size_t tamanyo, int(*cmp)(const void *, const void*))
{
    int i = 0, f = (int)cantelem - 1, medio, res;
    const char *base = (const char *)vec;

    while(i <= f)
    {
        medio = (i + f)/2;
        res = cmp(buscado, base + (medio * tamanyo));

        if(!res)
        {
            //Devuelve la posicion donde se encontro el elemento
            return medio;
        }
        else if(res < 0)
        {
            f = medio - 1;
        }
        else
        {
            i = medio + 1;
        }
    }
    return NO_EXISTE;
}
