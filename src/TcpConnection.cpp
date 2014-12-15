#include "TcpConnection.h"

TcpConnection::TcpConnection(String _ip, uint16_t _port, byte _maxConnectionRetries, uint16_t _delayBetweenConnestionReattemptsInMillis, uint16_t _incomingDataInactivityTimeoutInMillis): TCPClient() {
    ipArrayFromString(ip, _ip);
    port = _port;
    maxConnectionRetries = _maxConnectionRetries;
    connectionRetry = 0;
    delayBetweenConnestionReattemptsInMillis = _delayBetweenConnestionReattemptsInMillis;
    lastIncomingDataTime = millis();
    incomingDataInactivityTimeoutInMillis = _incomingDataInactivityTimeoutInMillis;
}

void TcpConnection::ipArrayFromString(byte ipAsByteArray[], String ip) {
  int dot1 = ip.indexOf('.');
  ipAsByteArray[0] = ip.substring(0, dot1).toInt();
  int dot2 = ip.indexOf('.', dot1 + 1);
  ipAsByteArray[1] = ip.substring(dot1 + 1, dot2).toInt();
  dot1 = ip.indexOf('.', dot2 + 1);
  ipAsByteArray[2] = ip.substring(dot2 + 1, dot1).toInt();
  ipAsByteArray[3] = ip.substring(dot1 + 1).toInt();
}

uint8_t TcpConnection::connected() {
    inactivityCheck();
    return TCPClient::connected();
}

void TcpConnection::stop() {
    while(available() > 0) {
        read();
        flush();
    }
    
    TCPClient::stop();
}

void TcpConnection::inactivityCheck() {
    if(incomingDataInactivityTimeoutInMillis > 0 && millis() - lastIncomingDataTime > incomingDataInactivityTimeoutInMillis) stop();
}

bool TcpConnection::connect() {
    if(connected()) {
        return true;
    } else {
        return connectNow();
    }
}


bool TcpConnection::connectNow() {
    if (TCPClient::connect(ip, port)) {
        connectionRetry = 0;
        lastIncomingDataTime = millis();
        return true;
    } else {
        connectionRetry++;
        
        if(maxConnectionRetries > 0 && connectionRetry >= maxConnectionRetries) System.reset();
        
        if(delayBetweenConnestionReattemptsInMillis > 0) delay(delayBetweenConnestionReattemptsInMillis);
        
        return false;
    } 
}

TcpConnection::~TcpConnection() { }
