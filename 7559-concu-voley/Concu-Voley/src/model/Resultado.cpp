//
// Created by navent on 10/10/17.
//

#include "Resultado.h"
#include "../utils/random/RandomUtil.h"
#include "../processes/TiemposEspera.h"


Resultado::Resultado(Pareja* pareja1, Pareja* pareja2, bool nuevo){

    this->pareja1 = pareja1;
    this->pareja2 = pareja2;

    double random = RandomUtil::randomCeroUno();

    int sets1 = -1;
    int sets2 = -1;

    if(nuevo) {
        if (random < TiemposEspera::PROBABILIDAD_3_A_0) {
            sets1 = 3;
            sets2 = 0;
        } else if (random < TiemposEspera::PROBABILIDAD_3_A_1) {
            sets1 = 3;
            sets2 = 1;
        } else {
            sets1 = 3;
            sets2 = 2;
        }

        if (RandomUtil::randomCeroUno() > TiemposEspera::PROBABILIDAD_GANAR_PAREJA_1) {
            int aux = sets2;

            sets2 = sets1;
            sets1 = aux;
        }

        this->setsPareja1 = sets1;
        this->setsPareja2 = sets2;
    }
}

Pareja* Resultado::getPareja1(){

    return this->pareja1;

}

Pareja* Resultado::getPareja2(){
    return this->pareja2;
}

int Resultado::getSetsPareja1(){
    return this->setsPareja1;
}

int Resultado::getSetsPareja2(){
    return this->setsPareja2;
}

void Resultado::setSetsPareja1(int sets){
    this->setsPareja1 = sets;
}
void Resultado::setSetsPareja2(int sets){
    this->setsPareja2 = sets;
}

Resultado::~Resultado(){

}