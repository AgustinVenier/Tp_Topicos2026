#include "estructuras.h"
#include "validaciones.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void ingresarFechaProceso(t_fecha *fecha_proceso)
{
    int estado;

    printf("Ingrese fecha del proceso (DD/MM/AA): ");

    do
    {
        scanf("%d/%d/%d", &fecha_proceso->dia, &fecha_proceso->mes, &fecha_proceso->anio);

        estado = validarFecha(fecha_proceso);

        if(estado == ERROR)
        {
            printf("Error. Ingrese nuevamente: ");
            fflush(stdin);
        }

    } while(estado == ERROR);
}

int validarFecha(t_fecha *f){
    int diasEnMes;

    if (f->anio < 1900)
    {
        return ERROR;
    }


    if (f->mes < 1 || f->mes > 12)
    {
        return ERROR;
    }

    if (f->mes == 1 || f->mes == 3 || f->mes == 5 || f->mes == 7 || f->mes == 8 || f->mes == 10 || f->mes == 12)
        diasEnMes = 31;
    else
    {
        if (f->mes == 4 || f->mes == 6 || f->mes == 9 || f->mes == 11)
            diasEnMes = 30;
        else // Febrero
        {
            if ((f->anio % 4 == 0 && f->anio % 100 != 0) || (f->anio % 400 == 0))
                diasEnMes = 29;  // Anio bisiesto
            else
                diasEnMes = 28;
        }
    }

    if (f->dia < 1 || f->dia > diasEnMes)
    {
        return ERROR;
    }


    return OK;
}

int validarEmail(const char s[]) {

    char *arroba;
    char *p;
    int i;
    int len;

    len = strlen(s);

    // buscamos @
    arroba = strchr(s, '@');

    // validaciones básicas
    if (arroba == NULL) {
        return ERROR;
    }

    // verificamos que haya un solo @
    if (strchr(arroba + 1, '@') != NULL) {
        return ERROR;
    }

    // no puede empezar ni terminar con @
    if (s[0] == '@' || s[len - 1] == '@') {
        return ERROR;
    }

    // después de @ no puede haber punto
    if (*(arroba + 1) == '.') {
        return ERROR;
    }

     // VALIDAMOS PARTE LOCAL

    for (p = (char *)s; p < arroba; p++) {

        if (!isalnum(*p) && *p != '.') {
            return ERROR;
        }
    }

    // VALIDAMOS DOMINIO

    p = arroba + 1;

    // debe existir al menos un punto en dominio
    if (strchr(p, '.') == NULL) {
        return ERROR;
    }

    while (*p != '\0') {

        if (!isalnum(*p) && *p != '.') {
            return ERROR;
        }

        p++;
    }

    // VALIDAMOS EXTENSIÓN

    // buscamos .com dentro del dominio
    char *pstr = strstr(arroba + 1, ".com");

    if (pstr == NULL) {
        return ERROR;
    }

    // después de ".com"
    char *despues = pstr + 4;

    // puede terminar ahí
    if (*despues == '\0') {
        return OK;
    }

    // o puede seguir ".ar"
    if (strcmp(despues, ".ar") == 0) {
        return OK;
    }

    return ERROR;
}
