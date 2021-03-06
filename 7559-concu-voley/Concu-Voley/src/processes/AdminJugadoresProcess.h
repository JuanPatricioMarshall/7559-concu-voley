
#ifndef PROCESSES_ADMIN_JUGADORES_PROCESS_H_
#define PROCESSES_ADMIN_JUGADORES_PROCESS_H_

#include <sched.h>
#include <vector>

#include "../utils/ipc/semaphore/Semaforo.h"
#include "../utils/ipc/shared-memory/MemoriaCompartida.h"
#include "../utils/ipc/pipe/Pipe.h"
#include "../utils/random/RandomUtil.h"
#include "../utils/logger/Logger.h"
#include "TiemposEspera.h"


namespace std {

    const string adminJugadoresLogId = "adminJugadores";

    class AdminJugadoresProcess {
    private:

        int cantJugadores;
        int cantPartidosPorJugador;


        vector<Semaforo> *semPartidoTerminado;
        Semaforo *semEsperarRecepcionista;
        Semaforo *semJugadoresPredio;
        Pipe *pipeJugadores;

        vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja;
        vector<Semaforo> *semJugadoresSinPareja;

        MemoriaCompartida<int> *shmCantGenteEnElPredio;
        Semaforo *semCantGenteEnElPredio;


        void inicializarHandler();


    public:
        AdminJugadoresProcess(int cantJugadores, int cantPartidosPorJugador,
                              vector<Semaforo> *semPartidoTerminado,
                              Semaforo *semEsperarRecepcionista, Semaforo *semJugadoresPredio, Pipe *pipeJugadores,
                              vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja,
                              vector<Semaforo> *, MemoriaCompartida<int> *shmCantGenteEnElPredio,
                              Semaforo *semCantGenteEnElPredio);

        int run();

        virtual ~AdminJugadoresProcess();
    };

} /* namespace std */

#endif
