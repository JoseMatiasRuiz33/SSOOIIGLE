#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <stdexcept>
#include "Buscador.hpp"

#define PATH_DATA "data/"

void parse_argv(int *nHilos, std::ifstream *fichero,
                std::string *palabra, std::string *ruta,
                int argc, char *argv[])
{

    try
    {

        if (argc != 4)
        {
            throw std::invalid_argument("Uso: ./SSOOIIGLE <archivo> <palabra> <hilos>");
        }

        *ruta = PATH_DATA;
        fichero->open((*ruta += (*++argv)));

        if (fichero->fail())
        {
            throw std::runtime_error("No se pudo abrir el fichero");
        }

        *palabra = *++argv;

        *nHilos = atoi(*++argv);

        if (*nHilos <= 0)
        {
            throw std::invalid_argument("Número de hilos inválido");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Error] " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::vector<int> calcularLineas(std::string ruta, const std::vector<std::streampos> &offsets)
{
    std::ifstream file(ruta);
    std::vector<int> lineas;

    lineas.push_back(1);

    int contadorActual = 1;
    std::string dummy;

    for (size_t i = 1; i < offsets.size(); i++)
    {

        while (file.tellg() < offsets[i] && std::getline(file, dummy))
        {
            contadorActual++;
        }

        lineas.push_back(contadorActual);
    }

    return lineas;
}

std::vector<std::streampos> calcularOffsets(std::ifstream &file, int nHilos)
{

    std::vector<std::streampos> offsets(nHilos + 1);

    file.seekg(0, std::ios::end);
    int totalBytes = file.tellg();
    file.seekg(0, std::ios::beg);

    int bytesPorHilo = totalBytes / nHilos;

    offsets[0] = 0;

    for (int i = 1; i < nHilos; i++)
    {
        int posicion = i * bytesPorHilo;
        file.seekg(posicion);
        std::string lineaDescartada;
        std::getline(file, lineaDescartada);
        offsets[i] = file.tellg();
    }

    offsets[nHilos] = totalBytes;

    file.clear();
    file.seekg(0, std::ios::beg);

    return offsets;
}

void crearBuscadores(std::string ruta, std::string palabra,
                     std::vector<Buscador> &buscadores,
                     std::vector<std::thread> &vThreads,
                     std::vector<std::streampos> &offsets)
{

    int nHilos = offsets.size() - 1;

    buscadores.reserve(nHilos);

    for (int i = 0; i < nHilos; i++)
    {
        buscadores.emplace_back(i + 1, offsets[i], offsets[i + 1], palabra);
        vThreads.emplace_back(std::ref(buscadores.back()), ruta);
    }

    for (std::thread &t : vThreads)
    {
        t.join();
    }
}

int main(int argc, char *argv[])
{

    try
    {

        int nHilos;
        std::string palabra, ruta;
        std::ifstream fichero;

        std::vector<std::thread> vThreads;
        std::vector<Buscador> buscadores;

        parse_argv(&nHilos, &fichero, &palabra, &ruta, argc, argv);

        std::vector<std::streampos> offsets = calcularOffsets(fichero, nHilos);
        std::vector<int> lineas = calcularLineas(ruta, offsets);

        crearBuscadores(ruta, palabra, buscadores, vThreads, offsets);

        for (size_t i = 0; i < buscadores.size(); i++)
        {
            for (const auto &r : buscadores[i].vectorBusquedas)
            {
                std::cout << "[Hilo " << buscadores[i].id
                          << " inicio:" << lineas[i]
                          << " – final: " << lineas[i + 1]
                          << "] :: línea " << r.Linea
                          << " :: … " << r.Anterior
                          << " " << buscadores[i].palabra
                          << " " << r.Posterior << " …" << std::endl;
            }
        }
        std::cout << "Hilos: " << buscadores.size() << std::endl;

        for (auto &b : buscadores)
        {
            std::cout << "Hilo " << b.id << " resultados: "
                      << b.vectorBusquedas.size() << std::endl;
        }
        for (auto o : offsets)
        {
            std::cout << o << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[FATAL ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}