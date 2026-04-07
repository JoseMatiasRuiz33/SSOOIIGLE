#include <iostream>
#include <vector>
#include <thread>
#include <SSOOIIGLE.hpp>
#include <fstream>

/**
 * Archivo: SSOOIIGLE.cpp
 * Proyecto: SSOOIIGLE - Buscador multihilo de palabras en ficheros de texto
 *
 * Descripción:
 * Este programa permite buscar de manera concurrente una palabra específica
 * dentro de un fichero de texto. Se divide el trabajo entre varios hilos,
 * cada uno procesando un bloque de líneas del fichero, y luego imprime
 * los resultados en orden.
 *
 * Funciones principales:
 *  - parse_argv: Valida y procesa los argumentos de entrada (fichero, palabra, número de hilos)
 *  - contarLineas: Cuenta el número de líneas del fichero
 *  - crearBuscadores: Crea los hilos que realizarán la búsqueda concurrente
 *
 * Uso:
 *  ./SSOOIIGLE <nombre_fichero> <palabra> <num_hilos>
 *
 * Dependencias:
 *  - C++11 o superior (para std::thread)
 *  - SSOOIIGLE.hpp (definición de la clase Buscador)
 *
 * Autor: [Jose Matías Ruiz Valero]
 * Fecha: 2026-03-16
 */

void parse_argv(int *nHilos, std::ifstream *fichero, std::string *palabra, std::string *ruta, int argc, char *argv[]){
    
    if(argc!=4){
        std::cerr << "[SSOOIIGLE] Error en el formato de las instrucciones \"./SSOOIIGLE <nombre_fichero> <palabra> <num_hilos>\".\n";
        exit(EXIT_FAILURE);
    }
    
    *ruta=PATH_DATA;
    fichero->open((*ruta+=(*++argv)));
    if(fichero->fail()){
        std::cerr << "[SSOOIIGLE] Error al abrir la ruta \"" << ruta << "\". No se ha podido abrir el fichero correctamente. \n";
        exit(EXIT_FAILURE);
    }

    *palabra=*++argv;
    if((*nHilos=atoi(*++argv))<=0){
        std::cerr << "[SSOOIIGLE] Error en el número de hilos seleccionados \"" << *nHilos << "\", el número debe ser mayor a 0.\n";
        exit(EXIT_FAILURE);
    }

}

int contarLineas(std::ifstream *file){
    std::string line;
    int nLineas=0;
    while(getline(*file,line)){
        nLineas++;
    }
    file->clear();   /* Cambiamos el fichero para que vuelva a apuntar al principio en lugar de al final*/
    return nLineas;
}


void crearBuscadores(std::ifstream *file, int *nLineas, int *nHilos, std::string palabra, std::string ruta, std::vector<std::thread> *vThreads){
    int lineasPorHilo=(*nLineas)/(*nHilos);
    int lineasEmpezar=0;
    int lineasFinal;
     /* Todos los casos menos el último*/
    for(int i=1;i<*nHilos;i++){
        lineasFinal=lineasEmpezar+lineasPorHilo; 
        Buscador buscador(i, lineasEmpezar, lineasFinal-1, palabra);
        vThreads->emplace_back(buscador, ruta);
        lineasEmpezar=lineasFinal;
    }
    /* Último caso (final del archivo)*/
    Buscador buscador(*nHilos, lineasEmpezar, *nLineas, palabra);
    vThreads->emplace_back(buscador, ruta);
    
    for(std::thread &t : *vThreads){
        t.join();
    }
}

int main(int argc, char *argv[]){
    int nHilos, nLineas;
    std::string palabra, ruta;
    std::ifstream fichero;
    std::vector<std::thread> vThreads;

    parse_argv(&nHilos, &fichero, &palabra, &ruta, argc, argv);
    nLineas=contarLineas(&fichero);
    crearBuscadores(&fichero, &nLineas, &nHilos, palabra, ruta, &vThreads);
    
    return 0;
}