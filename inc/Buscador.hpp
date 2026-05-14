#ifndef BUSCADOR_HPP
#define BUSCADOR_HPP

#include <string>
#include <vector>
#include <fstream>
#include "ResultadoBusqueda.hpp"

#define PATH_DATA "data/"
/**
 * Archivo: Buscador.hpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Define la clase Buscador, responsable de recorrer un fragmento
 * concreto de un fichero de texto buscando todas las ocurrencias
 * de una palabra dada. Está diseñada para ejecutarse directamente
 * como objeto-función dentro de un std::thread.
 *
 * Cada instancia de Buscador gestiona:
 *  - Un rango de bytes del fichero (inicioPos – finPos) que le ha
 *    sido asignado por la función calcularOffsets.
 *  - La palabra objetivo de la búsqueda.
 *  - Metadatos de identificación (id del cliente, id del hilo,
 *    rango porcentual) que se propagarán a cada ResultadoBusqueda.
 *  - Un vector de ResultadoBusqueda con todas las ocurrencias
 *    halladas en su fragmento.
 *
 * Funciones principales:
 *  - buscar          : Recorre el rango asignado línea a línea y
 *                      registra cada ocurrencia en vectorBusquedas.
 *  - operator()      : Abre el fichero de forma independiente y
 *                      llama a buscar; permite lanzar el objeto
 *                      directamente como hilo.
 *  - calcularOffsets : (estática) Divide un fichero en nHilos
 *                      fragmentos de bytes alineados a saltos de
 *                      línea y devuelve los offsets de inicio de
 *                      cada fragmento.
 *  - calcularLineas  : (estática) A partir de los offsets calcula
 *                      los números de línea absolutos de inicio de
 *                      cada fragmento, necesarios para el informe.
 *
 * Uso:
 *  Buscador b(id, offsetInicio, offsetFin, palabra, idHilo, pctIni, pctFin);
 *  std::thread t(b, rutaFichero);   // lanza buscar() en paralelo
 *  t.join();
 *
 * Dependencias:
 *  - C++11 o superior (std::thread, std::streampos)
 *  - ResultadoBusqueda.hpp
 *
 * Autores: [Jose Matías Ruiz Valero y Héctor García Rubio]
 * Fecha: 2026-05-14
 */
class Buscador
{
public:
    int id;
    std::streampos inicioPos;
    std::streampos finPos;
    std::string palabra;

    int idHilo;
    int inicioPct;
    int finPct;

    std::vector<ResultadoBusqueda> vectorBusquedas;

    Buscador(int id, std::streampos inicioPos, std::streampos finPos, std::string palabra, int idHilo, int inicioPct, int finPct);

    void buscar(std::ifstream *file);
    void operator()(std::string ruta);

    static std::vector<std::streampos> calcularOffsets(std::ifstream &file, int nHilos);
    static std::vector<int> calcularLineas(std::string ruta, const std::vector<std::streampos> &offsets);
};

#endif