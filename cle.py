import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('localhost', 10000)
print('connecting to port %s', server_address)
sock.connect(server_address)
try:
    
    # Send data
#     message = 'This is the message.  It will be repeated.'
#     print('sending "%s"', message)
#     sock.sendall(message)
    id = 30
    id_sent = str(id)
    sock.sendall(id_sent)

    # Look for the response
#     amount_received = 0
#     amount_expected = len(message)
    
#     while amount_received < amount_expected:
    data = sock.recv(1000)
#     amount_received += len(data)
    if data:
        print('received "%s"', data)
        a = data.split(',')
        time = a[0]
        status = a[1]
        id = a[2]
        print(time)
        print(status)
        print(id)

finally:
    print('closing connection')
    sock.close()