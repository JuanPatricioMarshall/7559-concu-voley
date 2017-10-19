//
// Created by navent on 10/10/17.
//

#include "CupidoProcess.h"
#include "../utils/serializer/ClaveJugadorSerializer.h"
#include "PartidoProcess.h"
#include "../main/MainProcess.h"
#include "../utils/ipc/signal/SignalHandler.h"

CupidoProcess::CupidoProcess(Pipe *jugadores, vector<vector<Semaforo>> *semCanchasLibres,
                             vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres, int cantNJugadores,int cantPartidosJugador,
                             Semaforo *semCupido, vector<Semaforo> *semsTerminoDeJugar, Semaforo *semCantCanchasLibres,
                             Pipe *pipeResultados, Pipe *pipeFixture, int cantJugadoresMinimosParaElTorneo,
                             vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja,
                             MemoriaCompartida<int> *shmNivelDeMarea,
                             Semaforo *semNivelDeMarea, vector<Semaforo> *semJugadoresSinPareja,
                             Semaforo *semCantGenteEnElPredio,
                             MemoriaCompartida<int> *shmCantGenteEnElPredio) {

    this->jugadores = jugadores;
    this->cantNJugadores = cantNJugadores;
    this->cantPartidosJugador = cantPartidosJugador;
    this->cantJugadoresMinimosParaElTorneo = cantJugadoresMinimosParaElTorneo;
    this->semCupido = semCupido;
    this->pipeFixture = pipeFixture;
    this->pipeResultados = pipeResultados;

    this->semCantGenteEnElPredio = semCantGenteEnElPredio;
    this->shmCantGenteEnElPredio = shmCantGenteEnElPredio;

    this->shmNivelDeMarea = shmNivelDeMarea;
    this->semNivelDeMarea = semNivelDeMarea;

    this->parejaEnEspera = new Pareja(new ClaveJugador(-1, -1), new ClaveJugador(-1, -1));

    this->semCanchasLibres = semCanchasLibres;

    this->shmCanchasLibres = shmCanchasLibres;

    this->shmJugadoresSinPareja = shmJugadoresSinPareja;
    this->semJugadoresSinPareja = semJugadoresSinPareja;

    this->semsTerminoDeJugar = semsTerminoDeJugar;

    this->semCantCanchasLibres = semCantCanchasLibres;


    for (int i = 0; i < cantJugadoresMinimosParaElTorneo; i++) {
        this->jugadoresSinPareja.push_back(ClaveJugador(-1, -1));
    }

    for (int i = 0; i < cantNJugadores; i++) {

        vector<bool> filaDeMatcheo;
        for (int j = 0; j < cantNJugadores; j++) {
            filaDeMatcheo.push_back(false);
        }

        this->matrizDeMatcheo.push_back(filaDeMatcheo);
    }

    inicializarHandler();
    inicializarMemoriasCompartidas();

}

void CupidoProcess::inicializarMemoriasCompartidas() {


    for (unsigned int i = 0; i < shmJugadoresSinPareja->size(); i++) {
        semJugadoresSinPareja->at(i).p();
        shmJugadoresSinPareja->at(i).crear(SHM_JUGADORES_SIN_PAREJA, i);
        semJugadoresSinPareja->at(i).v();

    }
    int cont = 0;
    for (unsigned int i = 0; i < shmCanchasLibres->size(); i++) {
        for (unsigned int j = 0; j < shmCanchasLibres->at(0).size(); j++) {
            shmCanchasLibres->at(i).at(j).crear(SHM_CANCHAS_LIBRES, cont);
            cont ++;
        }
    }

}

void CupidoProcess::inicializarHandler() {

    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigintHandler);
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1Handler);
    SignalHandler::getInstance()->registrarHandler(SIGUSR2, &sigusr2Handler);

}

