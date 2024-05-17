#ifndef RAC_H_INCLUDED
#define RAC_H_INCLUDED
#include "Envios.h"
#define MAXRAC 401 ///p=0.75


//ESTRUCTURA
typedef struct{
    Envio envios[MAXRAC];
    int cant;
}RAC;


void initializeRAC (RAC *rac){
    int i;
    for ( i = 0; i < MAXRAC; i++)
    {
        strcpy(rac->envios[i].codigo, VIRGEN); //Ninguna celda ha sido usada
    }
    rac->cant = 0;

}


int localizarRAC(RAC *rac, char code[], int *pos, float *celdasConsult)
{
    int i = hashing(code, MAXRAC), cont=0, celdaLibre = -1, p = 1, costosAux = 0;

    while ((cont < MAXRAC) && (strcmp(rac->envios[i].codigo, VIRGEN)!=0) && (strcmp(rac->envios[i].codigo, code)!=0)){

        if((celdaLibre==-1)&&(strcmp(rac->envios[i].codigo,LIBRE)==0)){
                celdaLibre=i; //CELDA LIBRE EN I
        }

        i = (i + p) % MAXRAC;
        cont++;
        p+=1;
        costosAux++;
    }

    if(cont<MAXRAC){
        costosAux+=1;
    }
    (*celdasConsult)=costosAux;
    if ((cont<MAXRAC) && (stricmp(rac->envios[i].codigo, code)==0)){ //EXITO
            (*pos)=i;
        return 1;
    } else if(cont<MAXRAC){ //FRACASO
                    if(celdaLibre!=-1){ //CELDA LIBRE
                        (*pos)=celdaLibre;
                        return 0;
                    } else { //CELDA VIRGEN
                        (*pos)=i;
                        return 0;
                    }
        } else { //FRACASO, RECORRIÓ TODA LA ESTRUCTURA
                return 0;
        }
}


int altaRAC (RAC *rac, Envio envios)
{
    int pos;
    float celdasConsult = 0.0;
    int loc = localizarRAC (rac,envios.codigo,&pos,&celdasConsult);
    if(loc == 0 ){ //EXITO, NO ESTA EN LA ESTRUCTURA
            rac->envios[pos]=envios;
            rac->cant++;
            return 1;
    } else if(loc == 1){ //FRACASO, ESTA EN LA ESTRUCURA
            return 0;
        } else {
                return 0; //FRACASO, ESTRUCTURA LLENA
            }
}


int bajaRAC (RAC *rac, Envio envios){
    int pos, confirm;
    float celdas_consultadas=0.0;
   if(localizarRAC(rac,envios.codigo,&pos,&celdas_consultadas)==0){ //FRACASO, NO SE ENCUENTRA EN LA ESTRUCTURA
        return 0;
    }else{
        //confirm=compareShipments(envios,rac->envios[pos]);
        if(confirm==1){ //EXITO, CELDA LIBRE
           strcpy(rac->envios[pos].codigo, LIBRE);
            rac->cant--;
            return 1;
        }else{
            return 0; //FRACASO, NO SON IGUALES
        }
    }
}

int evocarRAC(RAC *rac, char code[] ,Envio *envios, float* costo){
    int pos;
    float costosAux=0.0;
    if ( localizarRAC(rac,code,&pos,&costosAux) == 0){ //FRACASO, NO SE ENCONTRO EN LA ESTRUCTURA
        (*costo) = costosAux;
        return 0;
    }else{
        strcpy(envios->codigo, rac->envios[pos].codigo);
        envios->dni_remitente = rac->envios[pos].dni_receptor;
        strcpy(envios->nombre_r, rac->envios[pos].nombre_r);
        strcpy(envios->direccion, rac->envios[pos].direccion);
        envios->dni_remitente = rac->envios[pos].dni_remitente;
        strcpy(envios->nombre, rac->envios[pos].nombre);
        envios->dni_remitente = rac->envios[pos].dni_remitente;
        strcpy(envios->fecha_envio, rac->envios[pos].fecha_envio);
        strcpy(envios->fecha_recepcion, rac->envios[pos].fecha_recepcion);
        (*costo) = costosAux;
        return 1;
    }
}

//MOSTRAR
void printRAC (RAC rac){
    int i = 0;
        for (i=0; i < MAXRAC ; i++) {
             if(strcmp(rac.envios[i].codigo,LIBRE)==0){
            printf("\t--------------------------------------");
            printf("\n\tElemento N #%d de %d \n", i, MAXRAC);
            printf("\tCELDA LIBRE +\n");
            printf("\t--------------------------------------\n\n");
        }
        else if(strcmp(rac.envios[i].codigo, VIRGEN)==0){
            printf("\n\t--------------------------------------");
            printf("\n\tElemento N #%d de %d \n", i, MAXRAC);
            printf("\tCELDA VIRGEN *\n");
           printf("\t--------------------------------------\n\n");
        }
         else {
                 printf("\n\t--------------------------------------");
                    printf("\n\tElemento N #%d de %d\n", i, MAXRAC);
                    mostrarenvio(rac.envios[i]);
                    printf("\t--------------------------------------\n");
            }
            if ((i+1) % 5 == 0) system("pause");
        }
}



#endif // RAC_H_INCLUDED
