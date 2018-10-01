#ifndef SIGUSR1_HANDLER_RANKING_H_
#define SIGUSR1_HANDLER_RANKING_H_

#include <signal.h>
#include <assert.h>
#include <iostream>

#include "EventHandler.h"

class SIGUSR1_Handler_Ranking : public EventHandler {

private:

public:
    sig_atomic_t *sig;
    sig_atomic_t nivelMarea;
    SIGUSR1_Handler_Ranking () : nivelMarea(0) {
    }

    ~SIGUSR1_Handler_Ranking () {
    }

    virtual int handleSignal ( int signum ) {
        assert ( signum == SIGUSR1 );
        std::cout<<"Imprimo rankinszz "<<*sig<<std::endl;

        *sig = *sig+1;
        return 0;
    }

    sig_atomic_t getGracefulQuit () const {
        return *sig;
    }

};

#endif /* SIGUSR1_HANDLER_RANKING_H_ */
