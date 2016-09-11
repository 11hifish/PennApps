import socket
import sys
import datetime
import threading

# status:
status_unsent=0
status_sent=1
ststus_receivedResponse=2
ststus_forwardedResponse=3

ArduinoClient=0
NormalClient=1

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Bind the socket to the port
server_address = ('localhost', 8080)
print('starting up on %s port %s', server_address[0],server_address[1])
sock.bind(server_address)
sock.listen(1)
q = []
#q.append(("2016/1/1/10:00pm", 0, 30))

def query(q, id):
    for (t,i,s) in q:
        if i == id:
            return (t,s,i)

def insertAndAddTime(q, id, status):
    time = datetime.ctime()
    q.insert((time, id, status))
    return time

def change_status_to_sent(q, id, status):
    for (t,i,s) in q:
        if (t,i,s) == (time, id, status) and status == status_unsent:
            q.remove((time, id, status))
            q.insert((time, id, status_sent))
def change_status_to_received(q, id, status):
    for (t,i,s) in q:
        if (t,i,s) == (time, id, status) and status == status_sent:
            q.remove((time, id, status))
            q.insert((time, id, status_receivedResponse))

while True:
    # Wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()
    try:
        print ('connection from', client_address)

        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(1000)
            print ('received information: %data')
            if data:
                s = data.split("_")
                if len(s) != 3:
                    print("Parse Failure: +")
                    print(s)
                    continue
                type = s[0]
                id = int(s[1])
                status = int(s[2])
                if type == "Pressed":
                    time = insertAndAddTime(q, id, status)
                    print('sending data back to the client')
                    connection.sendall(time+"_"+str(id)+"_"+str(status))
                    change_status_to_sent(q, id, status)
#                if type == "Client":
#                    change_status_to_received(q, id, status)

            else:
                print('no more data from', client_address)
                break
            
    finally:
        # Clean up the connection
        connection.close()
