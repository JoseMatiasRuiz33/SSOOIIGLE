#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <mutex>
#include <condition_variable>

#define PATH_DATA "data/"

/**
 * Archivo: SSOOIIGLE.hpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Define la clase Buscador y la clase ResultadoBusqueda, utilizadas para
 * realizar búsquedas de palabras en ficheros de texto de manera concurrente.
 *
 * Clase ResultadoBusqueda:
 *  - Representa un resultado de búsqueda, incluyendo la línea encontrada
 *    y las palabras anterior y posterior a la palabra buscada.
 *
 * Clase Buscador:
 *  - Permite realizar la búsqueda de una palabra en un rango de líneas
 *    específico del fichero.
 *  - Maneja la sincronización de impresión de resultados entre hilos
 *    mediante mutex y condition_variable.
 *  - Sobrecarga del operador () para poder ejecutarse directamente como hilo.
 *
 * Constantes:
 *  - PATH_DATA: Directorio base donde se encuentran los ficheros a procesar
 *
 * Autor: [Jose Matías Ruiz Valero]
 * Fecha: 2026-03-16
 */


class Usuario{

    public:
        int id;
        int saldo;
        enum Categoria{Gratuito, PremiumLimitado,Premium};
    
};

