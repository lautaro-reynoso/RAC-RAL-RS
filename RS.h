#ifndef RS_H_INCLUDED
#define RS_H_INCLUDED
#include "Envios.h"
#define MAXRS 37 //p=1.45 por lo que el resultado es techo(206.8965)=207 y no hace falta acercarlo al numero primo mas cercano

struct nodo{
Envio vipd;
struct Nodo *ps;
};typedef struct nodo Nodo;

typedef struct {
Nodo *cur;
Nodo *acc;
Nodo *aux;} lista_de_envios;

typedef struct{
    lista_de_envios envios[MAXRS];
    int cant;
    int indicador[MAXRS];
}RS;

void initializeRS(RS *rs){
    int i;
    for (i=0;i<MAXRS;i++){
        //init(&rs->ships[i]);
        Nodo *actual = rs->envios[i].acc;
        Nodo *siguiente;

    while (actual != NULL) {
        siguiente = actual->ps;
        free(actual);
        actual = siguiente;
    }
        rs->envios[i].acc = NULL;
        rs->envios[i].cur = NULL;
        rs->envios[i].aux = NULL;
        rs->indicador[i]=0;
    }
    rs->cant=0;
}

int LocalizeRs(RS *rs, int *j, char *code, float *costo){

    int i=hashing(code,MAXRS);
    (*j)=i;
    if(rs->envios[i].acc == NULL){
        (*costo)++;
        return 0;
    }
    //reset(&rs->ships[i]);
    rs->envios[i].cur=rs->envios[i].acc;
    rs->envios[i].aux=rs->envios[i].acc;
    while(rs->envios[i].cur != NULL && strcmp(rs->envios[i].cur->vipd.codigo, code)!=0){
        //foward(&rs->ships[i]);
        rs->envios[i].aux=rs->envios[i].cur;
        rs->envios[i].cur= rs->envios[i].cur->ps;
        (*costo)++;
    }
    if (rs->envios[i].cur!=NULL)
        return 1;
    else
        return 0;
}


void insertl(lista_de_envios *lista, Envio a) {
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    nuevo->vipd = a;
    nuevo->ps = lista->cur;
    lista->cur = nuevo;

    if (lista->acc == NULL) {
        lista->acc = nuevo;
    }
}





int altaRS(RS *rs, Envio a){
    if (rs->cant==MAXRS)
        return 0;

    int i, costos;

    if(LocalizeRs(rs,&i,a.codigo,&costos)==1)
        return 0;

    insertl(&rs->envios[i],a);

     Nodo *p;
    p=(Nodo*)malloc(sizeof(Nodo));
    if (rs->envios[i].cur == rs->envios[i].acc){
(*p).vipd=a;
(*p).ps=rs->envios[i].cur;
rs->envios[i].acc=p;
rs->envios[i].cur=p;
}
else{
    (*p).vipd=a;
    (*p).ps=rs->envios[i].cur;
    rs->envios[i].aux->ps=p;
    rs->envios[i].cur=p;
}
    rs->cant++;
    rs->indicador[i]=1;
    return 1;
}





int bajaRS(RS *rs, Envio a){
    if (rs->cant==0)
        return 0;

    int i, costos;

    if(LocalizeRs(rs,&i,a.codigo,&costos)==0)
        return 0;


    if (rs->envios[i].acc == rs->envios[i].cur){
        rs->envios[i].acc = rs->envios[i].cur->ps;
        rs->envios[i].aux=rs->envios[i].cur->ps;
        free(rs->envios[i].cur);
        rs->envios[i].cur = rs->envios[i].acc;
    }
    else
    {
        rs->envios[i].aux->ps=rs->envios[i].cur->ps;
        free(rs->envios[i].cur);
        rs->envios[i].cur=rs->envios[i].aux->ps;

    }
    rs->cant--;

}

int evocarRS(RS *rs, Envio a, float *costo, Envio *aux){
    if(rs->cant==0)
        return 0;

    int i;
    float costos=0.0;

    if(LocalizeRs(rs,&i,a.codigo,&costos)==0){

        return 0;
    } else {

        return 1;
    }



}
void printRS(RS *rs) {
    for (int i = 0; i < MAXRS; ++i) {
        printf("Balde N %d:\n", i + 1);
        Nodo *current = rs->envios[i].acc;
        int count = 1;  // Contador para elementos dentro de la lista
        while (current != NULL) {
            if (strcmp(current->vipd.codigo, LIBRE) == 0) {
                printf("\t--------------------------------------");
                printf("\n\tElemento N #%d de balde %d \n", count, i+1);
                printf("\tCELDA LIBRE +\n");
                printf("\t--------------------------------------\n\n");
            } else if (strcmp(current->vipd.codigo, VIRGEN) == 0) {
                printf("\n\t--------------------------------------");
                printf("\n\tElemento N #%d de balde %d \n", count, i+1);
                printf("\tCELDA VIRGEN *\n");
                printf("\t--------------------------------------\n\n");
            } else {
                printf("\n\t--------------------------------------");
                printf("\n\tElemento N #%d de balde %d\n", count, i+1);
                mostrarenvio(current->vipd);
                printf("\t--------------------------------------\n");
            }

            if (count % 5 == 0) {
                printf("Presiona Enter para continuar...\n");
                getchar();  // Pausa, espera a que el usuario presione Enter
            }

            current = current->ps;
            count++;
        }
    }
}









#endif // RAL_H_INCLUDED
