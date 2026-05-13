#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include "Cliente.hpp"
#include "Monitor.hpp"
#include "SistemaPago.hpp"

#define PATH_DATA "data/"
#define DICT_FILE "data/diccionario.txt"

void parse_argv(int *nClientes, int *nReplicas, int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            throw std::invalid_argument("Uso: ./SSOOIIGLE <num_clientes> <n_replicas>");
        }

        *nClientes = std::stoi(argv[1]);
        *nReplicas = std::stoi(argv[2]);

        if (*nClientes <= 0 || *nReplicas <= 0)
        {
            throw std::invalid_argument("Los parámetros deben ser números enteros positivos.");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Error Argumentos] " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::vector<std::string> cargarDiccionario()
{
    std::vector<std::string> diccionario;
    std::ifstream f(DICT_FILE);
    if (!f.is_open())
    {
        throw std::runtime_error("No se puede abrir el archivo diccionario.txt");
    }
    std::string palabra;
    while (f >> palabra)
    {
        diccionario.push_back(palabra);
    }

    if (diccionario.empty())
    {
        throw std::runtime_error("El archivo diccionario.txt está vacío.");
    }

    return diccionario;
}

int main(int argc, char *argv[])
{
    int numClientes, nReplicas;
    srand(time(NULL));

    try
    {

        parse_argv(&numClientes, &nReplicas, argc, argv);
        std::vector<std::string> diccionario = cargarDiccionario();

        SistemaPago sp;
        std::thread hiloSP(std::ref(sp));

        Monitor monitorPrincipal(nReplicas);

        std::vector<Cliente *> vClientes;
        std::vector<std::thread> hilosClientes;

        std::cout << "--- Iniciando Simulación SSOOIIGLE ---" << std::endl;

        for (int i = 0; i < numClientes; i++)
        {

            std::string palabra = diccionario[rand() % diccionario.size()];

            TipoUsuario tipo;
            int prob = rand() % 100;
            if (prob < 20)
                tipo = TipoUsuario::PremiumIlimitado;
            else if (prob < 60)
                tipo = TipoUsuario::Premium;
            else
                tipo = TipoUsuario::Gratuito;

            vClientes.push_back(new Cliente(i + 1, tipo, palabra, &monitorPrincipal, &sp, 20));

            hilosClientes.emplace_back(std::ref(*vClientes.back()));

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        for (auto &t : hilosClientes)
        {
            if (t.joinable())
                t.join();
        }

        sp.parar();
        if (hiloSP.joinable())
            hiloSP.join();

        std::cout << "\n==========================================" << std::endl;
        std::cout << "       INFORME FINAL DE BÚSQUEDAS" << std::endl;
        std::cout << "==========================================" << std::endl;

        for (auto c : vClientes)
        {
            std::string sTipo;
            if (c->tipo == TipoUsuario::Gratuito)
                sTipo = "Gratuito";
            else if (c->tipo == TipoUsuario::Premium)
                sTipo = "Premium";
            else
                sTipo = "Premium Ilimitado";

            std::cout << "\n[Cliente " << c->id << "] (" << sTipo << ") Palabra: '" << c->palabraBusqueda << "'" << std::endl;
            std::cout << "   Tiempo de búsqueda: " << c->tiempoBusqueda << "s | Resultados: " << c->resultados.size() << std::endl;

            for (const auto &r : c->resultados)
            {

                std::cout << "[Hilo " << r.idHilo << " inicio:" << r.inicio
                          << " – final: " << r.fin << "] :: línea " << r.Linea
                          << " :: … " << r.Anterior << " [" << c->palabraBusqueda
                          << "] " << r.Posterior << " …" << std::endl;
            }

            delete c;
        }

        std::cout << "\nSimulación finalizada correctamente." << std::endl;
    }
    catch (const std::exception &e)
    {

        std::cerr << "[FATAL ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}