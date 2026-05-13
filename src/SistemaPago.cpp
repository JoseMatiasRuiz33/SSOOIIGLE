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