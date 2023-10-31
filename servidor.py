import socket
import threading

# Função para lidar com a conexão de um cliente
def handle_client(client_address, data):
    # Processar os dados e enviar a resposta
    response_data = b"Resposta: " + data
    server_socket.sendto(response_data, client_address)

# Configurações do servidor
bind_ip = "0.0.0.0"  # Ou o IP da interface de rede desejada
bind_port = 12345

# Crie um socket UDP
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Faça o bind do socket
server_socket.bind((bind_ip, bind_port))

print(f"[*] Ouvindo em {bind_ip}:{bind_port}")

while True:
    data, client_address = server_socket.recvfrom(1024)
    print(f"[*] Recebeu dados de {client_address[0]}:{client_address[1]}: {data.decode()}")

    # Inicie uma thread para lidar com o cliente
    client_handler = threading.Thread(target=handle_client, args=(client_address, data))
    client_handler.start()
