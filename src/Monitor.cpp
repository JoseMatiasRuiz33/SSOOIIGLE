/**
 * Archivo: Monitor.cpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Implementación de la clase Monitor. Controla el acceso concurrente
 * de los clientes a un número limitado de réplicas del buscador,
 * garantizando una política de equidad 8:2 entre usuarios Premium
 * y Gratuitos para evitar la inanición de estos últimos.
 *
 * Política de equidad:
 *  - En cada ciclo de 10 turnos se permiten como máximo 8 Premium y 2 Gratuitos.
 *  - Un Premium puede avanzar si: hay réplicas libres Y (no hay Gratuitos
 *    esperando O el ciclo aún no ha consumido 8 Premium).
 *  - Un Gratuito puede avanzar si: hay réplicas libres Y (no hay Premium
 *    esperando O el ciclo ya ha consumido 8 Premium, dejando paso libre).
 *  - Al acumularse 10 turnos (contadorPremium + contadorGratuitos >= 10)
 *    ambos contadores se reinician, comenzando un nuevo ciclo.
 *
 * Funciones implementadas:
 *  - Monitor        (constructor)
 *  - esperarTurno   (bloqueo hasta obtener acceso con la política de equidad)
 *  - liberarTurno   (devolución de la réplica y notificación a la cola)
 *
 * Autores: [Jose Matías Ruiz Valero y Héctor García Rubio]
 * Fecha: 2026-05-14
 */
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