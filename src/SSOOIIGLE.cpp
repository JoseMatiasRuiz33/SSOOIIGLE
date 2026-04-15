#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <stdexcept>
#include "Buscador.hpp"

#define PATH_DATA "data/"

void parse_argv(int *nHilos, std::ifstream *fichero,
                std::string *palabra, std::string *ruta,
                int argc, char *argv[]){

    try{

        if(argc != 4){
            throw std::invalid_argument("Uso: ./SSOOIIGLE <archivo> <palabra> <hilos>");
        }

        *ruta = PATH_DATA;
        fichero->open((*ruta += (*++argv)));

        if(fichero->fail()){
            throw std::runtime_error("No se pudo abrir el fichero");
        }

        *palabra = *++argv;

        *nHilos = atoi(*++argv);

        if(*nHilos <= 0){
            throw std::invalid_argument("Número de hilos inválido");
        }

    }
    catch(const std::exception &e){
        std::cerr << "[Error] " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::vector<std::streampos> calcularOffsets(std::ifstream &file, int nHilos){

    std::vector<std::streampos> offsets;
    offsets.push_back(0);

    std::string line;
    int totalLineas = 0;

    while(getline(file, line)){
        totalLineas++;
    }

    file.clear();
    file.seekg(0);

    int lineasPorHilo = totalLineas / nHilos;
    int contador = 0;

    while(getline(file, line)){
        contador++;

        if(contador % lineasPorHilo == 0 && offsets.size() < static_cast<std::size_t>(nHilos)){
            offsets.push_back(file.tellg());
        }
    }

    file.clear();
offsets.push_back(file.seekg(0, std::ios::end).tellg());
file.clear();
file.seekg(0);

    file.clear();
    file.seekg(0);

    return offsets;
}

void crearBuscadores(std::string ruta, std::string palabra,
                     std::vector<Buscador> &buscadores,
                     std::vector<std::thread> &vThreads,
                     std::vector<std::streampos> &offsets){

    int nHilos = offsets.size() - 1;

    buscadores.reserve(nHilos);

    for(int i = 0; i < nHilos; i++){
        buscadores.emplace_back(i + 1, offsets[i], offsets[i + 1], palabra);
        vThreads.emplace_back(std::ref(buscadores.back()), ruta);
    }

    for(std::thread &t : vThreads){
        t.join();
    }
}

int main(int argc, char *argv[]){

    try{

        int nHilos;
        std::string palabra, ruta;
        std::ifstream fichero;

        std::vector<std::thread> vThreads;
        std::vector<Buscador> buscadores;

        parse_argv(&nHilos, &fichero, &palabra, &ruta, argc, argv);

        std::vector<std::streampos> offsets = calcularOffsets(fichero, nHilos);

        crearBuscadores(ruta, palabra, buscadores, vThreads, offsets);

        for(Buscador &b : buscadores){
            for(ResultadoBusqueda &r : b.vectorBusquedas){
                std::cout << "[Hilo " << b.id << "] :: línea " << r.Linea
                          << " :: ... " << r.Anterior << " "
                          << b.palabra << " "
                          << r.Posterior << " ...\n";
            }
        }
        std::cout << "Hilos: " << buscadores.size() << std::endl;

for(auto &b : buscadores){
    std::cout << "Hilo " << b.id << " resultados: " 
              << b.vectorBusquedas.size() << std::endl;
}
for(auto o : offsets){
    std::cout << o << std::endl;
}

    }
    catch(const std::exception &e){
        std::cerr << "[FATAL ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    

    return 0;
}