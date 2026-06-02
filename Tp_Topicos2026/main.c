#include <stdio.h>
#include <stdlib.h>

#include "archivos.h"
#include "indice.h"
#include "menu.h"
#include "validaciones.h"

#define RUTA_TEXTO "miembros-VC.txt"
#define RUTA_TEXTO_PRUEBAS "miembros-pruebas.csv"
#define RUTA_BINARIO "ArchivoBinario/miembros-VC.dat"
#define RUTA_TITULOS_CSV "titulos.csv"
#define RUTA_TITULOS_PRUEBAS "titulos-pruebas.csv"
#define RUTA_TITULOS_BIN "ArchivoBinario/titulos-VC.dat"

int main(void)
{
    t_fecha fecha_proceso;
    t_indice indice_miembros = {0};
    t_indice indice_titulos = {0};
    t_miembro *vec_miembros = NULL;
    t_titulos *vec_titulos = NULL;
    t_alquiler *vec_alquileres = NULL;
    size_t cantidad_miembros = 0;
    size_t capacidad_miembros = 0;
    size_t cantidad_titulos = 0;
    size_t capacidad_titulos = 0;
    size_t cantidad_alquileres = 0;
    size_t capacidad_alquileres = 0;
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
        indice_crear(&indice_miembros, CANT_ELEMENTOS, sizeof(t_reg_indice));
        if (indice_cargar(RUTA_BINARIO, &indice_miembros, sizeof(t_reg_indice), cmp_por_dni) == ERROR)
        {
            free(vec_miembros);
            indice_vaciar(&indice_miembros);
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

        indice_crear(&indice_miembros, CANT_ELEMENTOS, sizeof(t_reg_indice));
        if (cantidad_miembros > 0 && miembros_construir_indice(vec_miembros, cantidad_miembros, &indice_miembros) == ERROR)
        {
            free(vec_miembros);
            indice_vaciar(&indice_miembros);
            return EXIT_FAILURE;
        }
    }

    indice_crear(&indice_titulos, CANT_ELEMENTOS, sizeof(t_reg_indice_titulo));
    if (titulos_cargar_desde_texto(RUTA_TITULOS_CSV, &vec_titulos, &cantidad_titulos, &capacidad_titulos) == ERROR)
    {
        if (titulos_cargar_desde_texto(RUTA_TITULOS_PRUEBAS, &vec_titulos, &cantidad_titulos, &capacidad_titulos) != OK)
            printf("No se pudo cargar titulos. Se continuara sin ellos.\n");
    }
    if (cantidad_titulos > 0 && titulos_construir_indice(vec_titulos, cantidad_titulos, &indice_titulos) == ERROR)
        printf("No se pudo construir indice de titulos.\n");

    menu(&vec_miembros, &cantidad_miembros, &capacidad_miembros, &indice_miembros,
         &vec_titulos, &cantidad_titulos, &capacidad_titulos, &indice_titulos,
         &vec_alquileres, &cantidad_alquileres, &capacidad_alquileres,
         &fecha_proceso);

    if (miembros_guardar_en_binario(RUTA_BINARIO, vec_miembros, cantidad_miembros) == ERROR)
        printf("No se pudo guardar el binario de miembros.\n");
    if (titulos_guardar_en_binario(RUTA_TITULOS_BIN, vec_titulos, cantidad_titulos) == ERROR)
        printf("No se pudo guardar el binario de titulos.\n");

    free(vec_miembros);
    free(vec_titulos);
    free(vec_alquileres);
    indice_vaciar(&indice_miembros);
    indice_vaciar(&indice_titulos);

    return EXIT_SUCCESS;
}
