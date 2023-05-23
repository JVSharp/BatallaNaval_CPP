#ifndef CLIENTE_H
#define CLIENTE_H

class Cliente {
public:
    Cliente();
    ~Cliente();

    void conectar();
    void desconectar();
    void enviarMensaje(const char* mensaje);
    void recibirMensaje(char* buffer, int bufferSize);
    // Aquí puedes agregar otras funciones y miembros según sea necesario
};

#endif  // CLIENTE_H