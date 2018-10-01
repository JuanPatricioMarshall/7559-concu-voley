cmake_minimum_required(VERSION 3.8)
project(7559_concu_voley)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Concu-Voley/ipc-init-files/shm_resultados_finales.txt
        Concu-Voley/ipc-init-files/sems_cancha_ocupada.txt
        Concu-Voley/ipc-init-files/shm_gente_en_el_predio.txt
        Concu-Voley/ipc-init-files/sem_lista_pid.txt
        Concu-Voley/ipc-init-files/sem_gente_en_el_predio.txt
        Concu-Voley/ipc-init-files/shm_lista_pid.txt
        Concu-Voley/ipc-init-files/sems_cant_canchas_libres.txt
        Concu-Voley/ipc-init-files/sems_cupido.txt
        Concu-Voley/ipc-init-files/sems_esperar_recepcionista.txt
        Concu-Voley/ipc-init-files/sems_jugadores_predio.txt
        Concu-Voley/ipc-init-files/sems_listo_para_jugar.txt
        Concu-Voley/ipc-init-files/shm_canchas_libres.txt
        Concu-Voley/ipc-init-files/sems_ranking.txt
        Concu-Voley/ipc-init-files/sems_termino_de_jugar.txt
        Concu-Voley/ipc-init-files/shm_jugadores_sin_pareja.txt
        Concu-Voley/ipc-init-files/shm_nivel_marea.txt
        Concu-Voley/ipc-init-files/sems_jugador_sin_pareja.txt
        Concu-Voley/ipc-init-files/shm_canchas_libres.txt
        Concu-Voley/ipc-init-files/sem_nivel_marea.txt
        Concu-Voley/src/main/ConcuVoley.cpp
        Concu-Voley/src/main/MainProcess.cpp
        Concu-Voley/src/main/MainProcess.h
        Concu-Voley/src/model/Cancha.cpp
        Concu-Voley/src/model/Cancha.h
        Concu-Voley/src/model/ClaveJugador.cpp
        Concu-Voley/src/model/ClaveJugador.h
        Concu-Voley/src/model/Pareja.cpp
        Concu-Voley/src/model/Pareja.h
        Concu-Voley/src/model/Resultado.cpp
        Concu-Voley/src/model/Resultado.h
        Concu-Voley/src/processes/AdminJugadoresProcess.cpp
        Concu-Voley/src/processes/AdminJugadoresProcess.h
        Concu-Voley/src/processes/CupidoProcess.cpp
        Concu-Voley/src/processes/CupidoProcess.h
        Concu-Voley/src/processes/JugadorProcess.cpp
        Concu-Voley/src/processes/JugadorProcess.h
        Concu-Voley/src/processes/PartidoProcess.cpp
        Concu-Voley/src/processes/PartidoProcess.h
        Concu-Voley/src/processes/Process.cpp
        Concu-Voley/src/processes/Process.h
        Concu-Voley/src/processes/RecepcionistaProcess.cpp
        Concu-Voley/src/processes/RecepcionistaProcess.h
        Concu-Voley/src/processes/TiemposEspera.h
        Concu-Voley/src/utils/ipc/fifo/Fifo.cpp
        Concu-Voley/src/utils/ipc/fifo/Fifo.h
        Concu-Voley/src/utils/ipc/fifo/FifoEscritura.cpp
        Concu-Voley/src/utils/ipc/fifo/FifoEscritura.h
        Concu-Voley/src/utils/ipc/fifo/FifoLectura.cpp
        Concu-Voley/src/utils/ipc/fifo/FifoLectura.h
        Concu-Voley/src/utils/ipc/lock/LockFile.cpp
        Concu-Voley/src/utils/ipc/lock/LockFile.h
        Concu-Voley/src/utils/ipc/pipe/Pipe.cpp
        Concu-Voley/src/utils/ipc/pipe/Pipe.h
        Concu-Voley/src/utils/ipc/semaphore/Semaforo.cpp
        Concu-Voley/src/utils/ipc/semaphore/Semaforo.h
        Concu-Voley/src/utils/ipc/shared-memory/MemoriaCompartida.h
        Concu-Voley/src/utils/logger/Logger.cpp
        Concu-Voley/src/utils/logger/Logger.h
        Concu-Voley/src/utils/ipc/signal/EventHandler.h
        Concu-Voley/src/utils/ipc/signal/SIGINT_Handler_Partidos.h
        Concu-Voley/src/utils/ipc/signal/SIGUSR1_Handler.h
        Concu-Voley/src/utils/ipc/signal/SIGUSR1_Handler_Ranking.h
        Concu-Voley/src/utils/ipc/signal/SignalHandler.cpp
        Concu-Voley/src/utils/ipc/signal/SignalHandler.h
        Concu-Voley/src/utils/ipc/signal/SIGUSR2_Handler.h
        Concu-Voley/src/utils/random/RandomUtil.cpp
        Concu-Voley/src/utils/random/RandomUtil.h
        Concu-Voley/src/utils/serializer/ClaveJugadorSerializer.cpp
        Concu-Voley/src/utils/serializer/ClaveJugadorSerializer.h
        Concu-Voley/src/utils/serializer/ResultadoSerializer.cpp Concu-Voley/src/utils/serializer/ResultadoSerializer.h Concu-Voley/src/processes/FixtureProcess.cpp Concu-Voley/src/processes/FixtureProcess.h)

add_executable(7559_concu_voley ${SOURCE_FILES})