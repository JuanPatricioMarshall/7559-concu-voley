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

    string std::ClaveJugadorSerializer::intToString(int i) {
        stringstream stream;
        stream <<  i;
        return stream.str();
    }


    ClaveJugador ClaveJugadorSerializer::deserializarClaveJugador(string claveJugadorStr) {
        istringstream ss(claveJugadorStr);
        string n;
        string pid;


        getline(ss, pid, SEPARADOR); //Tipo de dato
        getline(ss, n, SEPARADOR);

        int a = -1;
        int b = -1;

        try {
            a=stoi(pid);
            b=stoi(n);


        }catch (int e)
        {
            cout << "stoi en clave jugador serializer" << e << '\n';
        }

        ClaveJugador clave(a, b);


        return clave;

    }



    ClaveJugadorSerializer::~ClaveJugadorSerializer() {
        // TODO Auto-generated destructor stub
    }

} /* namespace std */

