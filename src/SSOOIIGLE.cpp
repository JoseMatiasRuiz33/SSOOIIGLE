#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <stdexcept>
#include "Buscador.hpp"

#define PATH_DATA "data/"

// Función para parsear argumentos con control de excepciones
void parse_argv(int &nHilos, std::string &palabra, std::string &rutaCompleta, int argc, char *argv[]) {
    if (argc != 4) {
        throw std::invalid_argument("Formato incorrecto. Uso: ./SSOOIIGLE <fichero> <palabra> <num_hilos>");
    }

    std::string nombreFichero = argv[1];
    palabra = argv[2];
    nHilos = std::stoi(argv[3]);

    if (nHilos <= 0) {
        throw std::invalid_argument("El número de hilos debe ser mayor que 0");
    }

    rutaCompleta = std::string(PATH_DATA) + nombreFichero;
    
    std::ifstream test(rutaCompleta);
    if (!test.is_open()) {
        throw std::runtime_error("No se pudo abrir el fichero en la ruta: " + rutaCompleta);
    }
}

// Cálculo de offsets por bytes (Eficiencia I/O)
std::vector<std::streampos> calcularOffsets(const std::string &ruta, int nHilos) {
    std::ifstream file(ruta, std::ios::binary | std::ios::ate);
    std::streampos tamañoTotal = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<std::streampos> offsets;
    offsets.push_back(0);

    std::streampos tamañoTrozos = tamañoTotal / static_cast<long long>(nHilos);

    for (int i = 1; i < nHilos; ++i) {
        file.seekg(tamañoTrozos * static_cast<long long>(i));
        std::string dummy;
        std::getline(file, dummy); // Para no cortar una línea por la mitad
        offsets.push_back(file.tellg());
    }
    offsets.push_back(tamañoTotal);
    return offsets;
}

int main(int argc, char *argv[]) {
    try {
        int nHilos;
        std::string palabra, ruta;
        
        parse_argv(nHilos, palabra, ruta, argc, argv);
        
        std::vector<std::streampos> offsets = calcularOffsets(ruta, nHilos);
        
        std::vector<Buscador> buscadores;
        buscadores.reserve(nHilos); // CRÍTICO: Para que las referencias en los hilos no se rompan
        
        std::vector<std::thread> vThreads;

        // Crear y lanzar hilos
        for (int i = 0; i < nHilos; i++) {
            buscadores.emplace_back(i + 1, offsets[i], offsets[i + 1], palabra);
            // Pasamos por referencia el buscador y la ruta por valor
            vThreads.emplace_back(std::ref(buscadores.back()), ruta);
        }

        // Esperar a que terminen
        for (auto &t : vThreads) {
            t.join();
        }

        // IMPRESIÓN FINAL (Posponiendo para evitar variables de condición/turnos)
        for (const auto &b : buscadores) {
            for (const auto &r : b.vectorBusquedas) {
                // Formato exacto que pediste
                std::cout << "[Hilo " << b.id 
                          << " inicio:" << b.inicioPos 
                          << " – final: " << b.finPos << "] :: "
                          << "línea " << r.Linea << " :: … " 
                          << r.Anterior << " " << b.palabra << " " << r.Posterior 
                          << " …" << std::endl;
            }
        }

    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}