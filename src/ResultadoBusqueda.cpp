#include "ResultadoBusqueda.hpp"
/**
 * Archivo: ResultadoBusqueda.cpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Implementación de la clase ResultadoBusqueda.
 * Esta clase es un simple contenedor de datos (value object) que
 * almacena la información de una ocurrencia concreta de la palabra
 * buscada: número de línea, contexto inmediato (palabras anterior
 * y posterior) e identificación del hilo que la encontró con su
 * rango porcentual en el fichero.
 * Su única lógica es el constructor de inicialización.
 *
 * Funciones implementadas:
 *  - ResultadoBusqueda (constructor)
 *
 * Autores: [Jose Matías Ruiz Valero y Héctor García Rubio]
 * Fecha: 2026-05-14
 */
ResultadoBusqueda::ResultadoBusqueda(int Linea, std::string Anterior, std::string Posterior, int idHilo, int inicio, int fin)
{
    this->Linea = Linea;
    this->Anterior = Anterior;
    this->Posterior = Posterior;
    this->idHilo = idHilo;
    this->inicio = inicio;
    this->fin = fin;
}