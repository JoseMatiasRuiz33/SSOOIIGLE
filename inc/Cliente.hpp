#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "ResultadoBusqueda.hpp"
#include "Buscador.hpp"

enum class TipoUsuario
{
    Gratuito,
    Premium,
    PremiumIlimitado
};

class Monitor;
class SistemaPago;

class Cliente
{
public:
    int id;
    TipoUsuario tipo;
    std::string palabraBusqueda;
    int saldo;
    size_t maxPalabras;
    double tiempoBusqueda;
    std::vector<ResultadoBusqueda> resultados;

    Monitor *monitor;
    SistemaPago *sistemaPago;

    Cliente(int id, TipoUsuario tipo, std::string palabra, Monitor *m, SistemaPago *sp, int saldoInicial = 0);

    void operator()();
    void realizarBusqueda();
};

#endif