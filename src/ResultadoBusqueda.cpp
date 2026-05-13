#include "ResultadoBusqueda.hpp"

ResultadoBusqueda::ResultadoBusqueda(int Linea, std::string Anterior, std::string Posterior, int idHilo, int inicio, int fin)
{
    this->Linea = Linea;
    this->Anterior = Anterior;
    this->Posterior = Posterior;
    this->idHilo = idHilo;
    this->inicio = inicio;
    this->fin = fin;
}