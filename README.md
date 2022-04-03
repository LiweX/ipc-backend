Sistemas Operativos II - Laboratorio II IPC
### Lihué Leandro Luna - MAT:39129465 - Ingeniería en Compitación - FCEFyN - UNC
# Interprocess Communication

## Desarrollo
Retomando la estructura cliente-servidor del trabajo anterior se optmizó el sistema de paso de mensajes para evitar convenientes al momento de realizar las querys. Hecho esto se le implemento el uso de base de datos con `sqlite3_open()` y la ejecucion de querys con `sqlite3_exec()`.
### Connection Pool
Para resolver el pool de conexiones se utiliza un arreglo de 5 `sqlite3 *` que seran las 5 conexiones simultaneas que maneja el servidor. Para el manejo de concurrencia existe el arreglo `flags` que pone 1 en la base de datos ocupada y un 0 en la desocupada. Dichos arreglos estan en memoria compartida.
### Cliente
El cliente se instancia de la siguiente forma:
```
./client <type> <address> <port> <interface>
ej: ./client B 127.0.0.1 8080 lo
```
En este caso se esta instanciando un cliente de tipo B que va a presentar un prompt para interactuar con el la base de datos del servidor
### Servidor
El servidor se instancia de la siguiente forma:
```
./server <ipv4address> <ipv6address> <port> <interface> <dbname>
ej: ./server 127.0.0.1 ::1 8080 lo test
```
En este caso se esta instanciando un server que esta escuchando en el puerto 8080 en la direccion de loopback usando como base de datos el archivo `test.db` que debe estar en el directorio del server.
El servidor tambien se encarga de logear todos los comandos que los clientes envian y los pedidos de base de datos.
### Tools
Se implementaron dos pequeñas aplicaciones: 
- `createdb` que se encarga de crear una base de datos de ejemplo con el nombre que se le pase como argumento
- `testdb` para testear la integridad de una base de datos. Se diseño para probar los .db que los clientes C solicitan. 