void CupidoProcess::run() {

    Logger::log(cupidoProcessLogId, "Iniciando Cupido ", DEBUG);


    this->semCupido->p();
    Logger::log(cupidoProcessLogId, "Ya entraron " + Logger::intToString(cantJugadoresMinimosParaElTorneo) +
                                    " personas, asi que empiezo el torneo", DEBUG);


    bool isAlive = true;
    Logger::log(cupidoProcessLogId, "Cupido empieza a leer del pipe", DEBUG);
    semCantGenteEnElPredio->p();
    shmCantGenteEnElPredio->crear(SHM_GENTE_EN_EL_PREDIO, 0);
    semCantGenteEnElPredio->v();
    bool subidaMarea = false;
    bool bajadaMarea = false;
    LOG_MODE mode = DEBUG;
    int ocupadas = 0;
    int contPartidos = 0;
    int ciclos = 0;
    int sentadas=0;
    int cant= 0;

    int datos[8];
    for(int i=0;i<8;i++){
        datos[i]=-1;
    }

    int mesas [10];
    int pids [10];
    for(int i=0;i<10;i++){
        mesas[i]=-1;
        pids[i]=-1;
    }

    while (isAlive) {
        this->semCantGenteEnElPredio->p();
        cant = this->shmCantGenteEnElPredio->leer();
        this->semCantGenteEnElPredio->v();
        Logger::log(cupidoProcessLogId, "Cantidad De Partidos " + Logger::intToString(contPartidos), INFO);
        Logger::log(cupidoProcessLogId, "Cantidad De Ciclos " + Logger::intToString(ciclos), INFO);
        ciclos++;

        Logger::log(cupidoProcessLogId, "Cantidad De Personas En El Predio " + Logger::intToString(cant), INFO);
        int tamanioClaveJugador = leerTamanioClaveJugador();

        char buffer[tamanioClaveJugador];
        string claveJugadorStr;


        Logger::log(cupidoProcessLogId, "Cupido espera leer del pipe de jugadores",INFO);



        jugadores->leer(static_cast<void *>(buffer), tamanioClaveJugador);


        claveJugadorStr = buffer;




        ClaveJugador claveJugador = ClaveJugadorSerializer::deserializarClaveJugador(claveJugadorStr);
        Logger::log(cupidoProcessLogId, "Matias: Cupido leyo del pipe de jugadores: " + Logger::intToString(claveJugador.getPid()), INFO);

        Logger::log(cupidoProcessLogId,
                    "Cupido buscando pareja para " + Logger::intToString(claveJugador.getPid()) + " indice: " +
                    Logger::intToString(claveJugador.getIndice()), mode);




        if(ciclos==(cantNJugadores*cantPartidosJugador)){
            int indice = -1;
            for(int aux = 0;aux<10;aux++){
                indice=mesas[aux];
                if(indice != -1){
                    semJugadoresSinPareja->at(indice).p();
                    shmJugadoresSinPareja->at(indice).escribir(true);
                    semJugadoresSinPareja->at(indice).v();
                    semsTerminoDeJugar->at(indice).v();
                }
            }

            indice=datos[1];
            if(indice != -1){
                semJugadoresSinPareja->at(indice).p();
                shmJugadoresSinPareja->at(indice).escribir(true);
                semJugadoresSinPareja->at(indice).v();
                semsTerminoDeJugar->at(indice).v();
            }
            indice=datos[3];
            if(indice != -1){
                semJugadoresSinPareja->at(indice).p();
                shmJugadoresSinPareja->at(indice).escribir(true);
                semJugadoresSinPareja->at(indice).v();
                semsTerminoDeJugar->at(indice).v();
            }
            indice=claveJugador.getIndice();
            if(indice != -1){
                semJugadoresSinPareja->at(indice).p();
                shmJugadoresSinPareja->at(indice).escribir(true);
                semJugadoresSinPareja->at(indice).v();
                semsTerminoDeJugar->at(indice).v();
            }
            break;

        }




        bool seQuedaEnElPredio = false;

        Logger::log(cupidoProcessLogId,
                    "Cupido tiene " + Logger::intToString(jugadoresSinPareja.size())+" mesas con "+Logger::intToString(ocupadas)+ " ocupadas" , mode);

        logTables(claveJugador,pids,mesas,datos);

        for(int i = 0; i < 10; i++){
            if(mesas[i]!=-1){
                if(!matrizDeMatcheo[mesas[i]][claveJugador.getIndice()]){
                    matrizDeMatcheo[mesas[i]][claveJugador.getIndice()]=true;
                    matrizDeMatcheo[claveJugador.getIndice()][mesas[i]]=true;
                    sentadas++;
                    ClaveJugador claveJugadorSinPareja(pids[i],mesas[i]);

                    seQuedaEnElPredio=true;
                    mesas[i]=-1;
                    pids[i]=-1;

                    if(datos[0]==-1){
                        datos[0]=claveJugador.getPid();
                        datos[1]=claveJugador.getIndice();
                        datos[2]=claveJugadorSinPareja.getPid();
                        datos[3]=claveJugadorSinPareja.getIndice();

                    }else{
                        datos[4]=claveJugador.getPid();
                        datos[5]=claveJugador.getIndice();
                        datos[6]=claveJugadorSinPareja.getPid();
                        datos[7]=claveJugadorSinPareja.getIndice();
                        inicializarPartido(new Pareja(new ClaveJugador(datos[0], datos[1]),
                                                      new ClaveJugador(datos[2], datos[3])),
                                           new Pareja(new ClaveJugador(datos[4], datos[5]),
                                                      new ClaveJugador(datos[6], datos[7])));

                        for(int au =0;au<8;au++){
                            datos[au]=-1;
                        }
                        contPartidos++;
                        sentadas=sentadas-4;
                    }
                    break;

                }
            }
        }


        if(!seQuedaEnElPredio){
            for(int i = 0; i < 10; i++) {
                if (mesas[i] == -1) {
                    seQuedaEnElPredio = true;
                    mesas[i] = claveJugador.getIndice();
                    pids[i] = claveJugador.getPid();
                    sentadas++;
                    break;
                }
            }

        }
        Logger::log(cupidoProcessLogId, "Cantidad De Personas Sentadas " + Logger::intToString(sentadas), INFO);

        if (!seQuedaEnElPredio) {
            Logger::log(cupidoProcessLogId, "Cupido le avisa al jugador " + Logger::intToString(claveJugador.getPid()) +
                                            " que se vaya del predio porque no encontro pareja", INFO);

            semJugadoresSinPareja->at(claveJugador.getIndice()).p();
            shmJugadoresSinPareja->at(claveJugador.getIndice()).escribir(true);
            semJugadoresSinPareja->at(claveJugador.getIndice()).v();
            semsTerminoDeJugar->at(claveJugador.getIndice()).v();

        }




    }

    Logger::log(cupidoProcessLogId, "Lets die for the love of God" , INFO);

}
void CupidoProcess::logTables(ClaveJugador claveJugador,int* pids,int* indices,int* datos){
    Logger::log(cupidoProcessLogId, "Matias: [ "+
                                    Logger::intToString(claveJugador.getPid())+" # "+
                                    Logger::intToString(claveJugador.getIndice())
                                    +" ]", INFO);

    Logger::log(cupidoProcessLogId, "Matias: Mesas[ "  +
            Logger::intToString(pids[0]) + " | " +
            Logger::intToString(pids[1]) + " | " +
            Logger::intToString(pids[2]) + " | " +
            Logger::intToString(pids[3]) + " | " +
            Logger::intToString(pids[4]) + " | " +
            Logger::intToString(pids[5]) + " | " +
            Logger::intToString(pids[6]) + " | " +
            Logger::intToString(pids[7]) + " | " +
            Logger::intToString(pids[8]) + " | " +
            Logger::intToString(pids[9])+
                                    " ]", INFO);

    Logger::log(cupidoProcessLogId, "Matias: [ " +
                                    Logger::intToString(indices[0]) +" | " +
                                    Logger::intToString(indices[1]) +" | " +
                                    Logger::intToString(indices[2]) +" | " +
                                    Logger::intToString(indices[3]) +" | " +
                                    Logger::intToString(indices[4]) +" | " +
                                    Logger::intToString(indices[5]) +" | " +
                                    Logger::intToString(indices[6]) +" | " +
                                    Logger::intToString(indices[7]) +" | " +
                                    Logger::intToString(indices[8]) +" | " +
                                    Logger::intToString(indices[9]) +
                                    " ]", INFO);
    Logger::log(cupidoProcessLogId, "Matias:[ "+
            Logger::intToString(datos[0])+" # "+
            Logger::intToString(datos[1])+" | "+
            Logger::intToString(datos[2])+" # "+
            Logger::intToString(datos[3])
                                    +" ]", INFO);
}
void CupidoProcess::handleSubida() {

    for(int i = 0; i < partidos.size(); i++){

       kill(partidos.at(i), SIGUSR1);
    }


}

