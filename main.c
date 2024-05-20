#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "RAC.h"
#include "RAL.h"
#include "RS.h"
#include "Envios.h"


int LecturaOperaciones();

char *Mayusculas();

void mostrarestructura();

void mostrarSubmenu() {

    printf("--Submenu--\n");
    printf("1. REBALSE ABIERTO CUADRATICO.\n");
    printf("2. REBALSE ABIERTO LINEAL\n");
    printf("3. REBALSE SEPARADO\n");
    printf("4. Volver al Menu Principal\n");
}

char *Mayusculas(char string[]) {
    int i;
    for (i = 0; string[i] != '\0'; i++) {
        string[i] = toupper(string[i]);
    }
    return string;
}






int main() {


    int opcion, submenu_opcion;
    RAC rac;
    RAL ral;
 RS rs;

    initializeRAL(&ral);
    initializeRAC(&rac);
    initializeRS(&rs);
    LecturaOperaciones(&rac,&ral,&rs);

    do {
        // Men� principal

        printf("1. Comparacion de estructuras\n");
        printf("2. Mostrar Estructura\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                system("cls");




                printf("\t Acum Ev.Ex | Max.Ev.Ex | Med.Ev.Ex |Acum Ev.Fr | Max.Ev.Fr | Med.Ev.Fr|\n");
                printf("-----------------------------------------------------------------------------------------------\n");
                printf("RAL ::   |     %.2f  |     %.2f    |   %.2f    |   %.2f     |   %.2f      |   %.2f    | \n",ral.costoEvoE, ral.eExMax, ral.eExMed ,ral.costoEvoF ,ral.eFrMax , ral.eFrMed);
                printf("-----------------------------------------------------------------------------------------------\n");
                printf("RAC ::   |     %.2f  |     %.2f    |   %.2f    |   %.2f     |   %.2f      |   %.2f    | \n",rac.costoEvoE, rac.eExMax, rac.eExMed ,rac.costoEvoF ,rac.eFrMax , rac.eFrMed);
                printf("-----------------------------------------------------------------------------------------------\n");
           //     printf("RS   ::  |   %.2f   |   %.2f    |  %.2f    |   %.2f   | \n");
                printf("-----------------------------------------------------------------------------------------------\n");


                break;
            case 2:


                system("cls");
                do {
                    mostrarSubmenu();
                    printf("Seleccione una opcion: ");
                    scanf("%d", &submenu_opcion);
                    switch (submenu_opcion) {
                        case 1:
                            system("cls");

                            printf("REBALSE ABIERTO CUADRATICO:\n");
                            printRAC(rac);
                            break;
                        case 2:
                            system("cls");
                            printf(" REBALSE ABIERTO LINEAL:\n");
                            printRAL(ral);

                            break;
                        case 3:
                            system("cls");
                            printf("REBALSE SEPARADO:\n");

                            printRS(&rs);

                            break;
                        case 4:
                            system("cls");
                            // Volver
                            break;
                        default:
                            system("cls");
                            printf("Opcion no valida. Intente de nuevo.\n");
                    }
                } while (submenu_opcion != 4);

                break;
            case 3:
                system("cls");
                printf("Saliendo de la aplicacion.\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 3);


    return 0;
}

int LecturaOperaciones(RAC *rac, RAL *ral, RS *rs) {
int baja=0,alta=0;
    // Declaraciones e inicializaciones
    int evocar = 0, contador = 0;
    Envio aux, aux2;
    FILE *fp;
    char cwd[1024];



    // Obtener el directorio actual
    if (_getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error al obtener el directorio actual");
        return 1;
    }




    // Concatenar la ruta del archivo al directorio actual
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s\\%s", cwd, "../Operaciones-Envios.txt");

    // Intentar abrir el archivo en modo lectura
    if ((fp = fopen(filepath, "r")) == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    } else {
        printf("ARCHIVO ABIERTO CON EXITO\n");
        int codigoOperador = 0, i;


        while (!(feof(fp))) {


            fscanf(fp, "%d", &codigoOperador);


            fscanf(fp, " %[^\n]", aux.codigo);
            for (i = 0; i <= 8; i++) {
                aux.codigo[i] = toupper(aux.codigo[i]);
            }
            if (codigoOperador == 1 || codigoOperador == 2) {

                // Leer y procesar los datos para Alta o Baja
                fscanf(fp, "%d", &aux.dni_receptor);

                fscanf(fp, " %[^\n]", aux.nombre);
                for (i = 0; i <= strlen(aux.nombre); i++) {
                    aux.nombre[i] = toupper(aux.nombre[i]);
                }
                fscanf(fp, " %[^\n]", aux.direccion);
                for (i = 0; i <= strlen(aux.direccion); i++) {
                    aux.direccion[i] = toupper(aux.direccion[i]);
                }
                fscanf(fp, "%d", &aux.dni_remitente);
                fscanf(fp, " %[^\n]", aux.nombre_r);
                for (i = 0; i <= strlen(aux.nombre_r); i++) {
                    aux.nombre_r[i] = toupper(aux.nombre_r[i]);
                }
                fscanf(fp, " %[^\n]", aux.fecha_envio);
                fscanf(fp, " %[^\n]", aux.fecha_recepcion);

                // Llama a la funci�n correspondiente para alta o baja en las estructuras
                if (codigoOperador == 1) {

                    alta++;

                   altaRAC(rac, aux);
                   altaRAL(ral,aux);

                //    altaRS(rs,aux);




                }
                if (codigoOperador == 2) {

                    baja+=1;
                    bajaRAC(rac,aux);
                 bajaRAL(ral,aux);
                 //   bajaRS(rs,aux);


                }

            } else if (codigoOperador == 3) {

                evocar++;

           evocarRAL(ral,aux.codigo, &aux2);
              evocarRAC(rac,aux.codigo, &aux2);
                int exito = 0;


            } else {
                // Maneja el caso de c�digo de operaci�n no reconocido
                printf("Error: Codigo de operaci�n no reconocido.\n");
                //break;
            }

        }

        printf("ALTAS: %d, BAJAS: %d \n", alta,baja);

        fclose(fp);


        return 1;
    }

}
