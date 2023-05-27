#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <sstream>
#include <random>
#include <ctime>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class Ship {
public:
    Ship(char symbol, int size) : symbol(symbol), size(size) {}

    char getSymbol() const {
        return symbol;
    }

    int getSize() const {
        return size;
    }

private:
    char symbol;
    int size;
};

class Game {
public:
    Game() {
        // Crear el tablero vacío 15x15
        board.resize(15, std::vector<char>(15, '.'));

        // Definir las embarcaciones
        ships = {
            { 'P', 5 },  // Portaaviones
            { 'B', 4 },  // Buque
            { 'S', 3 },  // Submarino
            { 'L', 1 }   // Lancha
        };
    }

    void placeRandomShips() { // Barcos de manera aleatoria.
        std::default_random_engine engine(static_cast<unsigned>(time(nullptr)));
        std::uniform_int_distribution<int> randomX(0, 14);
        std::uniform_int_distribution<int> randomY(0, 14);
        std::uniform_int_distribution<int> randomOrientation(0, 1);

        for (const auto& ship : ships) {
            bool placed = false;
            while (!placed) {
                int startX = randomX(engine);
                int startY = randomY(engine);
                int orientation = randomOrientation(engine);

                if (canPlaceShip(ship, startX, startY, orientation)) {
                    placeShip(ship, startX, startY, orientation);
                    placed = true;
                }
            }
        }
    }

    std::string processShot(const std::string& coords) { // Procesamiento de disparo.
        std::stringstream ss(coords);
        int x, y;
        ss >> x >> y;

        if (isValidShot(x, y)) { // Si es un disparo válido.
            if (isHit(x, y)) {
                board[y][x] = 'X';
                if (isGameOver()) {
                    return "¡Ganaste!";
                } else {
                    return "¡Impacto!";
                }
            } else {
                board[y][x] = 'O';
                return "Agua";
            }
        } else {
            return "Coordenadas inválidas";
        }
    }

    std::string getPlayerBoard() const {   //Obtenemos el tablero del jugador.
        std::stringstream ss;

        for (const auto& row : board) {
            for (const auto& cell : row) {
                ss << cell << ' ';
            }
            ss << '\n';
        }

        return ss.str();
    }

private: // Colocacion de barcos.
    bool canPlaceShip(const Ship& ship, int startX, int startY, int orientation) const {
        int size = ship.getSize();
        if (orientation == 0) {  // Horizontal
            if (startX + size > 15) {
                return false;
            }

            for (int i = startX; i < startX + size; ++i) {
                if (board[startY][i] != '.') {
                    return false;
                }
            }
        } else {  // Vertical
            if (startY + size > 15) {
                return false;
            }

            for (int i = startY; i < startY + size; ++i) {
                if (board[i][startX] != '.') {
                    return false;
                }
            }
        }

        return true;
    }

    void placeShip(const Ship& ship, int startX, int startY, int orientation) {
        int size = ship.getSize();
        if (orientation == 0) {  // Horizontal
            for (int i = startX; i < startX + size; ++i) {
                board[startY][i] = ship.getSymbol();
            }
        } else {  // Vertical
            for (int i = startY; i < startY + size; ++i) {
                board[i][startX] = ship.getSymbol();
            }
        }
    }

    bool isValidShot(int x, int y) const {
        return x >= 0 && x < 15 && y >= 0 && y < 15;
    }

    bool isHit(int x, int y) const {
        return board[y][x] != '.';
    }

    bool isGameOver() const {
        for (const auto& row : board) {
            for (const auto& cell : row) {
                if (cell != '.' && cell != 'X') {
                    return false;
                }
            }
        }
        return true;
    }

    std::vector<std::vector<char>> board;
    std::vector<Ship> ships;
};

class Server { //Configuración del servidor.
public:
    Server(int port) : port(port) {}

