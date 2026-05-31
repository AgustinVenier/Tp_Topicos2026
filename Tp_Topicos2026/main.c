#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validaciones.h"
#include "estructuras.h"
#include "indice.h"

#define subcarpeta_binario "ArchivoBinario"
#define SUBCARPETA_ERROR "ArchivoTextoError"
#define nombreArchivoTexto "miembros-VC.txt"
#define ARCHIVO_TITULOS_CSV "titulos.csv"

int main()
{
    t_fecha fecha_proceso;
    t_indice indice;
    char nombreArchivoBinario[60];
    int flagProcesar;
    LeeSubCarpeta(subcarpeta_binario,nombreArchivoBinario);
    ingresarFechaProceso(&fecha_proceso);

    flagProcesar=crearNombreArchivo(nombreArchivoBinario,subcarpeta_binario,subcarpeta_error,&fechaProceso);
    if(flagProcesar==ERROR)
    {
        procesamiento=pasajeTextoBinario(nombreArchivoTexto,nombreArchivoBinario,&fechaProceso,&indice,cmp_por_dni);
        if (procesamiento==OK)
            printf("Procesamiento exitoso\n");
        else
        {
            printf("Error en el procesamiento\n");
            indice_vaciar(&indice);
            return 0;
        }

        printf("Nombre archivo binario:%s\n",nombreArchivoBinario);
    }
    else if(indice_cargar(nombreArchivoBinario,&indice,indice.vindice,sizeof(t_reg_indice),cmp_por_dni)==ERROR){
        indice_vaciar(&indice);
        return 0;
    }


    system("pause");

    /// LOGICA PARTE 2 Y MENU

    menu(nombreArchivoBinario,&indice, &fechaProceso);

    indice_vaciar(&indice);

    return 0;
}
