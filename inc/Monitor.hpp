#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <mutex>
#include <condition_variable>
#include "Cliente.hpp"

class Monitor {
private:
    int replicasLibres;
    int contadorPremium = 0;
    int contadorGratuitos = 0;
    int esperandoPremium = 0;
    int esperandoGratuito = 0;
    
    std::mutex mtx;
    std::condition_variable cvPremium;
    std::condition_variable cvGratuito;

public:
    Monitor(int nReplicas);
    void esperarTurno(TipoUsuario tipo);
    void liberarTurno();
};

#endif