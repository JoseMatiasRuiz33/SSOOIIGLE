#ifndef SISTEMA_PAGO_HPP
#define SISTEMA_PAGO_HPP

/**
 * Archivo: SistemaPago.hpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Define la clase SistemaPago, que modela un servicio de recarga de
 * créditos para los usuarios Premium. Se ejecuta en un hilo dedicado
 * y atiende las solicitudes de recarga de forma secuencial, simulando
 * el tiempo de procesamiento de una pasarela de pago real.
 *
 * Mecanismo de funcionamiento:
 *  1. El hilo de SistemaPago permanece dormido esperando solicitudes
 *     mediante una variable de condición (cv_solicitud).
 *  2. Cuando un Cliente Premium agota su saldo, llama a solicitarPago(),
 *     que deposita los datos de la recarga y despierta al hilo de pago.
 *  3. SistemaPago procesa la solicitud (simula 500 ms de latencia),
 *     recarga 10 créditos en el saldo del cliente y señaliza la
 *     finalización mediante cv_confirmacion.
 *  4. El cliente que llamó a solicitarPago() se desbloquea y reanuda
 *     su búsqueda con el nuevo saldo.
 *  5. Al finalizar la simulación, el hilo principal llama a parar()
 *     para que el hilo de SistemaPago termine limpiamente.
 *
 * Funciones principales:
 *  - operator()     : Bucle principal del hilo; espera y procesa recargas.
 *  - solicitarPago  : Llamada bloqueante desde un hilo Cliente; deposita
 *                     la solicitud y espera confirmación de la recarga.
 *  - parar          : Señaliza al hilo que debe terminar su ejecución.
 *
 * Uso:
 *  SistemaPago sp;
 *  std::thread hiloSP(std::ref(sp));   // lanza el bucle de procesamiento
 *  // ... los clientes llaman a sp.solicitarPago() cuando sea necesario ...
 *  sp.parar();
 *  hiloSP.join();
 *
 * Dependencias:
 *  - <mutex>, <condition_variable>, <thread>, <chrono>
 *
 * Autores: [Jose Matías Ruiz Valero y Héctor García Rubio]
 * Fecha: 2026-05-14
 */
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <chrono>

class SistemaPago
{
private:
    std::mutex mtx;
    std::condition_variable cv_solicitud;
    std::condition_variable cv_confirmacion;

    int idClienteActual = -1;
    int *saldoClienteActual = nullptr;
    bool haySolicitud = false;
    bool pagoFinalizado = false;
    bool detener = false;

public:
    void operator()();

    void solicitarPago(int id, int &saldo);

    void parar();
};

#endif