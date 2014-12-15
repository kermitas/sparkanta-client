#include "ServerHello.h"

ServerHello::ServerHello(): Message(6) { }

void ServerHello::send(TCPClient* tcpClient) {
    // this message can only come from server side to device
}

bool ServerHello::is(byte messageAsBytes[], uint8_t length) {
    return length == 2 && messageAsBytes[0] == messageCode;
}

ServerHello::~ServerHello() { }
