

#ifndef UTILS_IPC_SIGNAL_SIGUSR1_HANDLER_PARTIDOS_H_
#define UTILS_IPC_SIGNAL_SIGUSR1_HANDLER_PARTIDOS_H_


#include <signal.h>
#include <assert.h>

#include "EventHandler.h"
#include "../../../processes/Process.h"
#include <iostream>

class SIGUSR1_Handler_Partidos : public EventHandler {

	private:
		Process* process;
		sig_atomic_t gracefulQuit;
		vector<vector<Semaforo>> *semCanchasLibres;
		vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres;
		Cancha *cancha;

	public:

		SIGUSR1_Handler_Partidos (
			vector<vector<Semaforo>> *semCanchasLibres,
			vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres,
			Cancha *cancha
		) : gracefulQuit(0) {

			this->semCanchasLibres=semCanchasLibres;
			this->shmCanchasLibres=shmCanchasLibres;
			this->cancha=cancha;
		}

		~SIGUSR1_Handler_Partidos () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGUSR1 );
			std::cout << "llegue"<<std::endl;
			int fila = cancha->getFila();
			int columna = cancha->getColumna();

			this->semCanchasLibres->at(fila).at(columna).p();
			this->shmCanchasLibres->at(fila).at(columna).escribir(true);

			this->semCanchasLibres->at(fila).at(columna).v();

			this->semCantCanchasLibres->v();


			return 0;
		}

		sig_atomic_t getGracefulQuit () const {
			return this->gracefulQuit;
		}

		void setGracefulQuit(sig_atomic_t gracefulQuit){
			this->gracefulQuit = gracefulQuit;
		}
};




#endif /* UTILS_IPC_SIGNAL_SIGUSR1_HANDLER_PARTIDOS_H_ */
