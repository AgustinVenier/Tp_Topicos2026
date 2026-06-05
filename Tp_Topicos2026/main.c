#include <stdio.h>
#include <stdlib.h>

#include "archivos.h"
#include "indice.h"
#include "menu.h"
#include "validaciones.h"

#define RUTA_TEXTO "miembros.csv"
#define RUTA_TEXTO_PRUEBAS "miembros.csv"
#define RUTA_BINARIO "ArchivoBinario/miembros-VC.dat"
#define RUTA_TITULOS_CSV "titulos.csv"
#define RUTA_TITULOS_PRUEBAS "titulos.csv"
#define RUTA_TITULOS_BIN "ArchivoBinario/titulos-VC.dat"

int main(void)
{
    t_fecha fecha_proceso;
    t_indice indice_miembros = {0};
    t_indice indice_titulos = {0};
    t_miembro *vec_miembros = NULL;
    t_titulos *vec_titulos = NULL;
    t_alquiler *vec_alquileres = NULL;
    t_error_miembro *errores_miembros = NULL;
    t_error_titulo *errores_titulos = NULL;
    size_t cantidad_miembros = 0;
    size_t capacidad_miembros = 0;
    size_t cantidad_titulos = 0;
    size_t capacidad_titulos = 0;
    size_t cantidad_alquileres = 0;
    size_t capacidad_alquileres = 0;
    size_t cantidad_errores_miembros = 0;
    size_t capacidad_errores_miembros = 0;
    size_t cantidad_errores_titulos = 0;
    size_t capacidad_errores_titulos = 0;
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

        if (miembros_cargar_desde_texto_con_auditoria(RUTA_TEXTO, &fecha_proceso, &vec_miembros, &cantidad_miembros, &capacidad_miembros,
                                                       &errores_miembros, &cantidad_errores_miembros, &capacidad_errores_miembros) == ERROR)
        {
            if (miembros_cargar_desde_texto_con_auditoria(RUTA_TEXTO_PRUEBAS, &fecha_proceso, &vec_miembros, &cantidad_miembros, &capacidad_miembros,
                                                           &errores_miembros, &cantidad_errores_miembros, &capacidad_errores_miembros) == OK)
                printf("No se encontro %s. Se cargo desde %s\n", RUTA_TEXTO, RUTA_TEXTO_PRUEBAS);
            else
                printf("No se pudo leer el texto inicial. Se continuara con una carga vacia.\n");
        }

        indice_crear(&indice_miembros, CANT_ELEMENTOS, sizeof(t_reg_indice));
        if (cantidad_miembros > 0 && miembros_construir_indice(vec_miembros, cantidad_miembros, &indice_miembros) == ERROR)
        {
            free(vec_miembros);
            free(errores_miembros);
            indice_vaciar(&indice_miembros);
            return EXIT_FAILURE;
        }
    }

    int desde_binario_titulos;
    desde_binario_titulos = (titulos_cargar_desde_binario(RUTA_TITULOS_BIN, &vec_titulos, &cantidad_titulos, &capacidad_titulos) == OK);

    indice_crear(&indice_titulos, CANT_ELEMENTOS, sizeof(t_reg_indice_titulo));
    
    if (desde_binario_titulos && cantidad_titulos > 0)
    {
        printf("Recuperando titulos desde %s\n", RUTA_TITULOS_BIN);
        if (titulos_construir_indice(vec_titulos, cantidad_titulos, &indice_titulos) == ERROR)
            printf("No se pudo construir indice de titulos.\n");
    }
    else
    {
        if (desde_binario_titulos)
        {
            printf("El binario de titulos existe pero esta vacio. Se cargara desde %s\n", RUTA_TITULOS_CSV);
            free(vec_titulos);
            vec_titulos = NULL;
            cantidad_titulos = 0;
            capacidad_titulos = 0;
        }
        else
        {
            printf("No hay binario previo de titulos. Se cargara desde %s\n", RUTA_TITULOS_CSV);
        }

        if (titulos_cargar_desde_texto_con_auditoria(RUTA_TITULOS_CSV, &vec_titulos, &cantidad_titulos, &capacidad_titulos,
                                                      &errores_titulos, &cantidad_errores_titulos, &capacidad_errores_titulos, &fecha_proceso) == ERROR)
        {
            if (titulos_cargar_desde_texto_con_auditoria(RUTA_TITULOS_PRUEBAS, &vec_titulos, &cantidad_titulos, &capacidad_titulos,
                                                          &errores_titulos, &cantidad_errores_titulos, &capacidad_errores_titulos, &fecha_proceso) == OK)
                printf("No se encontro %s. Se cargo desde %s\n", RUTA_TITULOS_CSV, RUTA_TITULOS_PRUEBAS);
            else
                printf("No se pudo leer titulos. Se continuara sin ellos.\n");
        }
        
        if (cantidad_titulos > 0 && titulos_construir_indice(vec_titulos, cantidad_titulos, &indice_titulos) == ERROR)
            printf("No se pudo construir indice de titulos.\n");
    }

    menu(&vec_miembros, &cantidad_miembros, &capacidad_miembros, &indice_miembros,
         &vec_titulos, &cantidad_titulos, &capacidad_titulos, &indice_titulos,
         &vec_alquileres, &cantidad_alquileres, &capacidad_alquileres,
         &errores_miembros, &cantidad_errores_miembros, &capacidad_errores_miembros,
         &errores_titulos, &cantidad_errores_titulos, &capacidad_errores_titulos,
         &fecha_proceso);

    if (miembros_guardar_en_binario(RUTA_BINARIO, vec_miembros, cantidad_miembros) == ERROR)
        printf("No se pudo guardar el binario de miembros.\n");
    if (titulos_guardar_en_binario(RUTA_TITULOS_BIN, vec_titulos, cantidad_titulos) == ERROR)
        printf("No se pudo guardar el binario de titulos.\n");

    free(vec_miembros);
    free(vec_titulos);
    free(vec_alquileres);
    free(errores_miembros);
    free(errores_titulos);
    indice_vaciar(&indice_miembros);
    indice_vaciar(&indice_titulos);

    return EXIT_SUCCESS;
}
