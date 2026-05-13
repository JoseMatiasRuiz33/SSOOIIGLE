#include "Monitor.hpp"

Monitor::Monitor(int nReplicas) : replicasLibres(nReplicas) {}

void Monitor::esperarTurno(TipoUsuario tipo)
{
    std::unique_lock<std::mutex> lock(mtx);

    if (tipo == TipoUsuario::Gratuito)
    {
        esperandoGratuito++;

        cvGratuito.wait(lock, [this]()
                        { return replicasLibres > 0 && (esperandoPremium == 0 || contadorGratuitos < 2); });

        esperandoGratuito--;
        contadorGratuitos++;
    }
    else
    {

        esperandoPremium++;

        cvPremium.wait(lock, [this]()
                       { return replicasLibres > 0 && (esperandoGratuito == 0 || contadorPremium < 8); });

        esperandoPremium--;
        contadorPremium++;
    }

    replicasLibres--;

    if (contadorPremium + contadorGratuitos >= 10)
    {
        contadorPremium = 0;
        contadorGratuitos = 0;
    }
}

void Monitor::liberarTurno()
{
    std::lock_guard<std::mutex> lock(mtx);

    replicasLibres++;

    cvPremium.notify_all();
    cvGratuito.notify_all();
}