cmake_minimum_required(VERSION 3.8)
project(resto)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        7559-restorrente/Concu-Voley/src/main/ConcuVoley.cpp
        7559-restorrente/Concu-Voley/src/main/MainProcess.cpp
        7559-restorrente/Concu-Voley/src/main/MainProcess.h
        7559-restorrente/Concu-Voley/src/model/Comida.cpp
        7559-restorrente/Concu-Voley/src/model/Comida.h
        7559-restorrente/Concu-Voley/src/model/LlamadoAMozo.cpp
        7559-restorrente/Concu-Voley/src/model/LlamadoAMozo.h
        7559-restorrente/Concu-Voley/src/model/Menu.cpp
        7559-restorrente/Concu-Voley/src/model/Menu.h
        7559-restorrente/Concu-Voley/src/model/Partido.cpp
        7559-restorrente/Concu-Voley/src/model/Partido.h
        7559-restorrente/Concu-Voley/src/model/Pedido.cpp
        7559-restorrente/Concu-Voley/src/model/Pedido.h
        7559-restorrente/Concu-Voley/src/model/PedidoCuenta.cpp
        7559-restorrente/Concu-Voley/src/model/PedidoCuenta.h
        7559-restorrente/Concu-Voley/src/model/Plato.cpp
        7559-restorrente/Concu-Voley/src/model/Plato.h
        7559-restorrente/Concu-Voley/src/processes/AdminComensalesProcess.cpp
        7559-restorrente/Concu-Voley/src/processes/AdminComensalesProcess.h
        7559-restorrente/Concu-Voley/src/processes/AdminJugadoresProcess.h
        7559-restorrente/Concu-Voley/src/processes/AdminJugadoresProcess.cpp
        7559-restorrente/Concu-Voley/src/processes/CocineroProcess.cpp
        7559-restorrente/Concu-Voley/src/processes/CocineroProcess.h
        7559-restorrente/Concu-Voley/src/processes/GerenteProcess.cpp
        7559-restorrente/Concu-Voley/src/processes/GerenteProcess.h
        7559-restorrente/Concu-Voley/src/processes/GrupoComensalesProcess.cpp
        7559-restorrente/Concu-Voley/src/processes/GrupoComensalesProcess.h
        7559-restorrente/Concu-Voley/src/processes/MozoProcess.cpp
        7559-restorrente/Concu-Voley/src/processes/MozoProcess.h
        7559-restorrente/Concu-Voley/src/processes/Process.cpp
        7559-restorrente/Concu-Voley/src/processes/Process.h
        7559-restorrente/Concu-Voley/src/processes/TiemposEspera.h
        7559-restorrente/Concu-Voley/src/utils/ipc/fifo/Fifo.cpp
        7559-restorrente/Concu-Voley/src/utils/ipc/fifo/Fifo.h
        7559-restorrente/Concu-Voley/src/utils/ipc/fifo/FifoEscritura.cpp
        7559-restorrente/Concu-Voley/src/utils/ipc/fifo/FifoEscritura.h
        7559-restorrente/Concu-Voley/src/utils/ipc/fifo/FifoLectura.cpp
        7559-restorrente/Concu-Voley/src/utils/ipc/fifo/FifoLectura.h
        7559-restorrente/Concu-Voley/src/utils/ipc/lock/LockFile.cpp
        7559-restorrente/Concu-Voley/src/utils/ipc/lock/LockFile.h
        7559-restorrente/Concu-Voley/src/utils/ipc/pipe/Pipe.cpp
        7559-restorrente/Concu-Voley/src/utils/ipc/pipe/Pipe.h
        7559-restorrente/Concu-Voley/src/utils/ipc/semaphore/Semaforo.cpp
        7559-restorrente/Concu-Voley/src/utils/ipc/semaphore/Semaforo.h
        7559-restorrente/Concu-Voley/src/utils/ipc/shared-memory/MemoriaCompartida.h
        7559-restorrente/Concu-Voley/src/utils/ipc/signal/EventHandler.h
        7559-restorrente/Concu-Voley/src/utils/ipc/signal/SIGINT_Handler.h
        7559-restorrente/Concu-Voley/src/utils/ipc/signal/SIGINT_ProcessHandler.h
        7559-restorrente/Concu-Voley/src/utils/ipc/signal/SignalHandler.cpp
        7559-restorrente/Concu-Voley/src/utils/ipc/signal/SignalHandler.h
        7559-restorrente/Concu-Voley/src/utils/ipc/signal/SIGUSR1_Handler.h
        7559-restorrente/Concu-Voley/src/utils/logger/Logger.cpp
        7559-restorrente/Concu-Voley/src/utils/logger/Logger.h
        7559-restorrente/Concu-Voley/src/utils/parser/pugixml/pugiconfig.hpp
        7559-restorrente/Concu-Voley/src/utils/parser/pugixml/pugixml.cpp
        7559-restorrente/Concu-Voley/src/utils/parser/pugixml/pugixml.hpp
        7559-restorrente/Concu-Voley/src/utils/parser/Parser.cpp
        7559-restorrente/Concu-Voley/src/utils/parser/Parser.h
        7559-restorrente/Concu-Voley/src/utils/random/RandomUtil.cpp
        7559-restorrente/Concu-Voley/src/utils/random/RandomUtil.h
        7559-restorrente/Concu-Voley/src/utils/serializer/LlamadoAMozoSerializer.cpp
        7559-restorrente/Concu-Voley/src/utils/serializer/LlamadoAMozoSerializer.h
        7559-restorrente/corteLuz.sh
        7559-restorrente/verInfo.sh 7559-restorrente/Concu-Voley/src/processes/JugadorProcess.cpp 7559-restorrente/Concu-Voley/src/processes/JugadorProcess.h 7559-restorrente/Concu-Voley/src/processes/RecepcionistaProcess.cpp 7559-restorrente/Concu-Voley/src/processes/RecepcionistaProcess.h 7559-restorrente/Concu-Voley/src/processes/CupidoProcess.cpp 7559-restorrente/Concu-Voley/src/processes/CupidoProcess.h 7559-restorrente/Concu-Voley/src/model/ClaveJugador.cpp 7559-restorrente/Concu-Voley/src/model/ClaveJugador.h 7559-restorrente/Concu-Voley/src/utils/serializer/ClaveJugadorSerializer.cpp 7559-restorrente/Concu-Voley/src/utils/serializer/ClaveJugadorSerializer.h 7559-restorrente/Concu-Voley/src/model/Pareja.cpp 7559-restorrente/Concu-Voley/src/model/Pareja.h 7559-restorrente/Concu-Voley/src/processes/PartidoProcess.cpp 7559-restorrente/Concu-Voley/src/processes/PartidoProcess.h 7559-restorrente/Concu-Voley/src/model/Cancha.cpp 7559-restorrente/Concu-Voley/src/model/Cancha.h 7559-restorrente/Concu-Voley/src/model/Resultado.cpp 7559-restorrente/Concu-Voley/src/model/Resultado.h)

add_executable(resto ${SOURCE_FILES})