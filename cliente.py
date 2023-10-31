import socket

# Cria um objeto socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Conecta-se ao servidor
server_address = ('127.0.0.1', 12345)
client_socket.connect(server_address)

while True:
    mensagem = input('Digite uma mensagem: ')
    client_socket.send(mensagem.encode())
    data = client_socket.recv(1024)
    print(f'Resposta do servidor: {data.decode()}')

client_socket.close()
