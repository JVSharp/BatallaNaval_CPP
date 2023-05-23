#include "Cliente.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

Cliente::Cliente() {
    // Inicializar los miembros del cliente si es necesario
}

Cliente::~Cliente() {
    // Realizar tareas de limpieza si es necesario
}

void Cliente::conectar(const std::string& servidorIP, int puerto) {
    // Crear un socket para el cliente
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error al crear el socket del cliente" << std::endl;
        return;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(puerto);
    if (inet_pton(AF_INET, servidorIP.c_str(), &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Dirección IP inválida" << std::endl;
        close(clientSocket);
        return;
    }

    // Conectar al servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error al conectar al servidor" << std::endl;
        close(clientSocket);
        return;
    }

    std::cout << "Conexión al servidor establecida. Esperando instrucciones..." << std::endl;

    // Leer las posiciones de las embarcaciones y el turno inicial enviados por el servidor
    std::string mensaje = recibirMensaje(clientSocket);
    int filaBarco1, columnaBarco1, filaBarco2, columnaBarco2, turnoInicial;
    sscanf(mensaje.c_str(), "%d %d %d %d %d", &filaBarco1, &columnaBarco1, &filaBarco2, &columnaBarco2, &turnoInicial);

    // Ciclo principal para jugar hasta que se haya hundido un barco
    while (true) {
        // Mostrar el estado del tablero
        mostrarTablero();

        // Turno del cliente
        if (turnoInicial == 0) {
            // Solicitar al cliente que ingrese el disparo
            int filaDisparo, columnaDisparo;
            std::cout << "Ingrese las coordenadas del disparo (fila columna): ";
            std::cin >> filaDisparo >> columnaDisparo;

            // Enviar el disparo al servidor
            std::string disparo = std::to_string(filaDisparo) + " " + std::to_string(columnaDisparo);
            enviarMensaje(clientSocket, disparo);

            // Recibir el resultado del disparo del servidor
            std::string resultado = recibirMensaje(clientSocket);
            std::cout << "Resultado del disparo: " << resultado << std::endl;

            // Verificar si se ha hundido un barco
            if (resultado == "GOLPE") {
                break;  // Salir del ciclo si se ha hundido un barco
            }
        }

        // Turno del servidor
        if (turnoInicial == 1) {
            // Recibir el disparo del servidor
            std::string disparoServidor = recibirMensaje(clientSocket);
            int filaDisparoServidor, columnaDisparoServidor;
            sscanf(disparoServidor.c_str(), "%d %d", &filaDisparoServidor, &columnaDisparoServidor);

            // Mostrar el disparo del servidor
            std::cout << "Disparo del servidor: " << filaDisparoServidor << " " << columnaDisparoServidor << std::endl;

            // Verificar si el disparo del servidor ha dado en un barco
            std::string resultadoServidor;
            if ((filaDisparoServidor == filaBarco1 && columnaDisparoServidor == columnaBarco1) ||
                (filaDisparoServidor == filaBarco2 && columnaDisparoServidor == columnaBarco2)) {
                resultadoServidor = "GOLPE";
            } else {
                resultadoServidor = "AGUA";
            }

            // Enviar el resultado del disparo del servidor al servidor
            enviarMensaje(clientSocket, resultadoServidor);

            // Verificar si se ha hundido un barco
            if (resultadoServidor == "GOLPE") {
                break;  // Salir del ciclo si se ha hundido un barco
            }
        }
    }

    // Cerrar el socket del cliente
    close(clientSocket);
}
