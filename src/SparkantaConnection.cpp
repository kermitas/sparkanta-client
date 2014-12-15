#include "SparkantaConnection.h"
#include "DeviceHello.h"

SparkantaConnection::SparkantaConnection(String _ip, uint16_t _port, byte _maxConnectionRetries, uint16_t _delayBetweenConnestionReattemptsInMillis,  uint16_t _incomingDataInactivityTimeoutInMillis, String _identyficationString, byte _softwareVersion, byte _hardwareVersion): TcpConnection(_ip, _port, _maxConnectionRetries, _delayBetweenConnestionReattemptsInMillis, _incomingDataInactivityTimeoutInMillis) {
    identyficationString = _identyficationString;
    softwareVersion = _softwareVersion;
    hardwareVersion = _hardwareVersion;
}

bool SparkantaConnection::connect() {
    if(TcpConnection::connected()) {
        return true;
    } else {
        bool connectingResult = TcpConnection::connect();
        
        if(connectingResult) {
            sendIdentyficationStringSoftwareAndHardwareVersion(identyficationString, softwareVersion, hardwareVersion);

            DeviceHello deviceHello;
            deviceHello.send(this);
        }
        
        return connectingResult;
    }
}

void SparkantaConnection::sendIdentyficationStringSoftwareAndHardwareVersion(String identyficationString, byte softwareVersion, byte hardwareVersion) {
    write(reinterpret_cast<const uint8_t*>(identyficationString.c_str()), identyficationString.length());
    write(softwareVersion);
    write(hardwareVersion);
    flush();
}

int SparkantaConnection::readMessage(byte bytes[]) {
    int messageLength = peek();
    if(messageLength == -1) {
        return -1;
    } else {
        if(available() >= messageLength + 1) {
            read();
            read(bytes, messageLength);
            lastIncomingDataTime = millis();
            return messageLength;   
        } else {
            return -1;
        }
    }
}

SparkantaConnection::~SparkantaConnection() { }
