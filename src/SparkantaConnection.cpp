#include "SparkantaConnection.h"

SparkantaConnection::SparkantaConnection(bool _logToSerail, 
                                         String _ip, 
                                         uint16_t _port, 
                                         String _deviceUniqueName,
                                         uint32_t _incomingMessageInactivityTimeoutInMillis, 
                                         String _identyficationString, 
                                         uint8_t _softwareVersion, 
                                         uint8_t _hardwareVersion,
                                         IndexedByteArray* _incomingByteBuffer,
		                                 IndexedByteArray* _outgoingByteBuffer): TcpConnection(_ip, _port, _logToSerail) {
                                             
    deviceUniqueName = _deviceUniqueName;
    incomingMessageInactivityTimeoutInMillis = _incomingMessageInactivityTimeoutInMillis;
    identyficationString = _identyficationString;
    softwareVersion = _softwareVersion;
    hardwareVersion = _hardwareVersion;
    
    incomingByteBuffer = _incomingByteBuffer;
    outgoingByteBuffer = _outgoingByteBuffer;
    
    lastIncomingMessageTimeInMillis = millis();
}

bool SparkantaConnection::isIncomingTrafficInactivityExceeded() {
    if(incomingMessageInactivityTimeoutInMillis > 0 && millis() - lastIncomingMessageTimeInMillis > incomingMessageInactivityTimeoutInMillis) {
        lastIncomingMessageTimeInMillis = millis();    
        return true;
    } else {
        return false;
    }
}

bool SparkantaConnection::connect() {

    if(TcpConnection::connected() && logToSerail) {
        Serial.println("!!!!!!!!!!!!!!!! Look out: you are trying to connect to Sparkanta gateway while TcpClient is saying that you are connected !!!!!!!!!!!!!!!!");
    }
        
    bool connectingResult = TcpConnection::connect();
        
    if(connectingResult) {
            
        lastIncomingMessageTimeInMillis = millis();
            
        outgoingByteBuffer->putBytes(identyficationString);
        outgoingByteBuffer->putByte(softwareVersion);
        outgoingByteBuffer->putByte(hardwareVersion);
        
        outgoingByteBuffer->putByte(deviceUniqueName.length());
        outgoingByteBuffer->putBytes(deviceUniqueName);
    }
        
    return connectingResult;
}

bool SparkantaConnection::readMessage() {
    if(TcpConnection::connected()) {
        int16_t messageLength = peek();
        if(messageLength == -1) {
            return false;
        } else {
            if(available() >= messageLength + 1) {
                read();
    
                incomingByteBuffer->index = 0;
                
                if(logToSerail) {
                    Serial.print("SparkantaConnection.readMessage: reading message (");
                    Serial.print(messageLength);
                    Serial.print(" B) and put to array at index ");
                    Serial.println(incomingByteBuffer->index);
                }
                
                read(&incomingByteBuffer->bytes[incomingByteBuffer->index], messageLength);
                incomingByteBuffer->index += messageLength;
                
                incomingByteBuffer->index = 0;
                
                lastIncomingMessageTimeInMillis = millis();
                return true;   
            } else {
                return false;
            }
        }
    } else {
        return false;
    }
}

void SparkantaConnection::sendMessages() {
    if(outgoingByteBuffer->containsData()) {
        if(TcpConnection::connected()) {
            if(logToSerail) {
                Serial.print("SparkantaConnection.sendMessages: sending ");
                Serial.print(outgoingByteBuffer->index);
                Serial.print(" bytes");
            }
            
            uint16_t wroteBytesCount = write(outgoingByteBuffer->bytes, outgoingByteBuffer->index);
                
            if(logToSerail) {
                Serial.print("; ");
                Serial.print(wroteBytesCount);
                Serial.println(" bytes were send");
            }
            
            if(outgoingByteBuffer->index != wroteBytesCount) {
                if(logToSerail) {
                    Serial.println("!!!!!!!!!!!!!!!! SparkantaConnection.sendMessages: not all bytes were send, resetting Spark Core !!!!!!!!!!!!!!!!");
                    delay(500);
                }
                
                System.reset();
            }
        }
        
        outgoingByteBuffer->index = 0;
    }
}
