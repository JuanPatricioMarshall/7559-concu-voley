//
// Created by navent on 10/10/17.
//

#ifndef RESTO_RESULTADO_H
#define RESTO_RESULTADO_H


#include "Pareja.h"


class Resultado {

private:

    Pareja *pareja1;
    Pareja *pareja2;

    int setsPareja1;
    int setsPareja2;

public:

    Resultado(Pareja* pareja1, Pareja* pareja2, bool nuevo);

    Pareja* getPareja1();

    Pareja* getPareja2();

    int getSetsPareja1();

    int getSetsPareja2();

    void setSetsPareja1(int sets);
    void setSetsPareja2(int sets);
    ~Resultado();


};


#endif //RESTO_RESULTADO_H
