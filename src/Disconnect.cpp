#include "Disconnect.h"

Disconnect::Disconnect(): Message(2) { }

void Disconnect::send(TCPClient* tcpClient) {
    // this message can only come from server side to device
}

bool Disconnect::is(byte messageAsBytes[], uint8_t length) {
    return length == 3 && messageAsBytes[0] == messageCode;
}

uint8_t Disconnect::getDelayBeforeNextConnectionAttemptInSeconds(byte messageAsBytes[]) {
    return messageAsBytes[2];
}

Disconnect::~Disconnect() { }