void CupidoProcess::handleBajada() {

    for(int i = 0; i < partidos.size(); i++){

        kill(partidos.at(i), SIGUSR2);
    }

}

void CupidoProcess::limpiarRecursos() {

    liberarMemoriasCompartidas();


}

void CupidoProcess::liberarMemoriasCompartidas() {

    for (unsigned int i = 0; i < shmJugadoresSinPareja->size(); i++) {
        shmJugadoresSinPareja->at(i).liberar();
    }
}

CupidoProcess::~CupidoProcess() {

}

int CupidoProcess::leerTamanioClaveJugador() {
    char tamanioClaveJugador;
    string tamanioClaveJugadorStr;
    bool finLectura = false;
    Logger::log(cupidoProcessLogId, "Cupido esperando para leer tamanio de clave Jugador", DEBUG);
    do {
        Logger::log(cupidoProcessLogId, "Cupido esperando para leer pipeJugadores", DEBUG);

        jugadores->leer(static_cast<void *>(&tamanioClaveJugador), sizeof(char));

        Logger::log(cupidoProcessLogId,
                    "Cupido leyo " + Logger::intToString(tamanioClaveJugador) + " de tamaño desde pipeJugadores",
                    DEBUG);

        finLectura = (tamanioClaveJugador == ClaveJugadorSerializer::SEPARADOR);
        if (!finLectura) {
            tamanioClaveJugadorStr += tamanioClaveJugador;
        }

    } while (!finLectura);

    return stoi(tamanioClaveJugadorStr);
}

