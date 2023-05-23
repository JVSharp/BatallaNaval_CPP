#include "Servidor.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <random>

Servidor::Servidor() {
    // Inicializar los miembros del servidor si es necesario
}

Servidor::~Servidor() {
    // Realizar tareas de limpieza si es necesario
}

void Servidor::iniciar() {
    // Crear un socket para el servidor
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error al crear el socket del servidor" << std::endl;
        return;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Puerto en el que escucha el servidor
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // Escucha en todas las interfaces de red

    // Vincular el socket a la dirección del servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error al vincular el socket del servidor" << std::endl;
        close(serverSocket);
        return;
    }

    // Escuchar por conexiones de clientes
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error al escuchar por conexiones de clientes" << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Servidor iniciado. Esperando por conexiones de clientes..." << std::endl;

    // Ciclo principal para aceptar conexiones de clientes
    while (true) {
        // Aceptar una conexión entrante
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            std::cerr << "Error al aceptar una conexión de cliente" << std::endl;
            close(serverSocket);
            return;
        }

        std::cout << "Cliente conectado. Creando hebra para atender al cliente." << std::endl;

        // Crear una hebra para atender al cliente
        std::thread clientThread(&Servidor::atenderCliente, this, clientSocket);
        clientThread.detach();  // Liberar la hebra sin esperar a que termine
    }

    // Cerrar el socket del servidor
    close(serverSocket);
}

void Servidor::atenderCliente(int clientSocket) {
    // Generar las posiciones de las embarcaciones aleatoriamente
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribucion(0, 9);  // Rango de 0 a 9 para las filas y columnas

    int filaBarco1 = distribucion(gen);
    int columnaBarco1 = distribucion(gen);
    int filaBarco2 = distribucion(gen);
    int columnaBarco2 = distribucion(gen);

    // Seleccionar al azar quién comienza el juego
    std::uniform_int_distribution<int> turno(0, 1);  // 0 para el servidor, 1 para el cliente
    int turnoInicial = turno(gen);

    // Enviar al cliente las posiciones de las embarcaciones y el turno inicial
    std::string mensaje = std::to_string(filaBarco1) + " " + std::to_string(columnaBarco1) + " "
                        + std::to_string(filaBarco2) + " " + std::to_string(columnaBarco2) + " "
                        + std::to_string(turnoInicial);
    enviarMensaje(clientSocket, mensaje);

    // Ciclo para jugar hasta que se haya hundido un barco
    while (true) {
        // Leer el disparo del cliente
        std::string disparo = recibirMensaje(clientSocket);

        // Verificar si el disparo ha dado en un barco
        int filaDisparo = std::stoi(disparo.substr(0, 1));
        int columnaDisparo = std::stoi(disparo.substr(2, 1));

        bool golpe = false;
        if ((filaDisparo == filaBarco1 && columnaDisparo == columnaBarco1) ||
            (filaDisparo == filaBarco2 && columnaDisparo == columnaBarco2)) {
            golpe = true;
        }

        // Enviar el resultado del disparo al cliente
        std::string resultado = golpe ? "GOLPE" : "AGUA";
        enviarMensaje(clientSocket, resultado);

        // Verificar si se ha hundido un barco
        if (golpe) {
            break;  // Salir del ciclo si se ha hundido un barco
        }

        // Generar el disparo del servidor al azar
        int filaDisparoServidor = distribucion(gen);
        int columnaDisparoServidor = distribucion(gen);

        // Verificar si el disparo del servidor ha dado en un barco
        bool golpeServidor = false;
        if ((filaDisparoServidor == filaBarco1 && columnaDisparoServidor == columnaBarco1) ||
            (filaDisparoServidor == filaBarco2 && columnaDisparoServidor == columnaBarco2)) {
            golpeServidor = true;
        }

        // Enviar el disparo del servidor al cliente
        std::string disparoServidor = std::to_string(filaDisparoServidor) + " " + std::to_string(columnaDisparoServidor);
        enviarMensaje(clientSocket, disparoServidor);

        // Enviar el resultado del disparo del servidor al cliente
        resultado = golpeServidor ? "GOLPE" : "AGUA";
        enviarMensaje(clientSocket, resultado);

        // Verificar si se ha hundido un barco
        if (golpeServidor) {
            break;  // Salir del ciclo si se ha hundido un barco
        }
    }

    // Cerrar el socket del cliente
    close(clientSocket);
}
