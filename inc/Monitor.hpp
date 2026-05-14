#ifndef MONITOR_HPP
#define MONITOR_HPP

/**
 * Archivo: Monitor.hpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Define la clase Monitor, que implementa un mecanismo de control de
 * acceso concurrente a un número limitado de réplicas del buscador.
 * Garantiza que, ante la competencia entre usuarios Gratuitos y Premium,
 * se respete una política de equidad configurable:
 *
 *  - Los usuarios Premium tienen prioridad mientras haya réplicas libres,
 *    pero se les permite avanzar como máximo 8 de cada 10 turnos para
 *    evitar la inanición de los usuarios Gratuitos.
 *  - Los usuarios Gratuitos solo avanzan si no hay Premium esperando,
 *    o si ya han pasado 8 Premium consecutivos (hasta 2 Gratuitos por ciclo).
 *  - El ciclo se reinicia al acumularse 10 turnos totales (8 Premium + 2 Gratuito).
 *
 * Funciones principales:
 *  - esperarTurno : Bloquea al hilo llamante hasta que haya una réplica
 *                   libre y se cumplan las condiciones de equidad para
 *                   su tipo de usuario.
 *  - liberarTurno : Libera una réplica y notifica a los hilos en espera.
 *
 * Uso:
 *  Monitor m(nReplicas);
 *  m.esperarTurno(TipoUsuario::Premium);   // bloquea hasta obtener acceso
 *  // ... sección crítica (búsqueda) ...
 *  m.liberarTurno();
 *
 * Dependencias:
 *  - <mutex>, <condition_variable>
 *  - Cliente.hpp (para el enumerado TipoUsuario)
 *
 * Autores: [Jose Matías Ruiz Valero y Héctor García Rubio]
 * Fecha: 2026-05-14
 */
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