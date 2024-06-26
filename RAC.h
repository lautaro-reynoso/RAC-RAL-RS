#ifndef RAC_H_INCLUDED
#define RAC_H_INCLUDED

#include "Envios.h"

#define MAXRAC 59//p=0.90

typedef struct {
    Envio envios[MAXRAC];
    int cant;

    float eExMax, eExMed, eFrMax, eFrMed, eExCant, eFrCant, costoEvoE, costoEvoF, tempe, tempef;

} RAC;


void initializeRAC(RAC *rac) {

    rac->eExCant = 0.0;
    rac->eExMax = 0.0;
    rac->eExMed = 0.0;
    rac->eFrMax = 0.0;
    rac->eFrMed = 0.0;
    rac->eFrCant = 0.0;
    rac->costoEvoE = 0.0;
    rac->costoEvoF = 0.0;
    rac->tempe = 0.0;
    rac->tempef = 0.0;
    int i;
    for (i = 0; i < MAXRAC; i++) {
        strcpy(rac->envios[i].codigo, VIRGEN); //Ninguna celda ha sido usada
    }
    rac->cant = 0;

}


int localizarRAC(RAC *rac, char codigo[], int *pos, int k) {
    *pos = 0;
    int i = hashing(codigo, MAXRAC),temp = 0;
    if (rac->cant == 0) {

        *pos = i;
        if (k == 0) {
            if (rac->eFrMax < temp) {
                rac->eFrMax = temp;
            }
            rac->eFrCant++;
            rac->costoEvoF += temp;
            rac->eFrMed = rac->costoEvoF / (rac->eFrCant);
        }
        return 0;
    }


    int libre = -1, cont = 0, p = 1;

    while ((cont < MAXRAC) && (strcmp(rac->envios[i].codigo, VIRGEN) != 0) &&
           (strcmp(rac->envios[i].codigo, codigo) != 0)) {

        if ((libre == -1) && (strcmp(rac->envios[i].codigo, LIBRE) == 0)) {
            libre = i;
        }

        i = (i + p) % MAXRAC;
        cont++;
        p += 1;
        temp++;
    }


    if (cont < MAXRAC) {
        temp++;
    }

    if ((cont < MAXRAC) && (strcmp(rac->envios[i].codigo, codigo) == 0)) { //EXITO
        (*pos) = i;

        if (k == 0) {


            if (rac->eExMax < temp) {
                rac->eExMax = temp;
            }

            rac->eExCant++;
            rac->costoEvoE += temp;
            rac->eExMed = rac->costoEvoE / (rac->eExCant);
        }

        return 1;
    } else if (cont < MAXRAC) {

        if (k == 0) {
            if (rac->eFrMax < temp) {
                rac->eFrMax = temp;
            }
            rac->eFrCant++;
            rac->costoEvoF += temp;
            rac->eFrMed = rac->costoEvoF / (rac->eFrCant);
        }



        (*pos) = i;
        return 0;

    } else {
        if (libre != -1) {
            (*pos) = libre;
            return 0;
        }

        if (k == 0) {
            if (rac->eFrMax < temp) {
                rac->eFrMax = temp;
            }
            rac->eFrCant++;
            rac->costoEvoF += temp;
            rac->eFrMed = rac->costoEvoF / (rac->eFrCant);
        }

        return 2;
    }
}


int altaRAC(RAC *rac, Envio envios) {

    if (rac->cant == MAXRAC) {
        return 2;
    }
    int pos;
    int loc = localizarRAC(rac, envios.codigo, &pos, 1);
    if (loc == 0) {
        rac->envios[pos] = envios;
        rac->cant++;
        return 1;
    } else if (loc == 1) {
        return 0;
    } else {
        return 2;
    }
}


int bajaRAC(RAC *rac, Envio envio) {
    int pos;

    if (localizarRAC(rac, envio.codigo, &pos, 1) != 1) {
        return 0;
    } else {
        if ((strcmp(rac->envios[pos].direccion, envio.direccion) == 0) &&
            (rac->envios[pos].dni_receptor == envio.dni_receptor)
            && (rac->envios[pos].dni_remitente == envio.dni_remitente) &&
            (strcmp(rac->envios[pos].fecha_envio, envio.fecha_envio) == 0)
            && (strcmp(rac->envios[pos].fecha_recepcion, envio.fecha_recepcion) == 0) &&
            (strcmp(rac->envios[pos].nombre, envio.nombre) == 0)
            && (strcmp(rac->envios[pos].nombre_r, envio.nombre_r) == 0)) {

            strcpy(rac->envios[pos].codigo, LIBRE);
            rac->cant--;
            return 1;
        } else {
            return 2;


        }
    }
}

int evocarRAC(RAC *rac, char code[], Envio *envios) {
    int pos;
    if (localizarRAC(rac, code, &pos, 0) == 0) {

        return 0;
    } else {

        strcpy(envios->codigo, rac->envios[pos].codigo);
        envios->dni_remitente = rac->envios[pos].dni_receptor;
        strcpy(envios->nombre_r, rac->envios[pos].nombre_r);
        strcpy(envios->direccion, rac->envios[pos].direccion);
        envios->dni_remitente = rac->envios[pos].dni_remitente;
        strcpy(envios->nombre, rac->envios[pos].nombre);
        envios->dni_remitente = rac->envios[pos].dni_remitente;
        strcpy(envios->fecha_envio, rac->envios[pos].fecha_envio);
        strcpy(envios->fecha_recepcion, rac->envios[pos].fecha_recepcion);

        return 1;
    }
}


void printRAC(RAC rac) {


    int i = 0;
    for (i = 0; i < MAXRAC; i++) {
        if (strcmp(rac.envios[i].codigo, LIBRE) == 0) {
            printf("\t***************************\n\n");
            printf("\n\tElemento N #%d de %d \n", i + 1, MAXRAC);
            printf("\tBALDE LIBRE +\n");
            printf("\t***************************\n\n");
        } else if (strcmp(rac.envios[i].codigo, VIRGEN) == 0) {
            printf("\n\t--------------------------------------");
            printf("\n\tElemento N #%d de %d \n", i + 1, MAXRAC);
            printf("\tBALDE VIRGEN *\n");
            printf("\t***************************\n\n");
        } else {
            printf("\n\t--------------------------------------");
            printf("\n\tElemento N #%d de %d\n", i + 1, MAXRAC);
            mostrarenvio(rac.envios[i]);
            printf("\t***************************\n\n");
        }
        if ((i + 1) % 10 == 0) system("pause");
    }

    printf("ENVIOS:%d\n", rac.cant);

}


#endif // RAC_H_INCLUDED
