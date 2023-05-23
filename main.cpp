#include <iostream>
#include <vector>

// Tamaño del tablero
const int FILAS = 10;
const int COLUMNAS = 10;

// Símbolos para representar los estados del tablero
const char VACIO = '-';
const char BARCO = 'B';
const char GOLPE = 'X';
const char AGUA = 'O';

// Clase que representa el juego Batalla Naval
class BatallaNaval {
public:
    BatallaNaval() {
        // Inicializar el tablero vacío
        tablero.resize(FILAS, std::vector<char>(COLUMNAS, VACIO));
    }

    void colocarBarco(int fila, int columna) {
        // Verificar que la posición sea válida
        if (fila < 0 || fila >= FILAS || columna < 0 || columna >= COLUMNAS) {
            std::cout << "Posición inválida." << std::endl;
            return;
        }

        // Colocar el barco en la posición especificada
        tablero[fila][columna] = BARCO;
    }

    void realizarAtaque(int fila, int columna) {
        // Verificar que la posición sea válida
        if (fila < 0 || fila >= FILAS || columna < 0 || columna >= COLUMNAS) {
            std::cout << "Posición inválida." << std::endl;
            return;
        }

        // Verificar el estado de la posición en el tablero
        char estado = tablero[fila][columna];
        if (estado == BARCO) {
            std::cout << "¡GOLPE! Has hundido un barco." << std::endl;
            tablero[fila][columna] = GOLPE;
        } else if (estado == VACIO) {
            std::cout << "¡AGUA! No has dado en el blanco." << std::endl;
            tablero[fila][columna] = AGUA;
        } else {
            std::cout << "Ya has atacado esta posición anteriormente." << std::endl;
        }
    }

    void mostrarTablero() {
        for (int i = 0; i < FILAS; i++) {
            for (int j = 0; j < COLUMNAS; j++) {
                std::cout << tablero[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::vector<char>> tablero;
};

int main() {
    BatallaNaval juego;

    // Ejemplo de colocación de barcos
    juego.colocarBarco(3, 4);
    juego.colocarBarco(6, 2);
    juego.colocarBarco(8, 9);

    // Ejemplo de ataques
    juego.realizarAtaque(3, 4);  // Debe mostrar "¡GOLPE! Has hundido un barco."
    juego.realizarAtaque(2, 7);  // Debe mostrar "¡AGUA! No has dado en el blanco."
    juego.realizarAtaque(8, 9);  // Debe mostrar "¡GOLPE! Has hundido un barco."
    juego.realizarAtaque(8, 9);  // Debe mostrar "Ya has atacado esta posición anteriormente."

    // Mostrar el estado del tablero
    juego.mostrarTablero();

    return 0;
}
