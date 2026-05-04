#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include "Cliente.hpp"

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
        std::vector<Cliente> vClientes;
        std::vector<std::thread> hilosClientes;

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

            vClientes.emplace_back(i + 1, tipo, palabra, 20);

            hilosClientes.emplace_back(std::ref(vClientes.back()));

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        for (auto &t : hilosClientes)
        {
            if (t.joinable())
                t.join();
        }

        std::cout << "Resultados de busqueda: " << std::endl;
        for (const auto &c : vClientes)
        {
            //Imprimir resultados
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[FATAL ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}