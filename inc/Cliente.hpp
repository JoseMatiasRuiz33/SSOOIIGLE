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

class Cliente
{
public:
    int id;
    TipoUsuario tipo;
    std::string palabraBusqueda;
    int saldo;
    int maxPalabras;
    double tiempoBusqueda;
    std::vector<ResultadoBusqueda> resultados;

    std::mutex mtxResultados;

    Cliente(int id, TipoUsuario tipo, std::string palabra, int saldoInicial = 0);

    void operator()();

    void realizarBusqueda();
};

#endif