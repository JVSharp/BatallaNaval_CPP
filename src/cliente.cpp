#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class Client {
public:
    Client(const std::string& serverIP, int serverPort) : serverIP(serverIP), serverPort(serverPort) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Error al inicializar Winsock." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    ~Client() {
        WSACleanup();
    }

void connectToServer() {
    // Crear socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket.\n";
        return;
    }

    // Configurar dirección del servidor
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());

    // Conectar al servidor
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al conectar al servidor.\n";
        closesocket(clientSocket);
        return;
    }

    // Resto del código...
}


    void sendShipPositions(const std::string& shipPositions) {
        send(serverSocket, shipPositions.c_str(), shipPositions.size(), 0);
    }

    std::string receiveInitialBoard() {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        recv(serverSocket, buffer, sizeof(buffer) - 1, 0);
        return std::string(buffer);
    }

    std::string receiveResult() {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        recv(serverSocket, buffer, sizeof(buffer) - 1, 0);
        return std::string(buffer);
    }

    void play() {
        connectToServer();

        // Recibir tablero inicial del servidor
        std::string initialBoard = receiveInitialBoard();
        std::cout << "Tablero inicial:\n" << initialBoard << '\n';

        // Generar posiciones aleatorias de embarcaciones
        std::string shipPositions = generateRandomShipPositions();
        std::cout << "Embarcaciones generadas aleatoriamente:\n" << shipPositions << '\n';

        // Enviar posiciones de embarcaciones al servidor
        sendShipPositions(shipPositions);

        // Determinar quién comienza el juego
        std::string startingPlayer = (rand() % 2 == 0) ? "Cliente" : "Servidor";
        std::cout << "Comienza el juego: " << startingPlayer << '\n';

        while (true) {
            // Turno del cliente
            if (startingPlayer == "Cliente") {
                std::string coords;
                std::cout << "Coordenadas del disparo (x y): ";
                std::getline(std::cin, coords);
                send(serverSocket, coords.c_str(), coords.size(), 0);

                std::string result = receiveResult();
                std::cout << "Resultado: " << result << '\n';

                if (result == "¡Ganaste!") {
                    std::cout << "¡Felicidades! Has ganado el juego.\n";
                    break;
                }
            }

            // Turno del servidor
            if (startingPlayer == "Servidor") {
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                recv(serverSocket, buffer, sizeof(buffer) - 1, 0);
                std::string coords(buffer);

                std::cout << "Es el turno del servidor. Dispara en las coordenadas: " << coords << '\n';

                // Simular resultado aleatorio del disparo
                std::string result = (rand() % 2 == 0) ? "¡Agua!" : "¡Impacto!";
                std::cout << "Resultado: " << result << '\n';
                send(serverSocket, result.c_str(), result.size(), 0);

                if (result == "¡Ganaste!") {
                    std::cout << "El servidor ha ganado el juego.\n";
                    break;
                }
            }
        }

        closesocket(serverSocket);
    }

private:
    std::string serverIP;
    int serverPort;
    SOCKET serverSocket;

    std::string generateRandomShipPositions() {
        // Generar posiciones aleatorias de embarcaciones (solo para fines de demostración)
        std::string positions = "";
        for (char ship = 'A'; ship <= 'L'; ship++) {
            int size;
            if (ship == 'A')
                size = 5;
            else if (ship == 'B' || ship == 'C')
                size = 4;
            else if (ship == 'D' || ship == 'E')
                size = 3;
            else
                size = 1;

            int x = rand() % 15;
            int y = rand() % 15;

            for (int i = 0; i < size; i++) {
                positions += std::to_string(x + i) + " " + std::to_string(y) + " " + ship + '\n';
            }
        }
        return positions;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <IP del servidor> <puerto del servidor>\n";
        return 1;
    }

    std::string serverIP = argv[1];
    int serverPort = std::atoi(argv[2]);

    srand(time(nullptr));
    Client client(serverIP, serverPort);
    client.play();

    return 0;
}
