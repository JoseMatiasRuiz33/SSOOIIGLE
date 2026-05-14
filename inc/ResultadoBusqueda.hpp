#ifndef RESULTADO_BUSQUEDA_HPP
#define RESULTADO_BUSQUEDA_HPP

/**
 * Archivo: ResultadoBusqueda.hpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Define la clase ResultadoBusqueda, que encapsula la información
 * relativa a una ocurrencia concreta de la palabra buscada dentro
 * de un fichero de texto.
 *
 * Cada instancia almacena:
 *  - La línea en la que aparece la palabra (relativa al bloque procesado).
 *  - La palabra inmediatamente anterior y la inmediatamente posterior
 *    a la ocurrencia encontrada (contexto mínimo de la búsqueda).
 *  - El identificador del hilo que localizó la ocurrencia, así como
 *    el rango porcentual del fichero que dicho hilo tenía asignado
 *    (inicio y fin en tanto por ciento), útil para el informe final.
 *
 * Uso:
 *  Los objetos ResultadoBusqueda son creados por la clase Buscador
 *  y acumulados en su vector interno vectorBusquedas. Posteriormente
 *  son leídos por Cliente para mostrar el informe de resultados.
 *
 * Dependencias:
 *  - <string>
 *
 * Autores: [Jose Matías Ruiz Valero y Héctor García Rubio]
 * Fecha: 2026-05-14
 */
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