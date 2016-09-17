'''
    Simple socket server using threads
'''
 
import socket
import sys
from thread import *
try:
    from http_parser.parser import HttpParser
except ImportError:
    from http_parser.pyparser import HttpParser
import urlparse

coursedict = {15112: "Fundamentals of Programming",
            18100: "Introduction to Electrical and Computer Engineering",
            21259: "Calculus in Three Dimensions",
            76101: "Interpretation and Argument",
            21120: "Differential and Integral Calculus",
            21122: "Integration and Approximation",
            33141: "Physics I for Engineering Students"}

userdict = dict()         
def addUser(andrewid,username):
    userdict[andrewid] = [username]
    
def getuserInfo(andrewid):
    if andrewid in userdict:
        return (userdict[andrewid])
    else:
        return "No user found!"

def getcourseList(andrewid):
    if len(getuserInfo(andrewid)) == 1:
        return None
    return getuserInfo(andrewid)[1]

def studentRole(andrewid, coursecode):
    for x in userdict[andrewid][1]:
        if x[0] == coursecode:
            return x[-1]
def addCourse(andrewid, coursecode,role):
    if len(userdict[andrewid]) == 1:
        userdict[andrewid].append([(coursecode, coursedict[coursecode],role)])
    else:
        userdict[andrewid][1].append((coursecode, coursedict[coursecode],role))
    return (userdict[andrewid])
    
def removeCourse(andrewid, coursecode, role):
    userdict[andrewid][1].remove((coursecode, coursedict[coursecode],role))
    return (userdict[andrewid])

duedict = dict()
def addDue(andrewid, coursecode, dueDate, homeworkName, handinType):
    if not coursecode in duedict:
        duedict[coursecode] =[(dueDate, homeworkName, handinType)]
    else:
        duedict[coursecode].append((dueDate, homeworkName, handinType))

def getDueDate(coursecode):
    if coursecode in duedict:
        return duedict[coursecode]
    else:
        return "No Course Found"

def removeDueDate(coursecode, dueDate, homeworkName, handinType):
    duedict[coursecode].remove((dueDate, homeworkName, handinType))

# This is the server part
 
HOST = ''   # Symbolic name meaning all available interfaces
PORT = 8000 # Arbitrary non-privileged port
 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print ('Socket created')
 
#Bind socket to local host and port
try:
    s.bind((socket.gethostname(), PORT))
except socket.error as msg:
    print ('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
    sys.exit()
     
print ('Socket bind complete')
 
#Start listening on socket
s.listen(10)
print ('Socket now listening')

#Function for handling connections. This will be used to create threads
def processrequest(requestdict):
    if requestdict["fun"][0] == "adduser":
        if requestdict["andrewid"][0] not in userdict:
            addUser(requestdict["andrewid"][0],requestdict["username"][0])
            return "User added"
        else:
            return "User already exists"
    elif requestdict["fun"][0] == "addcourse":
        if len(getuserInfo(requestdict["andrewid"][0])) == 1:
            addCourse(requestdict["andrewid"][0],int(requestdict["coursecode"][0]),requestdict["role"][0])
            return "Course added %s" % requestdict["coursecode"][0]
        else:
            for coursetuple in getuserInfo(requestdict["andrewid"][0])[1]:
                if coursetuple[0] == int(requestdict["coursecode"][0]):
                    return "Course already exisits"
            addCourse(requestdict["andrewid"][0],int(requestdict["coursecode"][0]),requestdict["role"][0])
            return "Course added %s" % requestdict["coursecode"][0]
    elif requestdict["fun"][0] == "getcourselist":
        tuplelist = getcourseList(requestdict["andrewid"][0])
        if tuplelist == None:
            return "No course found"
        else:
            coursestring = ""
            for coursetuple in tuplelist:
                coursestring += str(coursetuple[0])
                coursestring += "," 
            return coursestring[:-1]
    elif requestdict["fun"][0] == "adddue":
        if studentRole(requestdict["andrewid"][0],int(requestdict["coursecode"][0])) == "Instructor":
            addDue(requestdict["andrewid"][0],int(requestdict["coursecode"][0]),
                requestdict["duedate"][0] ,requestdict["hwname"][0],requestdict["handintype"][0])
            return ("Due added on %s at %s" % (requestdict["coursecode"][0], requestdict["duedate"][0]))
        else:
            return "Permission Denied"
    elif requestdict["fun"][0] == "getonedue":
        duelist = getDueDate(int(requestdict["coursecode"][0]))
        string = ""
        string += coursedict[int(requestdict["coursecode"][0])]
        string += ","
        if len(duelist) != 0:
            for duetuple in duelist:
                string += duetuple[0]
                string += "-"
                string += duetuple[1]
                string += "-"
                string += duetuple[2]
                string += ","
        return string[:-1]




def clientthread(conn):
    #Sending message to connected client
    # conn.send('Welcome to the server. Type something and hit enter\n') #send only takes string
     
    #infinite loop so that function do not terminate and thread do not end.
    while True:
         
        #Receiving from client
        data = conn.recv(1024)
        if not data: 
            break
        parsed = urlparse.urlparse(data.split(" ")[1])
        requestdict = urlparse.parse_qs(parsed.query)
        print(requestdict)
        if requestdict["fun"][0] == "adduser":
            andrewID = requestdict["andrewid"][0]
        msg = processrequest(requestdict)

        response_headers = {
            'Content-Type': 'text',
            'Content-Length': len(msg.encode(encoding="utf-8")),
            'Connection': 'close',
        }

        response_headers_raw = ''.join('%s: %s\n' % (k, v) for k, v in response_headers.items())

        response_proto = 'HTTP/1.1'
        response_status = '200'
        response_status_text = 'OK' # this can be random

        # sending all this stuff
        r = '%s %s %s' % (response_proto, response_status, response_status_text)
        conn.send(r.encode(encoding="utf-8"))
        conn.send(response_headers_raw.encode(encoding="utf-8"))
        conn.send('\n'.encode(encoding="utf-8")) # to separate headers from body
        conn.send(msg.encode(encoding="utf-8"))

        conn.close()
     
        # conn.sendall(reply)
     
    #came out of loop
    # conn.close()
 
#now keep talking with the client
while 1:
    #wait to accept a connection - blocking call
    conn, addr = s.accept()
    print 'Connected with ' + addr[0] + ':' + str(addr[1])
     
    #start new thread takes 1st argument as a function name to be run, second is the tuple of arguments to the function.
    start_new_thread(clientthread ,(conn,))
 
s.close()

    
    
# addUser("xingshew","Xingsheng Wang")
# print(getuserInfo("xingshew"))
# addCourse("xingshew",15112,"student")
# print(getuserInfo("xingshew"))
# addCourse("xingshew",21259,"student")
# print(getuserInfo("xingshew"))
# addDue(15112, "Sept.23rd","Written1", "recitation")
# addDue(15112, "Sept.24rd","Written2", "recitation")
# removeCourse("xingshew",21259,"student")
# print(getuserInfo("xingshew"))
# print(getDueDate(15112))
# removeDueDate(15112, "Sept.23rd","Written1", "recitation")
# print(getDueDate(15112))
# print(studentRole("xingshew",15112))

     
