/**
 * Archivo: Cliente.cpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Implementación de la clase Cliente. Cada instancia representa a un
 * usuario del buscador que se ejecuta como hilo independiente y orquesta
 * el ciclo completo de una búsqueda: espera a que el Monitor le conceda
 * acceso a una réplica, lanza en paralelo tantos Buscadores como núcleos
 * hardware haya por cada libro del directorio data/, recoge y filtra los
 * resultados según su tipo de usuario y libera la réplica al terminar.
 *
 * Funciones implementadas:
 *  - Cliente        (constructor)
 *  - operator()     (punto de entrada del hilo)
 *  - realizarBusqueda (lanzamiento de buscadores y recopilación de resultados)
 *
 * Autores: [Jose Matías Ruiz Valero y Héctor García Rubio]
 * Fecha: 2026-05-14
 */

#include "Cliente.hpp"
#include "Monitor.hpp"
#include "SistemaPago.hpp"
#include <iostream>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

Cliente::Cliente(int id, TipoUsuario tipo, std::string palabra, Monitor *m, SistemaPago *sp, int saldoInicial)
    : id(id), tipo(tipo), palabraBusqueda(palabra), saldo(saldoInicial), monitor(m), sistemaPago(sp)
{
    this->maxPalabras = 10;
    this->tiempoBusqueda = 0.0;
}

void Cliente::operator()()
{
    auto inicio = std::chrono::high_resolution_clock::now();
    monitor->esperarTurno(this->tipo);
    realizarBusqueda();
    monitor->liberarTurno();
    auto fin = std::chrono::high_resolution_clock::now();
    this->tiempoBusqueda = std::chrono::duration<double>(fin - inicio).count();
}

void Cliente::realizarBusqueda()
{
    std::vector<std::string> libros;
    for (const auto &entry : fs::directory_iterator("data/"))
    {
        if (entry.path().filename() != "diccionario.txt")
        {
            libros.push_back(entry.path().string());
        }
    }

    std::vector<std::thread> hilosLibros;
    std::vector<Buscador> buscadores;

    unsigned int nHilosPorLibro = std::thread::hardware_concurrency();
    if (nHilosPorLibro == 0)
        nHilosPorLibro = 2;

    buscadores.reserve(libros.size() * nHilosPorLibro);
    hilosLibros.reserve(libros.size() * nHilosPorLibro);

    for (const auto &rutaLibro : libros)
    {
        std::ifstream file(rutaLibro);
        if (file.is_open())
        {

            auto offsets = Buscador::calcularOffsets(file, nHilosPorLibro);

            for (unsigned int i = 0; i < nHilosPorLibro; i++)
            {

                int pIni = (i * 100) / nHilosPorLibro;
                int pFin = ((i + 1) * 100) / nHilosPorLibro;

                buscadores.emplace_back(id, offsets[i], offsets[i + 1], palabraBusqueda, i + 1, pIni, pFin);
                hilosLibros.emplace_back(std::ref(buscadores.back()), rutaLibro);
            }
        }
    }

    for (auto &t : hilosLibros)
    {
        if (t.joinable())
            t.join();
    }

    for (auto &b : buscadores)
    {
        for (const auto &res : b.vectorBusquedas)
        {
            if (tipo == TipoUsuario::Gratuito && resultados.size() >= maxPalabras)
            {
                return;
            }

            if (tipo == TipoUsuario::Premium)
            {
                if (saldo <= 0)
                {

                    sistemaPago->solicitarPago(this->id, this->saldo);
                }
                saldo--;
            }

            resultados.push_back(res);
        }
    }
}