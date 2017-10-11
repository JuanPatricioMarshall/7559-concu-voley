//
// Created by navent on 10/10/17.
//

#ifndef RESTO_CANCHA_H
#define RESTO_CANCHA_H


class Cancha {

private:

    int fila;
    int columna;

public:

    Cancha(int fila, int columna);

    int getFila();

    void setFila(int fila);

    void setColumna(int columna);

    int getColumna();

    ~Cancha();

};


#endif //RESTO_CANCHA_H
