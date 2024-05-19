#ifndef RAL_H_INCLUDED
#define RAL_H_INCLUDED
#include "Envios.h"
#define MAXRAL 68 ///p=0.74  50/0.74 = 67.56 , techo(67.56) = 68


//ESTRUCTURA
typedef struct{
    Envio envios[MAXRAL];
    int cant;
}RAL;


void initializeRAL (RAL *ral){
    int i;
    for ( i = 0; i < MAXRAL; i++)
    {
        strcpy(ral->envios[i].codigo, VIRGEN); //Ninguna celda ha sido usada
    }
    ral->cant = 0;

}



int localizarRAL(RAL *ral, char code[], int *pos, float *celdas_consultadas){
    int i, celdaLibre=-1, cont=0, costosAux=0;
    i=hashing(code, MAXRAL);

    while( (cont < MAXRAL) && (strcmp(ral->envios[i].codigo,VIRGEN)!=0) && (strcmp(ral->envios[i].codigo,code)!=0) ){

        if((celdaLibre==-1)&&(strcmp(ral->envios[i].codigo,LIBRE)==0)){
            celdaLibre=i;
        }
        i=(i+1)%MAXRAL;
        cont++;
        costosAux+=1;
    }

    if(cont<MAXRAL){
        costosAux+=1;
    }
    (*celdas_consultadas)=costosAux;

    if((cont<MAXRAL)&&(stricmp(ral->envios[i].codigo, code)==0)){ //EXITO
        (*pos)=i;
        return 1;
    } else if(cont<MAXRAL){ //FRACASO
        if(celdaLibre!=-1){ //PASO POR CELDA LIBRE
            (*pos)=celdaLibre;
            return 0;
        } else { //CELDA VIRGEN
            (*pos)=i;
            return 0;
        }
    } else { //FRACASO, RECORRIO TODA LA ESTRUCTURA
        return 2;
    }
}


//ALTA
int altaRAL(RAL *ral, Envio envio)
{
    int pos;
    float celdasConsult = 0.0;
    int loc = localizarRAL(ral, envio.codigo, &pos, &celdasConsult);
    if(loc == 0 ){
        ral->envios[pos] = envio;
        ral->cant++;
        return 0;
    } else if(loc == 1){
        return 1;
    } else { //esta lleno
        return 2;
    }
}



//BAJA

int bajaRAL (RAL *lista, Envio envio) {
    printf("baja RAL");
    int pos, confirm;
    float celdas_consultadas = 0.0;
    if (localizarRAL(lista, envio.codigo, &pos, &celdas_consultadas) == 0) { //FRACASO, NO SE ENCUENTRA EN LA ESTRUCTURA
        return 0;
    } else {


        if ((strcmp(lista->envios[pos].direccion, envio.direccion) == 0) &&
            (lista->envios[pos].dni_receptor == envio.dni_receptor)
            && (lista->envios[pos].dni_remitente == envio.dni_remitente) &&
            (strcmp(lista->envios[pos].fecha_envio, envio.fecha_envio) == 0)
            && (strcmp(lista->envios[pos].fecha_recepcion, envio.fecha_recepcion) == 0) &&
            (strcmp(lista->envios[pos].nombre, envio.nombre) == 0)
            && (strcmp(lista->envios[pos].nombre_r, envio.nombre_r) == 0)) {
            //EXITO, CELDA LIBRE
            strcpy(lista->envios[pos].codigo, LIBRE);
            lista->cant--;
            return 1;

        } else {
            return 2;

        }



    }

}

int evocarRAL(RAL *ral, char code[] ,Envio *envios, float* costo){
    int pos;
    float costosAux=0.0;
    if ( localizarRAL(ral,code,&pos,&costosAux) == 0){ //FRACASO, NO SE ENCONTRO EN LA ESTRUCTURA
        (*costo) = costosAux;
        return 1;
    }  else{
strcpy(envios->codigo, ral->envios[pos].codigo);
envios->dni_remitente = ral->envios[pos].dni_receptor;
strcpy(envios->nombre_r, ral->envios[pos].nombre_r);
strcpy(envios->direccion, ral->envios[pos].direccion);
envios->dni_remitente = ral->envios[pos].dni_remitente;
strcpy(envios->nombre, ral->envios[pos].nombre);
envios->dni_remitente = ral->envios[pos].dni_remitente;
strcpy(envios->fecha_envio, ral->envios[pos].fecha_envio);
strcpy(envios->fecha_recepcion, ral->envios[pos].fecha_recepcion);
(*costo) = costosAux;
return 1;
}
}

//MOSTRAR
void printRAL (RAL ral){
    int i = 0;
    for (i=0; i < MAXRAL ; i++) {
        if(strcmp(ral.envios[i].codigo,LIBRE)==0){
            printf("\t--------------------------------------");
            printf("\n\tElemento N #%d de %d \n", i, MAXRAL);
            printf("\tCELDA LIBRE +\n");
            printf("\t--------------------------------------\n\n");
        }
        else if(strcmp(ral.envios[i].codigo, VIRGEN)==0){
            printf("\n\t--------------------------------------");
            printf("\n\tElemento N #%d de %d \n", i, MAXRAL);
            printf("\tCELDA VIRGEN *\n");
            printf("\t--------------------------------------\n\n");
        }
        else {
            printf("\n\t--------------------------------------");
            printf("\n\tElemento N #%d de %d\n", i, MAXRAL);
            mostrarenvio(ral.envios[i]);
            printf("\t--------------------------------------\n");
        }
        if ((i+1) % 5 == 0) system("pause");
    }
}



#endif // RAL_H_INCLUDED
