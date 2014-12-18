// =============================================================

#include "Message.h"

Message::Message(uint8_t _messageCode) {
    messageCode = _messageCode;
}

Message::~Message() { }

// =============================================================

Ping::Ping(): Message(3) { }

void Ping::serialize(IndexedByteArray* byteArray) {
    byteArray->putByte(2); // length
    byteArray->putByte(messageCode);
    byteArray->putByte(1); // serialization version
}

/**
 * Will consume data from IndexedByteArray.
 */
bool Ping::is(IndexedByteArray* byteArray) {
    if(byteArray->getByte(byteArray->index) == messageCode) {
        byteArray->drop(2);
        return true;
    } else {
        return false;
    }
}

Ping::~Ping() { }

// =============================================================

Pong::Pong(): Message(4) { }

void Pong::serialize(IndexedByteArray* byteArray) {
    byteArray->putByte(2); // length
    byteArray->putByte(messageCode);
    byteArray->putByte(1); // serialization version
}

/**
 * Will consume data from IndexedByteArray.
 */
bool Pong::is(IndexedByteArray* byteArray) {
    if(byteArray->getByte(byteArray->index) == messageCode) {
        byteArray->drop(2);
        return true;
    } else {
        return false;
    }
}

Pong::~Pong() { }

// =============================================================

ServerHello::ServerHello(): Message(6) { }

void ServerHello::serialize(IndexedByteArray* byteArray) {
    // this message can only come from server side to device
}

/**
 * Will consume data from IndexedByteArray.
 */
bool ServerHello::is(IndexedByteArray* byteArray) {
    if(byteArray->getByte(byteArray->index) == messageCode) {
        byteArray->drop(2);
        return true;
    } else {
        return false;
    }
}

ServerHello::~ServerHello() { }

// =============================================================

GatewayHello::GatewayHello(): Message(5) { }

void GatewayHello::serialize(IndexedByteArray* byteArray) {
    // this message can only come from server side to device
}

/**
 * Will consume data from IndexedByteArray.
 */
bool GatewayHello::is(IndexedByteArray* byteArray) {
    if(byteArray->getByte(byteArray->index) == messageCode) {
        byteArray->drop(2);
        return true;
    } else {
        return false;
    }
}

GatewayHello::~GatewayHello() { }

// =============================================================

DeviceHello::DeviceHello(): Message(1) { }

void DeviceHello::serialize(IndexedByteArray* byteArray) {
    String sparkDeviceId = Spark.deviceID();
    
    byteArray->putByte(3 + sparkDeviceId.length()); // length
    byteArray->putByte(messageCode);
    byteArray->putByte(1); // serialization version
    byteArray->putByte(sparkDeviceId.length());
    
    byteArray->putBytes(sparkDeviceId);
}

bool DeviceHello::is(IndexedByteArray* byteArray) {
    return false; // this message can be only send from device to server
}

DeviceHello::~DeviceHello() { }

// =============================================================

Disconnect::Disconnect(): Message(2) { }

void Disconnect::serialize(IndexedByteArray* byteArray) {
    // this message can only come from server side to device
}

/**
 * Will NOT consume data from IndexedByteArray.
 */
bool Disconnect::is(IndexedByteArray* byteArray) {
    return byteArray->getByte(byteArray->index) == messageCode;
}

/**
 * Will consume data from IndexedByteArray.
 */
uint8_t Disconnect::getDelayBeforeNextConnectionAttemptInSeconds(IndexedByteArray* byteArray) {
    byteArray->drop(2);
    return byteArray->getByte();
}

Disconnect::~Disconnect() { }

// =============================================================

PinConfiguration::PinConfiguration(): Message(7) { }

void PinConfiguration::serialize(IndexedByteArray* byteArray) {
    // this message can only come from server side to device
}

/**
 * Will NOT consume data from IndexedByteArray.
 */
bool PinConfiguration::is(IndexedByteArray* byteArray) {
    return byteArray->getByte(byteArray->index) == messageCode;
}

/**
 * Will consume data from IndexedByteArray.
 */
void PinConfiguration::parse(PinsConfig* pinConfig, IndexedByteArray* byteArray) {
    
    byteArray->drop(2);
    
    parseDigitalPins(pinConfig->digitalPins, byteArray);
    parseAnalogPins(pinConfig->analogPins, byteArray);
}

void PinConfiguration::parseDigitalPins(DigitalPinConfig digitalPinsConfig[], IndexedByteArray* byteArray) {
    for (uint8_t i = 0; i < 8; i++) parseDigitalPin(&digitalPinsConfig[i], byteArray); 
}

