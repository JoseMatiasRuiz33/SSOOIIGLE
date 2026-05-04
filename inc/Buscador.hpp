#ifndef BUSCADOR_HPP
#define BUSCADOR_HPP

#include <string>
#include <vector>
#include <fstream>
#include "ResultadoBusqueda.hpp"

class Buscador
{
public:
    int id;
    std::streampos inicioPos;
    std::streampos finPos;
    std::string palabra;
    std::vector<ResultadoBusqueda> vectorBusquedas;

    Buscador(int id, std::streampos inicioPos, std::streampos finPos, std::string palabra);

    void buscar(std::ifstream *file);
    void operator()(std::string ruta);

    static std::vector<std::streampos> calcularOffsets(std::ifstream &file, int nHilos);
    static std::vector<int> calcularLineas(std::string ruta, const std::vector<std::streampos> &offsets);
};

#endif