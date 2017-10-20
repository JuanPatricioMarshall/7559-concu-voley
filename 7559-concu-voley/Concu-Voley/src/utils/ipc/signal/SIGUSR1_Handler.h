

#ifndef UTILS_IPC_SIGNAL_SIGUSR1_HANDLER_H_
#define UTILS_IPC_SIGNAL_SIGUSR1_HANDLER_H_


#include <signal.h>
#include <assert.h>

#include "EventHandler.h"
#include "../../../processes/Process.h"
#include <iostream>

class SIGUSR1_Handler : public EventHandler {

	private:
		Process* process;
		sig_atomic_t gracefulQuit;
		vector<pid_t> lista_de_partidos;
	public:

		SIGUSR1_Handler (vector<pid_t>  lista) : gracefulQuit(0) {
			lista_de_partidos = lista;
		}

		~SIGUSR1_Handler () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGUSR1 );
			std::cout << "llegue"<<std::endl;
			for(int i = 0;i<lista_de_partidos.size();i++){
				kill(lista_de_partidos.at(i),SIGUSR1);
			}

			return 0;
		}

		sig_atomic_t getGracefulQuit () const {
			return this->gracefulQuit;
		}

		void setGracefulQuit(sig_atomic_t gracefulQuit){
			this->gracefulQuit = gracefulQuit;
		}
};




#endif /* UTILS_IPC_SIGNAL_SIGUSR1_HANDLER_H_ */
