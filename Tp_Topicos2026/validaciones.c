#include "validaciones.h"
#include <stdio.h>

void obtenerCuil(const char *dni, char sexo, char *cuil)
{
    int i;
    int count = 0;
    int resto;
    int digito;

    char *ptr1;
    char *ptr2;
    char *ptr3;

    if (sexo == 'M') {
        strcpy(cuil, "20");
    }
    else if (sexo == 'F') {
        strcpy(cuil, "27");
    }
    else {
        strcpy(cuil, "30");
    }

    strcat(cuil, dni);

    ptr1 = cuil;
    ptr2 = cuil + 6;
    ptr3 = cuil + 8;

    for (i = 0; i < 4; i++) {
        count += (*(ptr1 + i) - '0') * (*(ptr2 - i) - '0');
    }

    for (i = 0; i < 6; i++) {
        count += (*(ptr1 + 4 + i) - '0') * (*(ptr3 - i) - '0');
    }

    resto = count % 11;

    if (resto == 0) {
        digito = 0;
    }
    else if (resto == 1) {
        if (sexo == 'M') {
            cuil[0] = '2';
            cuil[1] = '3';
            digito = 9;
        }
        else if (sexo == 'F') {
            cuil[0] = '2';
            cuil[1] = '3';
            digito = 4;
        }
        else {
            digito = 11 - resto;
        }
    }
    else {
        digito = 11 - resto;
    }

    cuil[10] = digito + '0';
    cuil[11] = '\0';
}

void ingresarFechaProceso(t_fecha *fecha_proceso)
{
    int estado;

    printf("Ingrese fecha del proceso (DD/MM/AAAA): ");

    do
    {
        scanf("%d/%d/%d", &fecha_proceso->dia, &fecha_proceso->mes, &fecha_proceso->anio);
        estado = validarFecha(fecha_proceso);
        if(estado == ERROR)
            printf("Error. Ingrese nuevamente: ");
    } while(estado == ERROR);
}

int validarFecha(t_fecha *f){
    int diasEnMes;

    if (f->anio < 1900)
        return ERROR;

    if (f->mes < 1 || f->mes > 12)
        return ERROR;

    if (f->mes == 1 || f->mes == 3 || f->mes == 5 || f->mes == 7 || f->mes == 8 || f->mes == 10 || f->mes == 12)
        diasEnMes = 31;
    else if (f->mes == 4 || f->mes == 6 || f->mes == 9 || f->mes == 11)
        diasEnMes = 30;
    else if ((f->anio % 4 == 0 && f->anio % 100 != 0) || (f->anio % 400 == 0))
        diasEnMes = 29;
    else
        diasEnMes = 28;

    if (f->dia < 1 || f->dia > diasEnMes)
        return ERROR;

    return OK;
}

int compararFecha(const t_fecha *a, const t_fecha *b)
{
    if (a->anio != b->anio)
        return a->anio - b->anio;
    if (a->mes != b->mes)
        return a->mes - b->mes;
    return a->dia - b->dia;
}

int validarEmail(const char s[]) {

    const char *arroba;
    const char *p;
    size_t len;

    len = strlen(s);
    arroba = strchr(s, '@');

    if (arroba == NULL) {
        return ERROR;
    }
    if (strchr(arroba + 1, '@') != NULL) {
        return ERROR;
    }
    if (s[0] == '@' || s[len - 1] == '@') {
        return ERROR;
    }
    if (*(arroba + 1) == '.') {
        return ERROR;
    }

    for (p = (char *)s; p < arroba; p++) {

        if (!isalnum((unsigned char)*p) && *p != '.') {
            return ERROR;
        }
    }

    p = arroba + 1;
    if (strchr(p, '.') == NULL) {
        return ERROR;
    }

    while (*p != '\0') {

        if (!isalnum((unsigned char)*p) && *p != '.') {
            return ERROR;
        }

        p++;
    }

    {
        const char *pstr = strstr(arroba + 1, ".com");
        const char *despues;

        if (pstr == NULL) {
            return ERROR;
        }

        despues = pstr + 4;
        if (*despues == '\0') {
            return OK;
        }

        if (strcmp(despues, ".ar") == 0) {
            return OK;
        }

        return ERROR;
    }
}
char * normalizar(char * cad) //Normaliza el nombre segun pide el TP
{
    char * lect = cad, * esc = cad;
    int primer_letra_palabra;
    int posPal = 0;
    while(*lect)
    {
        while(*lect && (isspace((unsigned char)*lect) || *lect == ','))
            lect++;
        if(*lect)
        {
            posPal++;
            if(posPal == 2)
            {
                *esc++ = ',';
                *esc++ = ' ';
            }
            else if(posPal > 2)
            {
                *esc++ = ' ';
            }
            primer_letra_palabra = 1;
            while(*lect && !isspace((unsigned char)*lect) && *lect != ',')
            {
                *esc = primer_letra_palabra ? toupper((unsigned char)*lect) : tolower((unsigned char)*lect);
                primer_letra_palabra = 0;
                esc++;
                lect++;
            }
        }
    }
    *esc = '\0';
    return cad;
}

static int validarFechaNacimiento(const t_fecha* nacimiento,const t_fecha * t_proceso,int cant_anios)
{
    int edad = t_proceso->anio - nacimiento->anio;

    if (t_proceso->mes < nacimiento->mes || (t_proceso->mes == nacimiento->mes && t_proceso->dia < nacimiento->dia))
        edad--;

    return (edad < cant_anios);
}

int validarFechaCategoria(const char * categ,const t_fecha* fechaNac,const t_fecha * f_proceso)
{
    if(validarFechaNacimiento(fechaNac,f_proceso,18))
    {
        if(strcmpi(categ, "MENOR") == 0)
            return OK;
        else
            return ERROR;
    }
    if(strcmpi(categ, "ADULTO") == 0)
        return OK;
    return ERROR;
}

int fNacValido(const t_fecha* fechaNac,const t_fecha * f_proceso)
{
    if (validarFecha((t_fecha *)fechaNac) == ERROR)
        return ERROR;

    if (validarFechaNacimiento(fechaNac,f_proceso,ANIOS_DESDE_NACIMIENTO) == ERROR)
        return ERROR;

    return OK;
}

int fAfiliacionValido(const t_fecha* fechaAfi, const t_fecha* fechaProc, const t_fecha* fechaNac)
{
    if (validarFecha((t_fecha *)fechaAfi) == ERROR)
        return ERROR;

    if (compararFecha(fechaAfi, fechaProc) > 0)
        return ERROR;

    if (compararFecha(fechaAfi, fechaNac) < 0)
        return ERROR;

    return OK;
}

int fUltCoutaValido(const t_fecha* fechaCuota, const t_fecha* fechaAfi, const t_fecha* fechaProc)
{
    if (validarFecha((t_fecha *)fechaCuota) == ERROR)
        return ERROR;

    if (compararFecha(fechaCuota, fechaAfi) < 0)
        return ERROR;

    if (compararFecha(fechaCuota, fechaProc) > 0)
        return ERROR;

    return OK;
}
