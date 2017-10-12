//
// Created by navent on 10/10/17.
//

#include "ClaveJugadorSerializer.h"

#include <iomanip>
#include <iostream>
#include <sstream>


namespace std {

    ClaveJugadorSerializer::ClaveJugadorSerializer() {

    }

    string ClaveJugadorSerializer::serializar(ClaveJugador* claveJugador){

        string claveJugadorStr = intToString(claveJugador->getPid()) + SEPARADOR + intToString(claveJugador->getIndice());

        int lenght = claveJugadorStr.length();

        claveJugadorStr  = intToString(lenght) + SEPARADOR + claveJugadorStr;

        return claveJugadorStr;

    }

    ClaveJugador ClaveJugadorSerializer::deserializarClaveJugador(string claveJugadorStr) {
        istringstream ss(claveJugadorStr);
        string n;
        string pid;


        getline(ss, pid, SEPARADOR); //Tipo de dato
        getline(ss, n, SEPARADOR);


        ClaveJugador clave(stoi(pid), stoi(n));


        return clave;

    }


    string std::ClaveJugadorSerializer::intToString(int i) {
        stringstream stream;
        stream <<  i;
        return stream.str();
    }



    ClaveJugadorSerializer::~ClaveJugadorSerializer() {
        // TODO Auto-generated destructor stub
    }

} /* namespace std */

