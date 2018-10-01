#ifndef SIGUSR2_HANDLER_H_
#define SIGUSR2_HANDLER_H_

#include <signal.h>
#include <assert.h>
#include <iostream>

#include "EventHandler.h"
#include "../semaphore/Semaforo.h"

class SIGUSR2_Handler : public EventHandler {

private:
public:
    int col;
    sig_atomic_t *sig;
    vector<vector<Semaforo>> *semCanchasLibres;
    vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres;
    SIGUSR2_Handler ()  {
    }

    ~SIGUSR2_Handler () {
    }

    virtual int handleSignal ( int signum ) {
        assert ( signum == SIGUSR2 );
        if(*sig == 0){
            return 0;
        }
        *sig = *sig-1;
        if(*sig<0){
            *sig=0;
        }
        for(int i = 0;i<col;i++){
            semCanchasLibres->at(*sig).at(i).p();


            shmCanchasLibres->at(*sig).at(i).escribir(true);



            semCanchasLibres->at(*sig).at(i).v();
        }
        return 0;
    }

};

#endif /* SIGUSR2_HANDLER_H_ */
