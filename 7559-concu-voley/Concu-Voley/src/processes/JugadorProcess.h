//
// Created by navent on 09/10/17.
//

#ifndef JUGADORPROCESS_H
#define JUGADORPROCESS_H


#include <termio.h>
#include "../main/MainProcess.h"
#include "Process.h"
#include <vector>

namespace std {

    const string jugadorLogId = "Jugador";


    class JugadorProcess : public Process {

    private:

        int cantidadDePartidosPendientes;

        int indice;

        vector<Semaforo> *semPartidoTerminado;
        Semaforo *semJugadoresPredio;
        Semaforo *semEsperarRecepcionista;
        Pipe *pipeJugadores;


        void llegar();

        void jugar();

        void decidirQueHacer();

        void terminar();

        void reentrar();


    public:

        JugadorProcess(int cantidadDePartidosPendientes,
                       vector<Semaforo> *semPartidoTerminado, int indice,
                       Semaforo *semEsperarRecepcionista, Semaforo *semJugadoresPredio, Pipe *pipeJugadores);


        void run();

        void limpiarRecursos();

        virtual ~JugadorProcess();

    };

}
#endif //JUGADORPROCESS_H
