#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

void menu(const char *nombreArch, t_indice *ind, const t_fecha *fecha);
char mostrarMenu(const char *msj, const char *opc);
void preguntarCambio(const char * cad, char* aux);
void eliminarFinDeLinea(char *cad);
int AltaMiembro(const char *nombreArch, t_indice *ind, const t_fecha *fecha);
int BajaMiembro(const char *nombreArch, t_indice *ind);
int ModificacionMiembro(const char *nombreArch, t_indice *ind, const t_fecha *fecha);
int MostrarInfoMiembro(const char *nombreArch, t_indice *ind);
int ListadoXDNI(const char *nombreArch, t_indice *ind);
int ListadoXPlan(const char *nombreArch, t_indice *ind);

#endif // MENU_H_INCLUDED
