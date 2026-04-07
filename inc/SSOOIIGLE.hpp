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

int turno=1; /* Para ordenar los hilos al imprimir*/
std::mutex mtx;
std::condition_variable turno_imprimir;

class ResultadoBusqueda{
    public:
        int Linea;
        std::string Anterior;
        std::string Posterior;
        ResultadoBusqueda(int Linea, std::string Anterior, std::string Posterior){
            this->Linea=Linea;
            this->Anterior=Anterior;
            this->Posterior=Posterior;
        }
};

class Buscador{
    public:
        int id;
        int inicioBusc;
        int finalBusc;
        std::string palabra;
        std::vector<ResultadoBusqueda> vectorBusquedas;
        
        Buscador(int id, int inicioBusc, int finalBusc, std::string palabra){
            this->id=id;
            this->inicioBusc=inicioBusc;
            this->finalBusc=finalBusc;
            this->palabra=palabra;
        }
        void imprimirBusqueda(){
            std::unique_lock<std::mutex> ul(mtx);
            turno_imprimir.wait(ul, [this]{return turno==id;});{
                for(ResultadoBusqueda &r : vectorBusquedas){
                    std::cout << "[Hilo " << id << " inicio:" << inicioBusc << " - final: " << finalBusc << "] :: línea " << r.Linea
                    << " :: ... " << r.Anterior << " " << palabra << " " << r.Posterior << " ..."<< std::endl;  
                }
            turno++;
            }
            turno_imprimir.notify_all();
        } 

        void buscar(std::ifstream *file){
            std::string line;
            std::string palabra_anterior="";
            std::string palabra_local;
            std::string palabra_siguiente;
            int i=0;
            while(getline(*file,line)){
                ++i;
                std::stringstream ss(line);
                if(i >= inicioBusc && i <= finalBusc){
                    while(ss >> palabra_local){
                        if(palabra_local==palabra){
                            ss >> palabra_siguiente;
                            vectorBusquedas.emplace_back(i,palabra_anterior,palabra_siguiente);
                        }
                        palabra_anterior=palabra_local;
                    }
                }
            }
            imprimirBusqueda();
        }

        void operator () (std::string ruta){
            std::ifstream fichero(ruta);
            if(!fichero){
                std::cerr << "[SSOOIIGLE] Error al abrir la ruta \"" << ruta << "\". No se ha podido abrir el fichero correctamente. \n";
                exit(EXIT_FAILURE);
            }
            buscar(&fichero);
        }
};