    int run() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Error: No se pudo iniciar winsock\n";
            return 1;
        }

        SOCKET serverSocket = createServerSocket();
        if (serverSocket == INVALID_SOCKET) {
            WSACleanup();
            return 1;
        }

        std::cout << "Esperando conexiones...\n";

        while (true) {
            SOCKET clientSocket = acceptClientConnection(serverSocket);
            if (clientSocket != INVALID_SOCKET) {
                std::thread clientThread([this, clientSocket]() {
                    handleClient(clientSocket);
                });
                clientThread.detach();
            }
        }

        closesocket(serverSocket);
        WSACleanup();

        return 0;
    }

private:
    SOCKET createServerSocket() {
        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Error: Error al crear el server socket\n";
            return INVALID_SOCKET;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Error: Error al enlazar server socket\n";
            closesocket(serverSocket);
            return INVALID_SOCKET;
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Error: Error al escuchar server socket\n";
            closesocket(serverSocket);
            return INVALID_SOCKET;
        }

        return serverSocket;
    }

    SOCKET acceptClientConnection(SOCKET serverSocket) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error: Error al escuchar la conexión del cliente\n";
            return INVALID_SOCKET;
        }

        std::cout << "Nuevo cliente conectado [IP:Puerto]: " << getClientAddress(clientSocket) << '\n';
        return clientSocket;
    }

    std::string getClientAddress(SOCKET clientSocket) const {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        getpeername(clientSocket, (struct sockaddr*)&clientAddr, &addrLen);

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

        return std::string(clientIP) + ":" + std::to_string(ntohs(clientAddr.sin_port));
    }

    void handleClient(SOCKET clientSocket) {
        Game game;
        game.placeRandomShips();

        // Enviar tablero inicial al cliente
        std::string initialBoard = game.getPlayerBoard();
        send(clientSocket, initialBoard.c_str(), initialBoard.size(), 0);

        // Recibir posición de las embarcaciones del cliente
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        std::string shipPositions(buffer);

        // Mostrar información de las embarcaciones del cliente
        std::cout << "Embarcaciones del cliente [IP:Puerto]: " << getClientAddress(clientSocket) << '\n';
        std::cout << shipPositions << '\n';

        // Determinar quién comienza el juego
        std::string startingPlayer = (rand() % 2 == 0) ? "Cliente" : "Servidor";
        std::cout << "Comienza el juego [" << getClientAddress(clientSocket) << "]: " << startingPlayer << '\n';

        while (true) {
            // Turno del cliente
            if (startingPlayer == "Cliente") {
                std::cout << "Disparar [Cliente]: leer coordenadas, enviar al servidor y mostrar tableros\n";
                std::cout << game.getPlayerBoard() << '\n';

                std::string coords;
                std::cout << "Coordenadas (x y): ";
                std::getline(std::cin, coords);

                send(clientSocket, coords.c_str(), coords.size(), 0);

                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                std::string result(buffer);

                std::cout << "Resultado: " << result << '\n';
                std::cout << "Tablero del servidor:\n" << game.getPlayerBoard() << '\n';

                if (result == "¡Ganaste!") {
                    std::cout << "¡Felicidades! ¡Has ganado el juego!\n";
                    break;
                }
            }

            // Turno del servidor
            if (startingPlayer == "Servidor") {
                std::cout << "Disparar [Servidor]: esperar resultados y mostrar tableros\n";
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                std::string coords(buffer);

                std::string result = game.processShot(coords);
                std::cout << "Resultado: " << result << '\n';
                std::cout << "Tablero del cliente:\n" << game.getPlayerBoard() << '\n';

                if (result == "¡Ganaste!") {
                    std::cout << "¡El servidor ha ganado el juego!\n";
                    break;
                }

                std::cout << "Coordenadas del disparo (x y): ";
                std::getline(std::cin, coords);
                send(clientSocket, coords.c_str(), coords.size(), 0);
            }
        }

        std::cout << "Juego finalizado [" << getClientAddress(clientSocket) << "]\n";
        closesocket(clientSocket);
    }

    int port;
};

int main() {
    int port = 12345; //Puerto que asignaremos al servidor.

    Server server(port);
    return server.run();
}
