int pasajeTextoBinario(char * nombreArchivoTexto, char * nombreArchivoBin, const t_fecha* f_proceso,t_indice * indice,int (*cmp)(const void *, const void *))
{
    char cad[BUFFER],aux[BUFFER+30],*ptr_fin;
    t_miembro m1;
    t_miembro *miembro = &m1;
    t_reg_indice auxReg;
    int valor,seInserta,contador=0;
    FILE* fbin;
    FILE* ftexto;

    ftexto = fopen(nombreArchivoTexto, "rt");
    if (ftexto == NULL)
    {
        printf("Error al abrir un archivo texto");
        return ERROR;
    }

    fbin = fopen(nombreArchivoBin, "wb");
    if (fbin == NULL)
    {
        printf("Error al abrir un archivo binario");
        fclose(ftexto);
        fclose(ferror);
        return ERROR;
    }

    //CARGAMOS

    while(fgets(cad, sizeof(cad), ftexto))
    {

        ptr_fin= strpbrk(cad, "\r\n");
        if (ptr_fin) {
            *ptr_fin = '\0';
        }
        *(miembro->email) = '\0';
        sscanf(cad,
               "%ld|%60[^|]|%d/%d/%d|%c|%d/%d/%d|%10[^|]|%d/%d/%d|%c|%9[^|]|%29[^\n]",
               &miembro->dni,miembro->nya, &miembro->fecha_nac.dia, &miembro->fecha_nac.mes,
               &miembro->fecha_nac.anio, &miembro->sexo, &miembro->fecha_afi.dia, &miembro->fecha_afi.mes,
               &miembro->fecha_afi.anio, miembro->cat, &miembro->fecha_cuota.dia, &miembro->fecha_cuota.mes,
               &miembro->fecha_cuota.anio, &miembro->estado, miembro->plan, miembro->email
              );
        valor = validaciones(miembro,f_proceso);

        auxReg.dni=miembro->dni;
        auxReg.nro_reg=contador;

        if(valor == OK)
        {
            if(toupper(miembro->estado)=='A')
                seInserta=indice_insertar(indice,&auxReg,sizeof(t_reg_indice),cmp);
            else
                seInserta=-1;

            if(seInserta==OK)
            {
                contador++;
                fwrite(miembro, sizeof(t_miembro), 1, fbin);
            }
            else
            {
                if(seInserta==-1)
                {
                    fwrite(miembro, sizeof(t_miembro), 1, fbin);
                    contador++;
                }
                else
                    valor=10;
            }
        }
    fclose(ftexto);
    fclose(fbin);
    return OK;
}

void LeeSubCarpeta (char* subCarpeta,char* nombreArchivo)
{
    struct dirent *dir;
    DIR *d = opendir(subCarpeta);
    int flag=0;

    if (!d)
    {
        perror("opendir() error");
        *nombreArchivo=' ';
        return ; // La subcarpeta no existe o no se puede leer
    }

    while ((dir = readdir(d)) != NULL && flag==0)
    {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
        {
            strcpy(nombreArchivo, dir->d_name);
            flag=1;
        }
        else
            *nombreArchivo=' ';
    }
    closedir(d); // Cerramos el directorio
    return ;
}

int crearNombreArchivo(char *nombreArchivoBinario, const char *subcarpeta_binario,const t_fecha *pf)
{
    char aux_fecha[10], aux_nombre[61];

    if (*nombreArchivoBinario == ' ')
    {
        printf("No hay archivos para recuperar, se generaran nuevos.\n");

        strcpy(nombreArchivoBinario,subcarpeta_binario);
        strcat(nombreArchivoBinario, "/miembros-VC-");
        sprintf(aux_fecha,"%04d%02d%02d",pf->anio,pf->mes,pf->dia);
        strcat(nombreArchivoBinario,aux_fecha);
        strcat(nombreArchivoBinario,".dat");

        return ERROR; // Procesar
    }
    else
    {
        strcpy(aux_nombre,subcarpeta_binario);
        strcat(aux_nombre,"/");
        strcat(aux_nombre,nombreArchivoBinario);
        strcpy(nombreArchivoBinario,subcarpeta_binario);
        strcat(nombreArchivoBinario, "/miembros-VC-");
        sprintf(aux_fecha,"%04d%02d%02d",pf->anio,pf->mes,pf->dia);
        strcat(nombreArchivoBinario,aux_fecha);
        strcat(nombreArchivoBinario,".dat");

        printf("Recuperando desde: %s\nLuego de finalizar se guardara con el nombre: %s\n",aux_nombre,nombreArchivoBinario);
        if ((rename(aux_nombre,nombreArchivoBinario)) == 0)
        {
            printf("Archivo renombrado exitosamente!\n\n");
        }
        else    // Si falla, imprime el error del sistema
        {
            perror("Error al renombrar el archivo");
        }
        return OK; // Recuperar
    }
}
