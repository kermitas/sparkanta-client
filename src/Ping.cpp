#include "Ping.h"

Ping::Ping(): Message(3) { }

void Ping::send(TCPClient* tcpClient) {
    tcpClient->write(2); // length
    tcpClient->write(messageCode);
    tcpClient->write(1); // serialization version
    
    tcpClient->flush();
}

bool Ping::is(byte messageAsBytes[], uint8_t length) {
    return length == 2 && messageAsBytes[0] == messageCode;
}

Ping::~Ping() { }
