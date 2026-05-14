#ifndef CLIENTE_HPP
#define CLIENTE_HPP

/**
 * Archivo: Cliente.hpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Define la clase Cliente y el enumerado TipoUsuario.
 * Un Cliente representa a un usuario del buscador SSOOIIGLE que,
 * al ejecutarse como hilo independiente, solicita acceso a una
 * réplica del sistema (a través del Monitor), realiza una búsqueda
 * concurrente sobre todos los libros del directorio data/ y, si es
 * de tipo Premium con saldo insuficiente, solicita una recarga al
 * SistemaPago antes de continuar.
 *
 * Tipos de usuario (TipoUsuario):
 *  - Gratuito         : Limitado a un máximo de maxPalabras resultados.
 *  - Premium          : Sin límite de resultados pero con coste por
 *                       ocurrencia; solicita recarga al agotarse el saldo.
 *  - PremiumIlimitado : Sin límite de resultados ni coste por ocurrencia.
 *
 * Funciones principales:
 *  - operator()       : Punto de entrada del hilo. Mide el tiempo total
 *                       de la operación (espera + búsqueda) y coordina
 *                       la reserva/liberación de réplica con el Monitor.
 *  - realizarBusqueda : Lanza un conjunto de objetos Buscador en paralelo
 *                       (uno por hilo de hardware por libro), recoge los
 *                       resultados y aplica las restricciones de tipo.
 *
 * Uso:
 *  Cliente c(id, tipo, palabra, &monitor, &sistemaPago, saldoInicial);
 *  std::thread t(std::ref(c));
 *  t.join();
 *
 * Dependencias:
 *  - C++17 (std::filesystem para iterar data/)
 *  - Monitor.hpp, SistemaPago.hpp, Buscador.hpp, ResultadoBusqueda.hpp
 *
 * Autores: [Jose Matías Ruiz Valero y Héctor García Rubio]
 * Fecha: 2026-05-14
 */
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