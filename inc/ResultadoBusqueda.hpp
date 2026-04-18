#ifndef RESULTADO_BUSQUEDA_HPP
#define RESULTADO_BUSQUEDA_HPP

#include <string>

class ResultadoBusqueda{
    public:
        int Linea;
        std::string Anterior;
        std::string Posterior;

        ResultadoBusqueda(int Linea, std::string Anterior, std::string Posterior);
};

#endif