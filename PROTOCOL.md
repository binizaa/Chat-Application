# Protocolo JSON del Chat Server

El servidor utiliza **sockets TCP a bajo nivel** (BSD sockets con kqueue) y se comunica mediante mensajes JSON.

## Puerto del Servidor
- **Puerto**: 8080
- **Host**: localhost

## Protocolo IDENTIFY

### 1. Solicitud de Identificación
El cliente envía:
```json
{
  "type": "IDENTIFY",
  "username": "Kimberly"
}
```

### 2. Respuesta Exitosa
El servidor responde al cliente:
```json
{
  "type": "RESPONSE",
  "operation": "IDENTIFY",
  "result": "SUCCESS",
  "extra": "Kimberly"
}
```

Y además envía un broadcast a **todos los demás clientes** conectados:
```json
{
  "type": "NEW_USER",
  "username": "Kimberly"
}
```

### 3. Respuesta de Error - Usuario Ya Existe
Si el nombre de usuario ya está en uso:
```json
{
  "type": "RESPONSE",
  "operation": "IDENTIFY",
  "result": "USER_ALREADY_EXISTS",
  "extra": "Kimberly"
}
```

### 4. Respuesta de Error - Formato Inválido
Si el nombre de usuario tiene formato inválido (vacío o más de 8 caracteres):
```json
{
  "type": "RESPONSE",
  "operation": "IDENTIFY",
  "result": "INVALID",
  "extra": "username"
}
```

## Cómo Compilar y Ejecutar

### Compilar el Servidor
```bash
cd server/build
make
```

### Ejecutar el Servidor
```bash
cd server/build
./server_app
```

## Cómo Probar el Servidor

### Opción 1: Usar el Script de Prueba Python (Recomendado)
```bash
python3 test_client.py
```

Este script ejecuta pruebas automáticas que verifican:
- ✓ Identificación exitosa
- ✓ Detección de usuarios duplicados
- ✓ Broadcast de NEW_USER a otros clientes

### Opción 2: Usar netcat manualmente
```bash
# Conectarse al servidor
nc localhost 8080

# Enviar el JSON (pegar y presionar Enter)
{"type": "IDENTIFY", "username": "Kimberly"}

# Esperar la respuesta
```

### Opción 3: Usar Python interactivamente
```python
import socket
import json

# Conectar
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('localhost', 8080))

# Enviar IDENTIFY
msg = {"type": "IDENTIFY", "username": "Kimberly"}
sock.send(json.dumps(msg).encode() + b'\n')

# Recibir respuesta
response = sock.recv(4096).decode()
print(response)

sock.close()
```

## Ejemplo de Prueba Completa

### Terminal 1 - Servidor
```bash
cd server/build
./server_app
```

### Terminal 2 - Cliente 1 (Bob)
```bash
nc localhost 8080
{"type": "IDENTIFY", "username": "Bob"}
# Recibe: {"type":"RESPONSE","operation":"IDENTIFY","result":"SUCCESS","extra":"Bob"}
# Espera...
```

### Terminal 3 - Cliente 2 (Alice)
```bash
nc localhost 8080
{"type": "IDENTIFY", "username": "Alice"}
# Recibe: {"type":"RESPONSE","operation":"IDENTIFY","result":"SUCCESS","extra":"Alice"}
```

### En Terminal 2
```
# Bob recibe el broadcast:
{"type":"NEW_USER","username":"Alice"}
```

## Notas Importantes

1. **Sockets a Bajo Nivel**: El servidor NO usa HTTP. Usa sockets TCP raw con kqueue para multiplexación de I/O.

2. **No se puede usar Postman directamente**: Postman está diseñado para HTTP/REST. Para probar este servidor debes usar:
   - `netcat` (nc)
   - Scripts Python/Node.js con sockets
   - El script de prueba incluido: `test_client.py`

3. **Formato del mensaje**: Cada mensaje JSON debe terminar con `\n` (newline).

4. **Límite de username**: Máximo 8 caracteres.

5. **Arquitectura**:
   - Usa `kqueue` (macOS/BSD) para eventos de I/O no bloqueantes
   - Maneja múltiples clientes concurrentemente
   - Cola de mensajes salientes por cliente
   - Protocolo basado en JSON para facilidad de uso
