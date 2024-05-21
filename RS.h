#ifndef RS_H_INCLUDED
#define RS_H_INCLUDED

#include "Envios.h"

#define MAXRS 36

struct nodo {
    Envio vipd;
    struct Nodo *siguiente;
};
typedef struct nodo Nodo;

typedef struct {
    Nodo *actual;
    Nodo *inicio;
    Nodo *anterior;
} lista_de_envios;

typedef struct {
    lista_de_envios envios[MAXRS];
    int cant;
    int indicador[MAXRS];
    float eExMax, eExMed, eFrMax, eFrMed, eExCant, eFrCant,costoEvoE,costoEvoF,tempe,tempef;
} RS;

void initializeRS(RS *rs) {
    int i;
    for (i = 0; i < MAXRS; i++) {

        Nodo *actual = rs->envios[i].inicio;
        Nodo *siguiente;

        while (actual != NULL) {
            siguiente = actual->siguiente;
            free(actual);
            actual = siguiente;
        }
        rs->envios[i].inicio = NULL;
        rs->envios[i].actual = NULL;
        rs->envios[i].anterior = NULL;
        rs->indicador[i] = 0;
    }
    rs->cant = 0;
    rs->eExCant = 0.0;
    rs->eExMax = 0.0;
    rs->eExMed = 0.0;
    rs->eFrMax = 0.0;
    rs->eFrMed = 0.0;
    rs->eFrCant = 0.0;
    rs->costoEvoE = 0.0;
    rs->costoEvoF = 0.0;
    rs->tempe = 0.0;
    rs->tempef = 0.0;
}

int LocalizarRS(RS *rs, int *j, char codigo[],int k) {

    int temp = 0.0;

    int i = hashing(codigo, MAXRS);
    (*j) = i;
    if (rs->envios[i].inicio == NULL) {

        return 0;
    }


    rs->envios[i].actual = rs->envios[i].inicio;
    rs->envios[i].anterior = rs->envios[i].inicio;
    while (rs->envios[i].actual != NULL && strcmp(rs->envios[i].actual->vipd.codigo, codigo) != 0) {

        rs->envios[i].anterior = rs->envios[i].actual;
        rs->envios[i].actual = rs->envios[i].actual->siguiente;
        temp++;
    }
    if (rs->envios[i].actual != NULL){
        temp++;
        if (k == 0) {


            if (rs->eExMax < temp) {
                rs->eExMax = temp;
            }

            rs->eExCant++;
            rs->costoEvoE += temp;
            rs->eExMed = rs->costoEvoE / (rs->eExCant);
        }
        return 1;
    }

    else{

        if (k == 0) {
            if (rs->eFrMax < temp) {
                rs->eFrMax = temp;
            }
            rs->eFrCant++;
            rs->costoEvoF += temp;
            rs->eFrMed = rs->costoEvoF / (rs->eFrCant);
        }
        return 0;
    }

}

int altaRS(RS *rs, Envio a) {
    if (rs->cant == MAXRS)
        return 0;

    int i;

    if (LocalizarRS(rs, &i, a.codigo,1) == 1)
        return 0;


    Nodo *p;
    p = (Nodo *) malloc(sizeof(Nodo));
    if (rs->envios[i].actual == rs->envios[i].inicio) {
        (*p).vipd = a;
        (*p).siguiente = rs->envios[i].actual;
        rs->envios[i].inicio = p;
        rs->envios[i].actual = p;
    } else {
        (*p).vipd = a;
        (*p).siguiente = rs->envios[i].actual;
        rs->envios[i].anterior->siguiente = p;
        rs->envios[i].actual = p;
    }
    rs->cant++;
    rs->indicador[i] = 1;
    return 1;
}


void limpiars(RS *rs){
    int i;
    for ( i = 0; i < MAXRS; ++i) {
        if(rs->envios[i].inicio==NULL){

        }
        else{
            rs->envios[i].actual=rs->envios[i].inicio->siguiente;
            rs->envios[i].anterior=rs->envios[i].inicio;
            while (rs->envios[i].actual!=NULL){
                rs->envios[i].anterior->siguiente=rs->envios[i].actual->siguiente;
                free(rs->envios[i].actual);
                rs->envios[i].actual=rs->envios[i].anterior->siguiente;
            }
            free(rs->envios[i].anterior);
            rs->envios[i].inicio=NULL;
        }
    }
}


int bajaRS(RS *rs, Envio a) {
    if (rs->cant == 0)
        return 0;

    int i;

    if (LocalizarRS(rs, &i, a.codigo,1) == 0)
        return 0;

    if ((strcmp(rs->envios[i].actual->vipd.direccion, a.direccion) == 0) &&
        (rs->envios[i].actual->vipd.dni_receptor == a.dni_receptor)
        && (rs->envios[i].actual->vipd.dni_remitente == a.dni_remitente) &&
        (strcmp(rs->envios[i].actual->vipd.fecha_envio, a.fecha_envio) == 0)
        && (strcmp(rs->envios[i].actual->vipd.fecha_recepcion, a.fecha_recepcion) == 0) &&
        (strcmp(rs->envios[i].actual->vipd.nombre, a.nombre) == 0)
        && (strcmp(rs->envios[i].actual->vipd.nombre_r, a.nombre_r) == 0)) {


        if (rs->envios[i].inicio == rs->envios[i].actual) {
            rs->envios[i].inicio = rs->envios[i].actual->siguiente;
            rs->envios[i].anterior = rs->envios[i].inicio;



        } else {
            rs->envios[i].anterior->siguiente = rs->envios[i].actual->siguiente;

        }
        free(rs->envios[i].actual);
        rs->envios[i].actual = rs->envios[i].inicio;
        rs->cant--;
        return 1;
    }else
        return 0;


}

int evocarRS(RS *rs, Envio a, Envio *envios) {
    if (rs->cant == 0)
        return 0;

    int pos;


    if (LocalizarRS(rs, &pos, a.codigo,0) == 0) {

        return 0;
    } else {

        strcpy(envios->codigo, rs->envios[pos].actual->vipd.codigo);
        envios->dni_remitente = rs->envios[pos].actual->vipd.dni_receptor;
        strcpy(envios->nombre_r, rs->envios[pos].actual->vipd.nombre_r);
        strcpy(envios->direccion, rs->envios[pos].actual->vipd.direccion);
        envios->dni_remitente = rs->envios[pos].actual->vipd.dni_remitente;
        strcpy(envios->nombre, rs->envios[pos].actual->vipd.nombre);
        envios->dni_remitente = rs->envios[pos].actual->vipd.dni_remitente;
        strcpy(envios->fecha_envio, rs->envios[pos].actual->vipd.fecha_envio);
        strcpy(envios->fecha_recepcion, rs->envios[pos].actual->vipd.fecha_recepcion);

        return 1;
    }


}

void printRS(RS *rs) {
    for (int i = 0; i < MAXRS; ++i) {

        Nodo *current = rs->envios[i].inicio;

        if( rs->envios[i].inicio == NULL){
            printf("Balde N %d: No contiene envios\n", i + 1);
        }else{
            printf("Balde N %d:\n", i + 1);
            int count = 1;  // Contador para elementos dentro de la lista
            while (current != NULL) {
                printf("\n\t--------------------------------------");
                printf("\n\tElemento N #%d de balde %d\n", count, i + 1);
                mostrarenvio(current->vipd);
                printf("\t--------------------------------------\n");


                if (count % 5 == 0) {
                    getchar();
                }

                current = current->siguiente;
                count++;
            }
        }


    }
}


#endif // RAL_H_INCLUDED
