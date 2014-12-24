#include "TcpConnection.h"

TcpConnection::TcpConnection(String _ip, uint16_t _port, bool _logToSerail): TCPClient() {    
    ipArrayFromString(ip, _ip);
    port = _port;
    logToSerail = _logToSerail;        
}

void TcpConnection::ipArrayFromString(uint8_t ipAsByteArray[], String ip) {
  int dot1 = ip.indexOf('.');
  ipAsByteArray[0] = ip.substring(0, dot1).toInt();
  int dot2 = ip.indexOf('.', dot1 + 1);
  ipAsByteArray[1] = ip.substring(dot1 + 1, dot2).toInt();
  dot1 = ip.indexOf('.', dot2 + 1);
  ipAsByteArray[2] = ip.substring(dot2 + 1, dot1).toInt();
  ipAsByteArray[3] = ip.substring(dot1 + 1).toInt();
}

bool TcpConnection::connect() {
    return TCPClient::connect(ip, port);
}

void TcpConnection::stop() {
    if(TCPClient::connected()) {
        
        if(logToSerail && available() > 0) {
            Serial.print("TcpConnection.stop: before closing connection read out all ");
            Serial.print(available());
            Serial.print(" available bytes and perform flush...");
        }
    
        uint16_t i = 0;
        int readByte = 0;
        while(available() > 0 && readByte >= 0 && i < 1024 * 8) {
            readByte = read();
            i++;
        }
        
        flush();
        
        if(logToSerail) Serial.println("closing connection right now");
    }
    
    TCPClient::stop();
}
