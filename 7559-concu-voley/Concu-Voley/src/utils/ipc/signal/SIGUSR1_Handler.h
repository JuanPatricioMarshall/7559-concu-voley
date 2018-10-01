#ifndef SIGUSR1_HANDLER_H_
#define SIGUSR1_HANDLER_H_

#include <signal.h>
#include <assert.h>
#include <iostream>

#include "EventHandler.h"

class SIGUSR1_Handler : public EventHandler {

private:

public:
    pid_t *vec_id;
    int *altos;
    int n;
    vector<int > *partidosFil;
    vector<int >*partidosCol;
    vector<pid_t > *partidos;
    int col;
    sig_atomic_t nivelMarea;
    vector<vector<Semaforo>> *semCanchasLibres;
    vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres;
    SIGUSR1_Handler () : nivelMarea(0) {
    }

    ~SIGUSR1_Handler () {
    }

    virtual int handleSignal ( int signum ) {
        assert ( signum == SIGUSR1 );
        if(this->nivelMarea<col){
            this->nivelMarea = this->nivelMarea+1;
        }

        vector<int >indices;
        for(int i = 0;i<partidosFil->size();i++){

            if(nivelMarea>=partidosFil->at(i)){
                indices.push_back(i);
            }
          }

        std::cout << "Matias: indices " << indices.size() <<std::endl;
        std::cout << "Matias: partidosFil " << partidosFil->size() <<std::endl;
        std::cout << "Matias: partidos " << partidos->size() <<std::endl;
        for(int i = indices.size()-1;i>=0;i--){
            int indice = indices.at(i);
            std::cout << "Matias: Pid " << partidos->at(indice) <<std::endl;
            kill(partidos->at(indice),SIGINT);

            partidos->erase(partidos->begin() + indice);
            partidosFil->erase(partidosFil->begin() + indice);
            partidosCol->erase(partidosCol->begin() + indice);
        }

        for(int i = 0;i<col;i++){
            semCanchasLibres->at(this->nivelMarea-1).at(i).p();

            shmCanchasLibres->at(this->nivelMarea-1).at(i).escribir(false);

            semCanchasLibres->at(this->nivelMarea-1).at(i).v();
        }
        return 0;
    }

    sig_atomic_t getGracefulQuit () const {
        return this->nivelMarea;
    }

};

#endif /* SIGINT_HANDLER_H_ */
