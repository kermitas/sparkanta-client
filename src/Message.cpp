// =============================================================

#include "Message.h"

Message::Message(uint8_t _messageCode) {
    messageCode = _messageCode;
}

// =============================================================

OutgoingMessage::OutgoingMessage(uint8_t _messageCode): Message(_messageCode) { }

// =============================================================

IncomingMessage::IncomingMessage(uint8_t _messageCode): Message(_messageCode) { 
    ackType = 0;    
}

// =============================================================

Ping::Ping(): IncomingMessage(Ping::staticMessageCode) { }

// =============================================================

Pong::Pong(): OutgoingMessage(Pong::staticMessageCode) { }

// =============================================================

ServerHello::ServerHello(): IncomingMessage(ServerHello::staticMessageCode) { }

// =============================================================

GatewayHello::GatewayHello(): IncomingMessage(GatewayHello::staticMessageCode) { }

// =============================================================

DeviceHello::DeviceHello(): OutgoingMessage(DeviceHello::staticMessageCode) { }

// =============================================================

Disconnect::Disconnect(): IncomingMessage(Disconnect::staticMessageCode) { 
    delayBeforeNextConnectionAttemptInSeconds = 0;
}

// =============================================================

PinConfiguration::PinConfiguration(): IncomingMessage(PinConfiguration::staticMessageCode) { }

// =============================================================

DigitalPinValue::DigitalPinValue(): OutgoingMessage(DigitalPinValue::staticMessageCode) { 
	pinNumber = 0;
    pinValue = 0;
}

// =============================================================

AnalogPinValue::AnalogPinValue(): OutgoingMessage(AnalogPinValue::staticMessageCode) {
    pinNumber = 0;
    pinValue = 0;
}

// =============================================================

SetDigitalPinValue::SetDigitalPinValue(): IncomingMessage(SetDigitalPinValue::staticMessageCode) { 
	pinNumber = 0;
    pinValue = 0;
}

// =============================================================

SetAnalogPinValue::SetAnalogPinValue(): IncomingMessage(SetAnalogPinValue::staticMessageCode) {
    pinNumber = 0;
    pinValue = 0;
}

// =============================================================

Ack::Ack(): OutgoingMessage(Ack::staticMessageCode) {
    ackMessageCode = 0;
    ackType = 0;
}

// =============================================================

Serializers::Serializers(bool _logToSerail, IndexedByteArray* _byteArray) {
    logToSerail = _logToSerail;
    byteArray = _byteArray;
    messageNumber = 0;
}

void Serializers::writeHeader(uint8_t length, uint8_t messageCode) {
    byteArray->putByte(3 + length);
    byteArray->putByte(messageCode);
    byteArray->putByte(serializationVersion);
    byteArray->putByte(messageNumber);
    messageNumber++;
}

bool Serializers::serialize(OutgoingMessage* message) {
    switch(message->messageCode) {
        
        case(Pong::staticMessageCode): {
            writeHeader(0, message->messageCode);
            break;
        }        
            
        case(DeviceHello::staticMessageCode): {
            String sparkDeviceId = Spark.deviceID();
            
            writeHeader(1 + sparkDeviceId.length(), message->messageCode);
            
            byteArray->putByte(sparkDeviceId.length());
            byteArray->putBytes(sparkDeviceId);
            break;
        }              
          
        case(DigitalPinValue::staticMessageCode): {
            
            DigitalPinValue* digitalPinValue = static_cast<DigitalPinValue*>(message);
            
            writeHeader(2, message->messageCode);
            
            byteArray->putByte(digitalPinValue->pinNumber);
            byteArray->putByte(digitalPinValue->pinValue);
            break;
        }  
        
        case(AnalogPinValue::staticMessageCode): {
            
            AnalogPinValue* analogPinValue = static_cast<AnalogPinValue*>(message);
            
            writeHeader(3, message->messageCode);
            
            byteArray->putByte(analogPinValue->pinNumber);
            byteArray->putWord(analogPinValue->pinValue);
            break;
        } 
        
        case(Ack::staticMessageCode): {
            
            Ack* ack = static_cast<Ack*>(message);
            
            writeHeader(2, message->messageCode);
            
            byteArray->putByte(ack->ackMessageCode);
            byteArray->putByte(ack->ackType);            
            break;
        }
        
        default: {
            if(logToSerail) {
                Serial.print("Serializers.serialize: message code ");
                Serial.print(message->messageCode);
                Serial.println(" was not recognized, could not serialize");
            }
            
            return false;
        }
    }
    
    return true;
}

// =============================================================

Deserializers::Deserializers(bool _logToSerail, IndexedByteArray* _byteArray) {
    logToSerail = _logToSerail;
    byteArray = _byteArray;
    messageNumber = 0;
}

