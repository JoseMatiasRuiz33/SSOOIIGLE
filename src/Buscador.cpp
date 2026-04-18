#include "Buscador.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

Buscador::Buscador(int id, std::streampos inicioPos, std::streampos finPos, std::string palabra){
    this->id = id;
    this->inicioPos = inicioPos;
    this->finPos = finPos;
    this->palabra = palabra;
}

void Buscador::buscar(std::ifstream *file){
    file->seekg(inicioPos);

    std::string line;
    std::string palabra_anterior;
    std::string palabra_local;
    std::string palabra_siguiente;
    int linea = 0;

    while(file->tellg() < finPos && getline(*file, line)){
        linea++;
        std::stringstream ss(line);
        palabra_anterior = "";

        while(ss >> palabra_local){
            if(palabra_local == palabra){
                ss >> palabra_siguiente;
                vectorBusquedas.emplace_back(linea, palabra_anterior, palabra_siguiente);
            }
            palabra_anterior = palabra_local;
        }
    }
}

void Buscador::operator()(std::string ruta){

    try{
        std::ifstream fichero(ruta);

        if(!fichero){
            throw std::runtime_error("[Buscador] No se pudo abrir el fichero");
        }

        buscar(&fichero);
    }
    catch(const std::exception &e){
        std::cerr << e.what() << std::endl;
    }
}