//
// Created by navent on 10/10/17.
//

#include "ResultadoSerializer.h"
#include "ClaveJugadorSerializer.h"
#include <iomanip>
#include <iostream>
#include <sstream>


string ResultadoSerializer::serializar(Resultado *resultado) {

    string strClave1 = ClaveJugadorSerializer::serializar(resultado->getPareja1()->getClaveJugador1());
    string strClave2 = ClaveJugadorSerializer::serializar(resultado->getPareja1()->getClaveJugador2());
    string strClave3 = ClaveJugadorSerializer::serializar(resultado->getPareja2()->getClaveJugador1());
    string strClave4 = ClaveJugadorSerializer::serializar(resultado->getPareja2()->getClaveJugador2());

    string strResultado =
            strClave1 + SEPARADOR + strClave2 + SEPARADOR + strClave3 + SEPARADOR + strClave4 + SEPARADOR +
            ResultadoSerializer::intToString(resultado->getSetsPareja1()) + SEPARADOR +
            ResultadoSerializer::intToString(resultado->getSetsPareja2());

    return strResultado;
}

Resultado ResultadoSerializer::deserializar(string resultadoStr) {

    istringstream ss(resultadoStr);

    string clave1Str;
    string clave2Str;
    string clave3Str;
    string clave4Str;

    string setsPareja1;
    string setsPareja2;


    getline(ss, clave1Str, SEPARADOR);
    getline(ss, clave2Str, SEPARADOR);
    getline(ss, clave3Str, SEPARADOR);
    getline(ss, clave4Str, SEPARADOR);

    ClaveJugador claveJugador1 = ClaveJugadorSerializer::deserializarClaveJugador(clave1Str);
    ClaveJugador claveJugador2 = ClaveJugadorSerializer::deserializarClaveJugador(clave2Str);
    ClaveJugador claveJugador3 = ClaveJugadorSerializer::deserializarClaveJugador(clave3Str);
    ClaveJugador claveJugador4 = ClaveJugadorSerializer::deserializarClaveJugador(clave4Str);

    getline(ss, setsPareja1, SEPARADOR);
    getline(ss, setsPareja2, SEPARADOR);


    Pareja pareja1(&claveJugador1, &claveJugador2);
    Pareja pareja2(&claveJugador3, &claveJugador4);


    Resultado resultado(&pareja1, &pareja2, false);


    resultado.setSetsPareja1(stoi(setsPareja1));
    resultado.setSetsPareja2(stoi(setsPareja2));


    return resultado;


}

string ResultadoSerializer::intToString(int setsGanado){

    stringstream stream;
    stream <<  setsGanado;
    return stream.str();

}



ResultadoSerializer::~ResultadoSerializer() {

}