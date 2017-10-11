//
// Created by navent on 10/10/17.
//

#include "Cancha.h"


Cancha::Cancha(int fila, int columna){

    this->fila = fila;
    this->columna = columna;
}

int Cancha::getFila(){
    return fila;
}
int Cancha::getColumna(){
    return columna;
}


void Cancha::setFila(int fila){

    this->fila = fila;
}

void Cancha::setColumna(int columna){
    this->columna = columna;
}

Cancha::~Cancha(){

}