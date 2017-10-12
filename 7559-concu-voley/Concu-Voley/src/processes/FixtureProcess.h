//
// Created by navent on 12/10/17.
//

#ifndef INC_7559_CONCU_VOLEY_FIXTUREPROCESS_H
#define INC_7559_CONCU_VOLEY_FIXTUREPROCESS_H

#include <vector>
#include "Process.h"
#include "../utils/ipc/pipe/Pipe.h"
#include "../model/Resultado.h"


namespace std {

    const string fixtureProcessId = "Fixture";


    class FixtureProcess : public Process {

    private:


        Pipe *pipeFixture;

        vector<Resultado> resultados;


        int leerTamanioResultado();

        void imprimirFixture();


    public:

        FixtureProcess(Pipe *pipeFixture);

        void run();

        void limpiarRecursos();

        virtual ~FixtureProcess();

    };

}


#endif //INC_7559_CONCU_VOLEY_FIXTUREPROCESS_H