bool Deserializers::isSerializationVersionValid(uint8_t incomingSerializationVersion, uint8_t messageCode) {
    if(incomingSerializationVersion == serializationVersion) {
        return true;
    } else {
        if(logToSerail) {
            Serial.print("Deserializers.isSerializationVersionValid: unsupported serialization version ");
            Serial.print(incomingSerializationVersion);
            Serial.print(" of message ");
            Serial.println(messageCode);
        }
        
        return false;
    }
}

bool Deserializers::isMessageNumberValid(uint8_t incomingMessageNumber, uint8_t messageCode) {
    if(incomingMessageNumber == messageNumber) {
        messageNumber++;
        return true;
    } else {
        if(logToSerail) {
            Serial.print("Deserializers.isMessageNumberValid: invalid message number ");
            Serial.print(incomingMessageNumber);
            Serial.print(" (should be ");
            Serial.print(messageNumber);            
            Serial.print(") of message ");
            Serial.println(messageCode);
        }
        
        return false;
    }
}

IncomingMessage* Deserializers::deserialize() {
    uint8_t messageCode = byteArray->getByte();
    
    if(isSerializationVersionValid(byteArray->getByte(), messageCode) && isMessageNumberValid(byteArray->getByte(), messageCode)) {
        uint8_t ackType = byteArray->getByte();
        IncomingMessage* incomingMessage = deserialize(messageCode);
        
        if(incomingMessage == NULL) {
            return NULL;
        } else {
            incomingMessage->ackType = ackType;
            return incomingMessage;
        }
    } else {
        return NULL;
    }
}

IncomingMessage* Deserializers::deserialize(uint8_t messageCode) {

    /*if(logToSerail) {
        Serial.print("Deserializers.deserialize: trying to deserialize message that has code ");
        Serial.println(messageCode);
    }*/
            
    switch(messageCode) {
        
        case(Ping::staticMessageCode): {
            return &ping;
        }
        
        case(GatewayHello::staticMessageCode): {
            return &gatewayHello;            
        }
        
        case(ServerHello::staticMessageCode): {
            return &serverHello;
        }
        
        case(Disconnect::staticMessageCode): {
            disconnect.delayBeforeNextConnectionAttemptInSeconds = byteArray->getByte();
            return &disconnect; 
        }        

        case(PinConfiguration::staticMessageCode): {
            parsePinConfiguration();
            return &pinConfiguration;
        } 
        
        case(SetDigitalPinValue::staticMessageCode): {
            setDigitalPinValue.pinNumber = byteArray->getByte();
            setDigitalPinValue.pinValue = byteArray->getByte() > 0 ? HIGH : LOW;
            return &setDigitalPinValue;            
        } 
        
        case(SetAnalogPinValue::staticMessageCode): {
            setAnalogPinValue.pinNumber = byteArray->getByte();
            setAnalogPinValue.pinValue = byteArray->getByte();
            return &setAnalogPinValue;
        }         
        
        default: {
            
            if(logToSerail) {
                Serial.print("Deserializers.deserialize: message code ");
                Serial.print(messageCode);
                Serial.println(" was not recognized, could not deserialize");
            }
            
            return NULL;
        }
    }
    
    if(logToSerail) Serial.println("!!!!!!!!!!!!!!!! Deserializers.deserialize: you should never see this message !!!!!!!!!!!!!!!!");    
    return NULL;
}

void Deserializers::parsePinConfiguration() {
    parseDigitalPins(pinConfiguration.pinsConfig.digitalPins);
    parseAnalogPins(pinConfiguration.pinsConfig.analogPins);
}

void Deserializers::parseDigitalPins(DigitalPinConfig digitalPinsConfig[]) {
    for (uint8_t i = 0; i < 8; i++) parseDigitalPin(&digitalPinsConfig[i]); 
}

void Deserializers::parseDigitalPin(DigitalPinConfig* digitalPinConfig) {
    
    digitalPinConfig->pinWorkMode = byteArray->getByte();
    
    switch(digitalPinConfig->pinWorkMode) {
        case OutputPinWorkMode:
            digitalPinConfig->initialPinValueForOutputMode = byteArray->getByte();
            break;
            
        case InputPinWorkMode:
            digitalPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            digitalPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        case InputPullUpPinWorkMode:
            digitalPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            digitalPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        case InputPullDownPinWorkMode:
            digitalPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            digitalPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        default: {}
    }
}

void Deserializers::parseAnalogPins(AnalogPinConfig analogPinsConfig[]) {
    for (uint8_t i = 0; i < 8; i++) parseAnalogPin(&analogPinsConfig[i]); 
}

void Deserializers::parseAnalogPin(AnalogPinConfig* analogPinConfig) {

    analogPinConfig->pinWorkMode = byteArray->getByte();
    
    switch(analogPinConfig->pinWorkMode) {
        case OutputPinWorkMode:
            analogPinConfig->initialPinValueForOutputMode = byteArray->getByte();
            break;
            
        case InputPinWorkMode:
            analogPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            analogPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        case InputPullUpPinWorkMode:
            analogPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            analogPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        case InputPullDownPinWorkMode:
            analogPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            analogPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        default: {}
    }
}

// =============================================================
