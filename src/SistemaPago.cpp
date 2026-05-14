/**
 * Archivo: SistemaPago.cpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Implementación de la clase SistemaPago, que modela un servicio de
 * recarga de créditos para los usuarios Premium. Opera mediante el
 * patrón productor-consumidor con doble variable de condición:
 *
 *  - cv_solicitud:    despierta al hilo de SistemaPago cuando un cliente
 *                     deposita una solicitud o cuando se ordena la parada.
 *  - cv_confirmacion: despierta al cliente solicitante cuando la recarga
 *                     ha sido procesada y el saldo ha sido actualizado.
 *
 * Funciones implementadas:
 *  - operator()     (bucle principal del hilo procesador de pagos)
 *  - solicitarPago  (llamada bloqueante desde un hilo cliente)
 *  - parar          (señal de parada limpia del hilo)
 *
 * Autores: [Jose Matías Ruiz Valero y Héctor García Rubio]
 * Fecha: 2026-05-14
 */

#include "SistemaPago.hpp"

void SistemaPago::operator()()
{
    while (!detener)
    {
        std::unique_lock<std::mutex> lock(mtx);

        cv_solicitud.wait(lock, [this]
                          { return haySolicitud || detener; });

        if (detener)
            break;

        std::cout << "[SistemaPago] Procesando pago para Cliente " << idClienteActual << "..." << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (saldoClienteActual)
        {
            *saldoClienteActual += 10;
        }

        pagoFinalizado = true;
        haySolicitud = false;

        cv_confirmacion.notify_all();
    }
}

void SistemaPago::solicitarPago(int id, int &saldo)
{
    std::unique_lock<std::mutex> lock(mtx);

    idClienteActual = id;
    saldoClienteActual = &saldo;
    haySolicitud = true;
    pagoFinalizado = false;

    cv_solicitud.notify_one();

    cv_confirmacion.wait(lock, [this]
                         { return pagoFinalizado; });
}

void SistemaPago::parar()
{
    std::lock_guard<std::mutex> lock(mtx);
    detener = true;
    cv_solicitud.notify_one();
}