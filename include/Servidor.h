#ifndef SERVIDOR_H
#define SERVIDOR_H

class Servidor {
public:
    Servidor();
    ~Servidor();

    void iniciar();
    void detener();

private:
    int serverSocket;

    void aceptarConexion();
    void manejarConexion(int clientSocket);
    void enviarMensaje(int clientSocket, const char* mensaje);
    void recibirMensaje(int clientSocket, char* buffer, int bufferSize);
    // Aquí puedes agregar otras funciones y miembros según sea necesario
};

#endif  // SERVIDOR_H
