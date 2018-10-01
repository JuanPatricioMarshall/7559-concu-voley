#ifndef SIGINT_HANDLER_PARTIDOS_H_
#define SIGINT_HANDLER_PARTIDOS_H_

#include <signal.h>
#include <assert.h>

#include "EventHandler.h"

class SIGINT_Handler_Partidos : public EventHandler {

private:
    sig_atomic_t gracefulQuit;

public:

    SIGINT_Handler_Partidos () : gracefulQuit(0) {
    }

    ~SIGINT_Handler_Partidos () {
    }

    virtual int handleSignal ( int signum ) {

        assert ( signum == SIGINT );
        this->gracefulQuit = 1;
        return 0;
    }

    sig_atomic_t getGracefulQuit () const {
        return this->gracefulQuit;
    }

};

#endif /* SIGINT_HANDLER_PARTIDOS_H_ */
