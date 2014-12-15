#include "Pong.h"

Pong::Pong(): Message(4) { }

void Pong::send(TCPClient* tcpClient) {
    tcpClient->write(2); // length
    tcpClient->write(messageCode);
    tcpClient->write(1); // serialization version
    
    tcpClient->flush();
}

bool Pong::is(byte messageAsBytes[], uint8_t length) {
    return length == 2 && messageAsBytes[0] == messageCode;
}

Pong::~Pong() { }
