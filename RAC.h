#ifndef RAC_H_INCLUDED
#define RAC_H_INCLUDED

#include "Envios.h"

#define MAXRAC 59 ///p=0.90


//ESTRUCTURA
typedef struct {
    Envio envios[MAXRAC];
    int cant;

      float eExMax, eExMed, eFrMax, eFrMed, eExCant, eFrCant,costoEvoE,costoEvoF,tempe,tempef;

} RAC;


void initializeRAC(RAC *rac) {

       rac->eExCant = 0.0;
    rac->eExMax =0.0;
    rac->eExMed =0.0;
     rac->eFrMax  =0.0;
     rac->eFrMed  =0.0;
     rac->eFrCant =0.0;
       rac->costoEvoE =0.0;
       rac->costoEvoF=0.0;
         rac->tempe =0.0;
         rac->tempef=0.0;
    int i;
    for (i = 0; i < MAXRAC; i++) {
        strcpy(rac->envios[i].codigo, VIRGEN); //Ninguna celda ha sido usada
    }
    rac->cant = 0;

}


int localizarRAC(RAC *rac, char code[], int *pos, int k) {
    int i = hashing(code, MAXRAC), cont = 0, celdaLibre = -1, p = 1, temp = 0;

    while ((cont < MAXRAC) && (strcmp(rac->envios[i].codigo, VIRGEN) != 0) &&
           (strcmp(rac->envios[i].codigo, code) != 0)) {

        if ((celdaLibre == -1) && (strcmp(rac->envios[i].codigo, LIBRE) == 0)) {
            celdaLibre = i; //CELDA LIBRE EN I
        }

        i = (i + p) % MAXRAC;
        cont++;
        p += 1;
        temp++;
    }

/*
    if (cont < MAXRAC) {
      temp++;
    }
*/
    if ((cont < MAXRAC) && (stricmp(rac->envios[i].codigo, code) == 0)) { //EXITO
        (*pos) = i;

  if(k==0){


          if (rac->eExMax < temp) {
                rac->eExMax = temp;
            }

            rac->eExCant++;
            rac->costoEvoE += temp;
            rac->tempe += rac->costoEvoE;
            rac->eExMed = rac->tempe / (rac->eExCant);
  }

        return 1;
    } else if (cont < MAXRAC) { //FRACASO

         if (k == 0) {
            if (rac->eFrMax < temp) {
                rac->eFrMax = temp;
            }
            rac->eFrCant++;
            rac->costoEvoF += temp;
            rac->tempef += rac->costoEvoF;
            rac->eFrMed = rac->tempef / (rac->eExCant);
        }



        if (celdaLibre != -1) { //CELDA LIBRE
            (*pos) = celdaLibre;
            return 0;
        } else { //CELDA VIRGEN
            (*pos) = i;
            return 0;
        }
    } else { //FRACASO, RECORRIÓ TODA LA ESTRUCTURA
        return 2;
    }
}


int altaRAC(RAC *rac, Envio envios) {
    int pos;
    float celdasConsult = 0.0;
    int loc = localizarRAC(rac, envios.codigo, &pos, 1);
    if (loc == 0) { //EXITO, NO ESTA EN LA ESTRUCTURA

        rac->envios[pos] = envios;
        rac->cant++;
        return 1;
    } else if (loc == 1) { //FRACASO, ESTA EN LA ESTRUCURA
        return 0;
    } else {
        return 2; //FRACASO, ESTRUCTURA LLENA
    }
}


int bajaRAC(RAC *rac, Envio envio) {
    int pos, confirm;
    float celdas_consultadas = 0.0;
    if (localizarRAC(rac, envio.codigo, &pos, 1) != 1) { //FRACASO, NO SE ENCUENTRA EN LA ESTRUCTURA
        return 0;
    } else {
        if ((strcmp(rac->envios[pos].direccion, envio.direccion) == 0) &&
            (rac->envios[pos].dni_receptor == envio.dni_receptor)
            && (rac->envios[pos].dni_remitente == envio.dni_remitente) &&
            (strcmp(rac->envios[pos].fecha_envio, envio.fecha_envio) == 0)
            && (strcmp(rac->envios[pos].fecha_recepcion, envio.fecha_recepcion) == 0) &&
            (strcmp(rac->envios[pos].nombre, envio.nombre) == 0)
            && (strcmp(rac->envios[pos].nombre_r, envio.nombre_r) == 0)) {
            //EXITO, CELDA LIBRE
            strcpy(rac->envios[pos].codigo, LIBRE);
            rac->cant--;
            return 1;
        } else {
            return 2; //FRACASO, NO SON IGUALES

        }
    }
}

    int evocarRAC(RAC *rac, char code[], Envio *envios) {
        int pos;
        float costosAux = 0.0;
        if (localizarRAC(rac, code, &pos, 0) == 0) { //FRACASO, NO SE ENCONTRO EN LA ESTRUCTURA

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

//MOSTRAR
    void printRAC(RAC rac) {
        int i = 0;
        for (i = 0; i < MAXRAC; i++) {
            if (strcmp(rac.envios[i].codigo, LIBRE) == 0) {
                printf("\t--------------------------------------");
                printf("\n\tElemento N #%d de %d \n", i+1, MAXRAC);
                printf("\tCELDA LIBRE +\n");
                printf("\t--------------------------------------\n\n");
            } else if (strcmp(rac.envios[i].codigo, VIRGEN) == 0) {
                printf("\n\t--------------------------------------");
                printf("\n\tElemento N #%d de %d \n", i+1, MAXRAC);
                printf("\tCELDA VIRGEN *\n");
                printf("\t--------------------------------------\n\n");
            } else {
                printf("\n\t--------------------------------------");
                printf("\n\tElemento N #%d de %d\n", i+1, MAXRAC);
                mostrarenvio(rac.envios[i]);
                printf("\t--------------------------------------\n");
            }
            if ((i + 1) % 5 == 0) system("pause");
        }
    }


#endif // RAC_H_INCLUDED
