#include "GatewayHello.h"

GatewayHello::GatewayHello(): Message(5) { }

void GatewayHello::send(TCPClient* tcpClient) {
    // this message can only come from server side to device
}

bool GatewayHello::is(byte messageAsBytes[], uint8_t length) {
    return length == 2 && messageAsBytes[0] == messageCode;
}

GatewayHello::~GatewayHello() { }
