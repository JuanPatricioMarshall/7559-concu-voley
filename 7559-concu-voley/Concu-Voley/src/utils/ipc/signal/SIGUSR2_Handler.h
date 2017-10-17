

#ifndef UTILS_IPC_SIGNAL_SIGUSR2_HANDLER_H_
#define UTILS_IPC_SIGNAL_SIGUSR2_HANDLER_H_


#include <signal.h>
#include <assert.h>

#include "EventHandler.h"

class SIGUSR2_Handler : public EventHandler {

	private:
		sig_atomic_t gracefulQuit;

	public:

		SIGUSR2_Handler () : gracefulQuit(0) {
		}

		~SIGUSR2_Handler () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGUSR2 );
			this->gracefulQuit = 1;
			return 0;
		}

		sig_atomic_t getGracefulQuit () const {
			return this->gracefulQuit;
		}

		void setGracefulQuit(sig_atomic_t gracefulQuit){
			this->gracefulQuit = gracefulQuit;
		}
};




#endif /* UTILS_IPC_SIGNAL_SIGUSR2_HANDLER_H_ */
