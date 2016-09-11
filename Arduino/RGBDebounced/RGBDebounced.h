
void ledDisplay();
void printWifiStatus()
/**
 * A blocking message emitter to server. Will be blocking 
 * main programm until connected to server.
 */
void sendContent(char* s);
/**
 * A blocking update request which ask server for feed back
 * and take in a char* buffer, size of buffer and cache info
 * from the server into buffer.
 */
void getStatus(char* buffer, int size)
void setup();
void changeState(state n);
void loop();
void onDataReceived(SocketIOClient client, char *data);