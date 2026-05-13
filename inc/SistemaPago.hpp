#ifndef SISTEMA_PAGO_HPP
#define SISTEMA_PAGO_HPP

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