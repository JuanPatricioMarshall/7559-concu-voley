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

    int lenght = strResultado.length();

    strResultado  = intToString(lenght) + SEPARADOR + strResultado;


    return strResultado;
}

Resultado ResultadoSerializer::deserializar(string resultadoStr) {

    istringstream ss(resultadoStr);

    string largo;
    string clave1IndiceStr;
    string clave1PIDStr;

    string largo2;
    string clave2IndiceStr;
    string clave2PIDStr;

    string largo3;
    string clave3IndiceStr;
    string clave3PIDStr;


    string largo4;
    string clave4IndiceStr;
    string clave4PIDStr;



    string setsPareja1;
    string setsPareja2;


    getline(ss, largo, SEPARADOR);
    getline(ss, clave1IndiceStr, SEPARADOR);
    getline(ss, clave1PIDStr, SEPARADOR);


    getline(ss, largo2, SEPARADOR);
    getline(ss, clave2IndiceStr, SEPARADOR);
    getline(ss, clave2PIDStr, SEPARADOR);


    getline(ss, largo3, SEPARADOR);
    getline(ss, clave3IndiceStr, SEPARADOR);
    getline(ss, clave3PIDStr, SEPARADOR);


    getline(ss, largo4, SEPARADOR);
    getline(ss, clave4IndiceStr, SEPARADOR);
    getline(ss, clave4PIDStr, SEPARADOR);


    string clave1Str = clave1PIDStr + ClaveJugadorSerializer::SEPARADOR + clave1IndiceStr;
    string clave2Str = clave2PIDStr + ClaveJugadorSerializer::SEPARADOR + clave2IndiceStr;
    string clave3Str = clave3PIDStr + ClaveJugadorSerializer::SEPARADOR + clave3IndiceStr;
    string clave4Str = clave4PIDStr + ClaveJugadorSerializer::SEPARADOR + clave4IndiceStr;


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