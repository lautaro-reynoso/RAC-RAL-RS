#ifndef RS_H_INCLUDED
#define RS_H_INCLUDED

#include "Envios.h"

#define MAXRS 7

struct nodo {
    Envio vipd;
    struct Nodo *siguiente;
};
typedef struct nodo Nodo;

typedef struct {
   Nodo *inicio;
} lista_de_envios;

typedef struct {
    lista_de_envios envios[MAXRS];
    int cant;
    int indicador[MAXRS];
    Nodo *actual;
    Nodo *anterior;
    float eExMax, eExMed, eFrMax, eFrMed, eExCant, eFrCant, costoEvoE, costoEvoF, tempe, tempef;
} RS;

void initializeRS(RS *rs) {
    int i;
    for (i = 0; i < MAXRS; i++) {

        rs->actual = rs->envios[i].inicio;
        Nodo *siguiente;

        while (rs->actual != NULL) {
            siguiente = rs->actual->siguiente;
            free(rs->actual);
            rs->actual = siguiente;
        }
        rs->envios[i].inicio = NULL;

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

int LocalizarRS(RS *rs, int *j, char codigo[], int k) {

    int temp = 0.0;

    int i = hashing(codigo, MAXRS);
    (*j) = i;
    if (rs->envios[i].inicio == NULL) {

        return 0;
    }


    rs->actual = rs->envios[i].inicio;
    rs->anterior = rs->envios[i].inicio;
    while (rs->actual != NULL && strcmp((rs->actual)->vipd.codigo, codigo) != 0) {

        rs->anterior = rs->actual;
        rs->actual = (rs->actual)->siguiente;
        temp++;
    }
    if (rs->actual != NULL) {
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
    } else {

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

  


    int i;

    if (LocalizarRS(rs, &i, a.codigo, 1) == 1)
        return 0;


    Nodo *p;
    p = (Nodo *) malloc(sizeof(Nodo));

    (*p).vipd = a;
    (*p).siguiente = rs->envios[i].inicio;
    rs->envios[i].inicio = p;

    rs->cant++;

    return 1;
}

/*
void limpiars(RS *rs) {
    int i;
    for (i = 0; i < MAXRS; ++i) {
        if (rs->envios[i].inicio == NULL) {

        } else {
            rs->actual = rs->envios[i].inicio->siguiente;
            rs->envios[i].anterior = rs->envios[i].inicio;
            while (rs->actual != NULL) {
                rs->envios[i].anterior->siguiente = rs->actual->siguiente;
                free(rs->actual);
                rs->actual = rs->envios[i].anterior->siguiente;
            }
            free(rs->envios[i].anterior);
            rs->envios[i].inicio = NULL;
        }
    }
}

*/
int bajaRS(RS *rs, Envio a) {


    int i;

    if (LocalizarRS(rs, &i, a.codigo, 1) == 0)
        return 0;

    if ((strcmp(rs->actual->vipd.direccion, a.direccion) == 0) &&
        (rs->actual->vipd.dni_receptor == a.dni_receptor)
        && (rs->actual->vipd.dni_remitente == a.dni_remitente) &&
        (strcmp(rs->actual->vipd.fecha_envio, a.fecha_envio) == 0)
        && (strcmp(rs->actual->vipd.fecha_recepcion, a.fecha_recepcion) == 0) &&
        (strcmp(rs->actual->vipd.nombre, a.nombre) == 0)
        && (strcmp(rs->actual->vipd.nombre_r, a.nombre_r) == 0)) {


        if (rs->envios[i].inicio == rs->actual) {
            rs->envios[i].inicio = rs->actual->siguiente;
            rs->anterior = rs->envios[i].inicio;


        } else {
            rs->anterior->siguiente = rs->actual->siguiente;

        }
        free(rs->actual);
        rs->actual = rs->envios[i].inicio;
        rs->cant--;
        return 1;
    } else
        return 0;


}

int evocarRS(RS *rs, Envio a, Envio *envios) {
    if (rs->cant == 0)
        return 0;

    int pos;


    if (LocalizarRS(rs, &pos, a.codigo, 0) == 0) {

        return 0;
    } else {
        
        strcpy(envios->codigo, rs->actual->vipd.codigo);
        envios->dni_remitente = rs->actual->vipd.dni_receptor;
        strcpy(envios->nombre_r, rs->actual->vipd.nombre_r);
        strcpy(envios->direccion, rs->actual->vipd.direccion);
        envios->dni_remitente = rs->actual->vipd.dni_remitente;
        strcpy(envios->nombre, rs->actual->vipd.nombre);
        envios->dni_remitente = rs->actual->vipd.dni_remitente;
        strcpy(envios->fecha_envio, rs->actual->vipd.fecha_envio);
        strcpy(envios->fecha_recepcion, rs->actual->vipd.fecha_recepcion);

        return 1;
    }


}

void printRS(RS *rs) {
    for (int i = 0; i < MAXRS; ++i) {

        Nodo *current = rs->envios[i].inicio;

        if (rs->envios[i].inicio == NULL) {
            printf("Balde N %d: No contiene envios\n", i + 1);
        } else {
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