void PinConfiguration::parseDigitalPin(DigitalPinConfig* digitalPinConfig, IndexedByteArray* byteArray) {
    
    digitalPinConfig->pinWorkMode = byteArray->getByte();
    
    switch(digitalPinConfig->pinWorkMode) {
        case outputPinWorkMode:
            digitalPinConfig->initialPinValueForOutputMode = byteArray->getByte();
            break;
            
        case inputPinWorkMode:
            digitalPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            digitalPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        case inputPullUpPinWorkMode:
            digitalPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            digitalPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        case inputPullDownPinWorkMode:
            digitalPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            digitalPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        default: {}
    }
}

void PinConfiguration::parseAnalogPins(AnalogPinConfig analogPinsConfig[], IndexedByteArray* byteArray) {
    for (uint8_t i = 0; i < 8; i++) parseAnalogPin(&analogPinsConfig[i], byteArray); 
}

void PinConfiguration::parseAnalogPin(AnalogPinConfig* analogPinConfig, IndexedByteArray* byteArray) {

    analogPinConfig->pinWorkMode = byteArray->getByte();
    
    switch(analogPinConfig->pinWorkMode) {
        case outputPinWorkMode:
            analogPinConfig->initialPinValueForOutputMode = byteArray->getByte();
            break;
            
        case inputPinWorkMode:
            analogPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            analogPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        case inputPullUpPinWorkMode:
            analogPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            analogPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        case inputPullDownPinWorkMode:
            analogPinConfig->probeTimeInMillisForInputMode = byteArray->getWord();
            analogPinConfig->readNotificationTypeForInputMode = byteArray->getByte();
            break;
            
        default: {}
    }
}

PinConfiguration::~PinConfiguration() { }

// =============================================================

DigitalPinValue::DigitalPinValue(): Message(8) { }

void DigitalPinValue::serialize(IndexedByteArray* byteArray) { }

void DigitalPinValue::serialize(uint8_t pinNumber, uint8_t pinValue, IndexedByteArray* byteArray) {
    byteArray->putByte(4); // length
    byteArray->putByte(messageCode);
    byteArray->putByte(1); // serialization version
    byteArray->putByte(pinNumber);
    byteArray->putByte(pinValue);
}

/**
 * Will NOT consume data from IndexedByteArray.
 */
bool DigitalPinValue::is(IndexedByteArray* byteArray) {
    return byteArray->getByte(byteArray->index) == messageCode;
}

uint8_t DigitalPinValue::getPinNumber(IndexedByteArray* byteArray) {
    return byteArray->getByte(byteArray->index + 2);
}

/**
 * Will NOT consume data from IndexedByteArray.
 */
uint8_t DigitalPinValue::getPinValue(IndexedByteArray* byteArray) {
    return byteArray->getByte(byteArray->index + 3) > 0 ? HIGH : LOW;
}
    
/**
 * Will consume data from IndexedByteArray.
 */  
void DigitalPinValue::consume(IndexedByteArray* byteArray) {
    byteArray->drop(4);
}

DigitalPinValue::~DigitalPinValue() { }

// =============================================================

AnalogPinValue::AnalogPinValue(): Message(9) { }

void AnalogPinValue::serialize(IndexedByteArray* byteArray) { }

void AnalogPinValue::serialize(uint8_t pinNumber, uint16_t pinValue, IndexedByteArray* byteArray) {
    byteArray->putByte(5); // length
    byteArray->putByte(messageCode);
    byteArray->putByte(1); // serialization version
    byteArray->putByte(pinNumber);
    byteArray->putWord(pinValue);
}

/**
 * Will NOT consume data from IndexedByteArray.
 */
bool AnalogPinValue::is(IndexedByteArray* byteArray) {
    return byteArray->getByte(byteArray->index) == messageCode;
}

/**
 * Will NOT consume data from IndexedByteArray.
 */
uint8_t AnalogPinValue::getPinNumber(IndexedByteArray* byteArray) {
    return byteArray->getByte(byteArray->index + 2);
}

/**
 * Will NOT consume data from IndexedByteArray.
 */
uint8_t AnalogPinValue::getPinValue(IndexedByteArray* byteArray) {
    return byteArray->getByte(byteArray->index + 3);
}

/**
 * Will consume data from IndexedByteArray.
 */
void AnalogPinValue::consume(IndexedByteArray* byteArray) {
    byteArray->drop(4);
}

AnalogPinValue::~AnalogPinValue() { }

// =============================================================
