#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // Crear un socket para escuchar conexiones entrantes
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error al crear el socket del servidor" << std::endl;
        return 1;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Puerto en el que escuchará el servidor
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Vincular el socket a la dirección del servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error al vincular el socket a la dirección del servidor" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 5) == -1) {  // Permite hasta 5 conexiones en espera
        std::cerr << "Error al escuchar conexiones entrantes" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Servidor en línea. Esperando conexiones entrantes..." << std::endl;

    // Aceptar conexiones entrantes
    while (true) {
        struct sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            std::cerr << "Error al aceptar la conexión entrante" << std::endl;
            close(serverSocket);
            return 1;
        }

        // Manejar la conexión entrante
        std::cout << "Conexión aceptada. Cliente conectado." << std::endl;

        // Aquí puedes implementar la lógica para interactuar con el cliente

        close(clientSocket);  // Cerrar el socket del cliente
    }

    // Cerrar el socket del servidor
    close(serverSocket);
    return 0;
}
