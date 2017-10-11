//
// Created by navent on 10/10/17.
//

#ifndef INC_7559_CONCU_VOLEY_RESULTADOSERIALIZER_H
#define INC_7559_CONCU_VOLEY_RESULTADOSERIALIZER_H


#include "../../model/Resultado.h"
#include <string>

using namespace std;

class ResultadoSerializer {

private:

    static string intToString(int setsGanado);

public:


    static const char SEPARADOR = ',';


    static string serializar(Resultado* resultado);

    static Resultado deserializar(string resultadoStr);

    ResultadoSerializer();

    virtual ~ResultadoSerializer();

};


#endif //INC_7559_CONCU_VOLEY_RESULTADOSERIALIZER_H