void CupidoProcess::inicializarPartido(Pareja *pareja1, Pareja *pareja2) {
    logCanchas();
    Logger::log(cupidoProcessLogId,
                "Matias: Inicio un partido entre " + Logger::intToString(pareja1->getClaveJugador1()->getIndice()) + ", " +
                Logger::intToString(pareja1->getClaveJugador2()->getIndice()) + " y " +
                Logger::intToString(pareja2->getClaveJugador1()->getIndice()) + ", " +
                Logger::intToString(pareja2->getClaveJugador2()->getIndice()), INFO);


    pid_t idPartido = fork();

    if (idPartido == 0) {

        PartidoProcess partidoProcess(pareja1, pareja2, semCanchasLibres, shmCanchasLibres, semsTerminoDeJugar,
                                      semCantCanchasLibres, pipeResultados, pipeFixture, shmNivelDeMarea,
                                      semNivelDeMarea);


        partidoProcess.run();


        exit(0);
    } else {

        partidos.push_back(idPartido);

    }

}
void CupidoProcess::logCanchas(){

    Logger::log(cupidoProcessLogId,
                "Estado de Canchas: " , INFO);
    bool canchaLibre = false;
    for (unsigned int i = 0; i < semCanchasLibres->size(); i++) {
        string fila="Estado de Canchas: [";
        for (unsigned int j = 0; j < semCanchasLibres[0].size(); j++) {


            semCanchasLibres->at(i).at(j).p();


            canchaLibre = shmCanchasLibres->at(i).at(j).leer();

            if (canchaLibre) {
                fila = fila +" 0 ";

            }else{
                fila = fila +" 1 ";
            }


            semCanchasLibres->at(i).at(j).v();

          }
        fila = fila + "]";
        Logger::log(cupidoProcessLogId,
                    fila , INFO);


    }



}