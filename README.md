# BatallaNaval_CPP

Batalla Naval en C++ con sockets

Este proyecto implementa el juego de Batalla Naval en C++ utilizando sockets para permitir la comunicación entre un servidor y múltiples clientes. El servidor actúa como el árbitro del juego, mientras que los clientes juegan contra el servidor intentando hundir los barcos.
Creado para el curso de Redes 2023, utilizando herramientas disponibles en la web.

## Requisitos

- Compilador de C++ compatible
- Biblioteca Winsock2  (Windows)

## Instrucciones de compilación

1. Abre el terminal.
2. Navegar al directorio del proyecto.
3. Compilar el servidor con el siguiente comando:
g++ servidor.cpp -o servidor -lws2_32


4. Compila el cliente ejecutando el siguiente comando:
g++ cliente.cpp -o cliente -lws2_32


## Instrucciones de ejecución

1. Inicia el servidor ejecutando el siguiente comando:
Reemplazar `<puerto>` con el número de puerto en el que deseas que el servidor escuche las conexiones.

2. Inicia el cliente ejecutando el siguiente comando:
cliente <IP del servidor> <puerto del servidor>

Reemplazar `<IP del servidor>` y `<puerto del servidor>` con la dirección IP y el puerto en los que se está ejecutando el servidor.

3. Ingresa las coordenadas de los disparos y recibirás los resultados.
Ejemplo: 
Coordenadas del disparo (x y): 3 4

4. El juego continuará hasta que haya un ganador.

Bugs que solucionar:
Matriz desplegada incorrectamente
Servidor no escucha la conexión correctamente, entrando en un loop desde el terminal de cliente




