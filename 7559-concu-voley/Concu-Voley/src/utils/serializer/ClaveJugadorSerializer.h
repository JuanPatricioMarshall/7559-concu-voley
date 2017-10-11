//
// Created by navent on 10/10/17.
//

#ifndef RESTO_CLAVEJUGADORSERIALIZER_H
#define RESTO_CLAVEJUGADORSERIALIZER_H


#include <string>

#include "../../model/ClaveJugador.h"

namespace std {

    class ClaveJugadorSerializer {
    private:

        static string intToString(int i);
        static string doubleToString(double d);

    public:

        static const char SEPARADOR = ',';


        ClaveJugadorSerializer();

        static string serializar(ClaveJugador* pedido);

        static ClaveJugador deserializarClaveJugador(string claveJugadorStr);

        virtual ~ClaveJugadorSerializer();
    };

} /* namespace std */

#endif //RESTO_CLAVEJUGADORSERIALIZER_H
