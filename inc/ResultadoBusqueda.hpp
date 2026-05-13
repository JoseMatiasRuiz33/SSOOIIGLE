#ifndef RESULTADO_BUSQUEDA_HPP
#define RESULTADO_BUSQUEDA_HPP

#include <string>

class ResultadoBusqueda
{
public:
    int Linea;
    std::string Anterior;
    std::string Posterior;

    int idHilo;
    int inicio;
    int fin;

    ResultadoBusqueda(int Linea, std::string Anterior, std::string Posterior, int idHilo, int inicio, int fin);
};

#endif