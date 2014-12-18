#include "SparkantaConnection.h"

SparkantaConnection::SparkantaConnection(String _ip, uint16_t _port, uint8_t _maxConnectionRetries, uint16_t _delayBetweenConnestionReattemptsInMillis, uint32_t _incomingMessageInactivityTimeoutInMillis, String _identyficationString, uint8_t _softwareVersion, uint8_t _hardwareVersion): TcpConnection(_ip, _port, _maxConnectionRetries, _delayBetweenConnestionReattemptsInMillis) {
    identyficationString = _identyficationString;
    softwareVersion = _softwareVersion;
    hardwareVersion = _hardwareVersion;
    incomingMessageInactivityTimeoutInMillis = _incomingMessageInactivityTimeoutInMillis;
}

void SparkantaConnection::process() {
    
    if(TcpConnection::connected() && incomingMessageInactivityTimeoutInMillis > 0 && millis() - lastIncomingMessageTimeInMillis > incomingMessageInactivityTimeoutInMillis) TcpConnection::stop();
    
    TcpConnection::process();
}

bool SparkantaConnection::connect(IndexedByteArray* byteArray) {
    if(TcpConnection::connected()) {
        return true;
    } else {
        bool connectingResult = TcpConnection::connect();
        
        if(connectingResult) {
            
            lastIncomingMessageTimeInMillis = millis();
            
            byteArray->putBytes(identyficationString);
            byteArray->putByte(softwareVersion);
            byteArray->putByte(hardwareVersion);

            DeviceHello deviceHello;
            deviceHello.serialize(byteArray);
        }
        
        return connectingResult;
    }
}

bool SparkantaConnection::readMessage(IndexedByteArray* byteArray) {
    int16_t messageLength = peek();
    if(messageLength == -1) {
        return false;
    } else {
        if(available() >= messageLength + 1) {
            read();

            /*
            // TODO to remove ---------------------------------
            Serial.print("reading message (");
            Serial.print(messageLength);
            Serial.print(" B) and put to array at index ");
            Serial.println(byteArray->index);
            // TODO to remove ---------------------------------
            */
            
            read(&byteArray->bytes[byteArray->index], messageLength);
            byteArray->index += messageLength;
            
            lastIncomingMessageTimeInMillis = millis();
            return true;   
        } else {
            return false;
        }
    }
}

SparkantaConnection::~SparkantaConnection() { }
