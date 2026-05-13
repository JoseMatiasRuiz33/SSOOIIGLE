#include "Cliente.hpp"
#include <iostream>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

Cliente::Cliente(int id, TipoUsuario tipo, std::string palabra, int saldoInicial)
    : id(id), tipo(tipo), palabraBusqueda(palabra), saldo(saldoInicial)
{
    this->maxPalabras = 10;
    this->tiempoBusqueda = 0.0;
}

void Cliente::operator()()
{
    auto inicio = std::chrono::high_resolution_clock::now();

    std::cout << "[Cliente " << id << "] Empezando búsqueda...." << std::endl;

    realizarBusqueda();

    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> transcurrido = fin - inicio;
    this->tiempoBusqueda = transcurrido.count();
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
    for (const auto &rutaLibro : libros)
    {
        std::ifstream file(rutaLibro);
        if (file.is_open())
        {
            // 1 hilo por libro
            auto offsets = Buscador::calcularOffsets(file, 1);
            buscadores.emplace_back(id, offsets[0], offsets[1], palabraBusqueda);
            hilosLibros.emplace_back(std::ref(buscadores.back()), rutaLibro);
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
                    // Llamamos al Sistema de Pago para recargar
                }
                saldo--;
            }

            resultados.push_back(res);
        }
    }
}