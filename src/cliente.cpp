#include "Cliente.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

Cliente::Cliente() {
    // Inicializar los miembros del cliente si es necesario
}

Cliente::~Cliente() {
    // Realizar tareas de limpieza si es necesario
}

void Cliente::conectar() {
    // Crear un socket para el cliente
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error al crear el socket del cliente" << std::endl;
        return;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Puerto en el que está escuchando el servidor
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  // Dirección IP del servidor

    // Conectar al servidor
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error al conectar al servidor" << std::endl;
        close(clientSocket);
        return;
    }

    std::cout << "Conectado al servidor" << std::endl;

    // Aquí puedes implementar la lógica para interactuar con el servidor

    // Cerrar el socket del cliente
    close(clientSocket);
}

void Cliente::desconectar() {
    // Implementa la lógica para cerrar la conexión con el servidor, si es necesario
}

void Cliente::enviarMensaje(const char* mensaje) {
    // Implementa la lógica para enviar un mensaje al servidor
}

void Cliente::recibirMensaje(char* buffer, int bufferSize) {
    // Implementa la lógica para recibir un mensaje del servidor y almacenarlo en el búfer
}
