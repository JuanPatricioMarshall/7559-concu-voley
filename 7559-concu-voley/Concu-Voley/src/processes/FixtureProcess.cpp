//
// Created by navent on 12/10/17.
//

#include "FixtureProcess.h"
#include "../utils/serializer/ResultadoSerializer.h"

FixtureProcess::FixtureProcess(Pipe *pipeFixture) {

    this->pipeFixture = pipeFixture;

}

int FixtureProcess::leerTamanioResultado() {
    char tamanioResutlado;
    string tamanioResultadoStr;
    bool finLectura = false;
    Logger::log(fixtureProcessId, "Fixture esperando para leer tamanio de Resultado", DEBUG);
    do {
        Logger::log(fixtureProcessId, "Cupido esperando para leer pipeJugadores", DEBUG);

        pipeFixture->leer(static_cast<void *>(&tamanioResutlado), sizeof(char));

        Logger::log(fixtureProcessId,
                    "Fixture leyo " + Logger::intToString(tamanioResutlado) + " de tamaño desde pipeFixture",
                    DEBUG);

        finLectura = (tamanioResutlado == ResultadoSerializer::SEPARADOR);
        if (!finLectura) {
            tamanioResultadoStr += tamanioResutlado;
        }

    } while (!finLectura);

    return stoi(tamanioResultadoStr);
}

void FixtureProcess::run() {


    bool isAlive = true;

    int cont = 0;

    while (isAlive) {

        int tamanioResultado = leerTamanioResultado();


        char buffer[tamanioResultado];
        string resultadoStr;


        Logger::log(fixtureProcessId, "Fixture espera leer del pipe de resultados", DEBUG);


        pipeFixture->leer(static_cast<void *>(buffer), tamanioResultado);

        resultadoStr = buffer;


        Logger::log(fixtureProcessId, "Fixture leyo del pipe de resultados: " + resultadoStr, DEBUG);


        Resultado resultado = ResultadoSerializer::deserializar(resultadoStr);

        resultados.push_back(resultado);

        cont++;

        Logger::log(fixtureProcessId, "Detalles de partidos hasta ahora: cantidad: " + Logger::intToString(resultados.size()), DEBUG);

        if (cont == 2) {
            imprimirFixture();
            cont = 0;
        }

    }
}

void FixtureProcess::imprimirFixture() {


    Logger::log(fixtureProcessId, "Detalles de partidos hasta ahora: cantidad: " + Logger::intToString(resultados.size()), DEBUG);


    string resultadoTotal = "";
    for (int i = 0; i < resultados.size(); i++) {
        Resultado resultado = resultados.at(i);
        string pareja1 = Logger::intToString(resultado.getPareja1()->getClaveJugador1()->getPid()) + ", " +
                         Logger::intToString(resultado.getPareja1()->getClaveJugador2()->getPid()) + ": " +
                         Logger::intToString(resultado.getSetsPareja1());
        string pareja2 = Logger::intToString(resultado.getPareja2()->getClaveJugador1()->getPid()) + ", " +
                         Logger::intToString(resultado.getPareja2()->getClaveJugador2()->getPid()) + ": " +
                         Logger::intToString(resultado.getSetsPareja2());

        string resultadoStr = pareja1 + " - " + pareja2;
        resultadoTotal = resultadoTotal + resultadoStr + "\n";
    }

    Logger::log(fixtureProcessId, "Detalles de partidos hasta ahora: " + resultadoTotal, DEBUG);


}

void FixtureProcess::limpiarRecursos() {

}

FixtureProcess::~FixtureProcess() {

}




