#include <stdio.h>
#include <stdlib.h>

#include "archivos.h"
#include "indice.h"
#include "menu.h"
#include "validaciones.h"

#define RUTA_TEXTO "miembros-VC.txt"
#define RUTA_TEXTO_PRUEBAS "miembros-pruebas.csv"
#define RUTA_BINARIO "ArchivoBinario/miembros-VC.dat"

int main(void)
{
    t_fecha fecha_proceso;
    t_indice indice = {0};
    t_miembro *vec_miembros = NULL;
    size_t cantidad_miembros = 0;
    size_t capacidad_miembros = 0;
    int desde_binario;

    ingresarFechaProceso(&fecha_proceso);
    {
        int c;

        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }

    desde_binario = (miembros_cargar_desde_binario(RUTA_BINARIO, &vec_miembros, &cantidad_miembros, &capacidad_miembros) == OK);

    if (desde_binario && cantidad_miembros > 0)
    {
        printf("Recuperando datos desde %s\n", RUTA_BINARIO);
        indice_crear(&indice, CANT_ELEMENTOS, sizeof(t_reg_indice));
        if (indice_cargar(RUTA_BINARIO, &indice, sizeof(t_reg_indice), cmp_por_dni) == ERROR)
        {
            free(vec_miembros);
            indice_vaciar(&indice);
            return EXIT_FAILURE;
        }
    }
    else
    {
        if (desde_binario)
        {
            printf("El binario existe pero esta vacio. Se cargara desde %s\n", RUTA_TEXTO);
            free(vec_miembros);
            vec_miembros = NULL;
            cantidad_miembros = 0;
            capacidad_miembros = 0;
        }
        else
        {
            printf("No hay binario previo. Se cargara desde %s\n", RUTA_TEXTO);
        }

        if (miembros_cargar_desde_texto(RUTA_TEXTO, &fecha_proceso, &vec_miembros, &cantidad_miembros, &capacidad_miembros) == ERROR)
        {
            if (miembros_cargar_desde_texto(RUTA_TEXTO_PRUEBAS, &fecha_proceso, &vec_miembros, &cantidad_miembros, &capacidad_miembros) == OK)
                printf("No se encontro %s. Se cargo desde %s\n", RUTA_TEXTO, RUTA_TEXTO_PRUEBAS);
            else
                printf("No se pudo leer el texto inicial. Se continuara con una carga vacia.\n");
        }

        indice_crear(&indice, CANT_ELEMENTOS, sizeof(t_reg_indice));
        if (cantidad_miembros > 0 && miembros_construir_indice(vec_miembros, cantidad_miembros, &indice) == ERROR)
        {
            free(vec_miembros);
            indice_vaciar(&indice);
            return EXIT_FAILURE;
        }
    }

    menu(&vec_miembros, &cantidad_miembros, &capacidad_miembros, &indice, &fecha_proceso);

    if (miembros_guardar_en_binario(RUTA_BINARIO, vec_miembros, cantidad_miembros) == ERROR)
        printf("No se pudo guardar el binario final.\n");

    free(vec_miembros);
    indice_vaciar(&indice);

    return EXIT_SUCCESS;
}
