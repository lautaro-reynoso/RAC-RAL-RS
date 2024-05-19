

#include "Envios.h"
#define MAXRS 207 //p=1.45 por lo que el resultado es techo(206.8965)=207 y no hace falta acercarlo al numero primo mas cercano

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
}rs;

void initializeRS(rs *rs){
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

int LocalizeRs(rs *rs, int *j, char *code, float *costo){

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

int altaRS(rs *rs, Envio a){
    if (rs->cant==MAXRS)
        return 0;

    int i, costos;

    if(LocalizeRs(rs,&i,a.codigo,&costos)==1)
        return 0;

    //insertl(&rs->ships[i],a);
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

int bajaRS(rs *rs, Envio a){
    if (rs->cant==0)
        return 0;

    int i, costos;

    if(LocalizeRs(rs,&i,a.codigo,&costos)==0)
        return 0;

    //supress(&rs->ships[i]);
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

int evocarRS(rs *rs, Envio a, float *costo, Envio *aux){
    if(rs->cant==0)
        return 0;

    int i;
    float costos=0.0;

    if(LocalizeRs(rs,&i,a.codigo,&costos)==0){
        *costo=costos;
        return 0;
    } else {
        *costo=costos;
        return 1;
    }



}


//#endif // RS_H_INCLUDED
