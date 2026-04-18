#ifndef BUSCADOR_HPP
#define BUSCADOR_HPP

#include <string>
#include <vector>
#include <fstream>
#include "ResultadoBusqueda.hpp"

class Buscador{
    public:
        int id;
        std::streampos inicioPos;
        std::streampos finPos;
        std::string palabra;
        std::vector<ResultadoBusqueda> vectorBusquedas;

        Buscador(int id, std::streampos inicioPos, std::streampos finPos, std::string palabra);

        void buscar(std::ifstream *file);
        void operator()(std::string ruta);
};

#endif