//
// Created by lautaro on 19/8/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef UNTITLED_ENVIOS_H
#define UNTITLED_ENVIOS_H
#define MAX_REGISTROS 1000
#define MAX_LONGITUD 100
#define VIRGEN "*"
#define LIBRE "+"

#define MAX_Envios 250

typedef struct {
    char codigo[7];
    int dni_receptor;
    char nombre[80];
    char direccion[80];
    int dni_remitente;
    char nombre_r[80];
    char fecha_envio[MAX_LONGITUD];
    char fecha_recepcion[MAX_LONGITUD];


}Envio;

int hashing( char* x, int M) {
    int longitud , i;
    int contador = 0;
    longitud = strlen(x);
    for ( i = 0; i < longitud ; i++ )
        contador +=( (int)x[i] ) * ( i+1 );
    return ( contador % M ) ;

}

void mostrarenvio (Envio envio){

    printf("Codigo: %s\n", envio.codigo);
    printf("Dni receptor: %d\n", envio.dni_receptor);
    printf("Nombre y Apellido: %s\n", envio.nombre);
    printf("Direccion: %s\n", envio.direccion);
    printf("Dni remitente: %d\n", envio.dni_remitente);
    printf("Fecha de envio: %s\n", envio.fecha_envio);
    printf("Fecha recepcion: %s\n", envio.fecha_recepcion);
    printf("Nombre receptor: %s\n\n", envio.nombre_r);

}

int compararenvios (Envio envio1,Envio envio2){
    if((strcmp(envio1.direccion , envio2.direccion)==0)
    && (envio1.dni_receptor == envio2.dni_receptor)
    && (envio1.dni_remitente == envio2.dni_remitente)
    && (strcmp(envio1.fecha_envio,envio2.fecha_envio)==0)
    && (strcmp(envio1.fecha_recepcion,envio2.fecha_recepcion)==0)
    && (strcmp(envio1.nombre,envio2.nombre)==0)
    && (strcmp(envio1.nombre_r,envio2.nombre_r)==0)){
        return 1;
    }

    return 0;
}

#endif //UNTITLED_ENVIOS_H
