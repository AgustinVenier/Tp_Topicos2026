#include "validaciones.h"

void ingresarFechaProceso(t_fecha *fecha_proceso)
{
    int estado;

    printf("Ingrese fecha del proceso (DD/MM/AA): ");

    do
    {
        scanf("%d/%d/%d", &fecha_proceso->d, &fecha_proceso->m, &fecha_proceso->a);

        estado = validarFecha(*fecha_proceso);

        if(estado == ERROR)
        {
            printf("Error. Ingrese nuevamente: ");
            fflush(stdin);
        }

    } while(estado == ERROR);
}

int validarFecha(t_fecha f){
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
