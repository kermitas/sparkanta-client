// =============================================================

#include "PinManager.h"

PinsManager::PinsManager(IndexedByteArray* _outgoingByteBuffer, Serializers* _serializers, uint8_t _doNotPutIntoBufferMoreThan) { 
    outgoingByteBuffer = _outgoingByteBuffer;
    serializers = _serializers;
    doNotPutIntoBufferMoreThan = _doNotPutIntoBufferMoreThan;
}

void PinsManager::configurePins(PinsConfig* newPinsConfig) {
    
    for (uint8_t i = 0; i < 8; i++) {
        digitalPins[i].pinNumber                         = newPinsConfig->digitalPins[i].pinNumber;
        digitalPins[i].pinWorkMode                       = newPinsConfig->digitalPins[i].pinWorkMode;
        digitalPins[i].initialPinValueForOutputMode      = newPinsConfig->digitalPins[i].initialPinValueForOutputMode;
        digitalPins[i].probeTimeInMillisForInputMode     = newPinsConfig->digitalPins[i].probeTimeInMillisForInputMode;
        digitalPins[i].readNotificationTypeForInputMode  = newPinsConfig->digitalPins[i].readNotificationTypeForInputMode;
        
        analogPins[i].pinNumber                          = newPinsConfig->analogPins[i].pinNumber;
        analogPins[i].pinWorkMode                        = newPinsConfig->analogPins[i].pinWorkMode;
        analogPins[i].initialPinValueForOutputMode       = newPinsConfig->analogPins[i].initialPinValueForOutputMode;
        analogPins[i].probeTimeInMillisForInputMode      = newPinsConfig->analogPins[i].probeTimeInMillisForInputMode;
        analogPins[i].readNotificationTypeForInputMode   = newPinsConfig->analogPins[i].readNotificationTypeForInputMode;        
    }
    
    for (uint8_t i = 0; i < 8; i++) configureDigitalPin(&digitalPins[i]);
    for (uint8_t i = 0; i < 8; i++) configureAnalogPin(&analogPins[i]);
}

uint16_t PinsManager::getSparkDigitalPinNumber(DigitalPinRuntimeConfig* digitalPinRuntimeConfig) {
    return digitalPinRuntimeConfig->pinNumber;
}

uint16_t PinsManager::getSparkAnalogPinNumber(AnalogPinRuntimeConfig* analogPinRuntimeConfig) {
    return analogPinRuntimeConfig->pinNumber + 10;
}
        
void PinsManager::configureDigitalPin(DigitalPinRuntimeConfig* digitalPinRuntimeConfig) {
    
    digitalPinRuntimeConfig->lastProbeTime = millis();
    
    uint16_t pinNumber = getSparkDigitalPinNumber(digitalPinRuntimeConfig);
    
    switch(digitalPinRuntimeConfig->pinWorkMode) {
        case OutputPinWorkMode:
            pinMode(pinNumber, OUTPUT);
            digitalWrite(pinNumber, digitalPinRuntimeConfig->initialPinValueForOutputMode > 0 ? HIGH : LOW);
            break;
            
        case InputPinWorkMode:
            pinMode(pinNumber, INPUT);
            break;
            
        case InputPullUpPinWorkMode:
            pinMode(pinNumber, INPUT_PULLUP);
            break;
            
        case InputPullDownPinWorkMode:
            pinMode(pinNumber, INPUT_PULLDOWN);
            break;
            
        default: {}
    }
}

void PinsManager::configureAnalogPin(AnalogPinRuntimeConfig* analogPinRuntimeConfig) {
   
    analogPinRuntimeConfig->lastProbeTime = millis();
    
    uint16_t pinNumber = getSparkAnalogPinNumber(analogPinRuntimeConfig);
    
    switch(analogPinRuntimeConfig->pinWorkMode) {
        case OutputPinWorkMode:
            pinMode(pinNumber, OUTPUT);
            analogWrite(pinNumber, analogPinRuntimeConfig->initialPinValueForOutputMode);
            break;
            
        case InputPinWorkMode:
            pinMode(pinNumber, INPUT);
            break;
            
        case InputPullUpPinWorkMode:
            pinMode(pinNumber, INPUT_PULLUP);
            break;
            
        case InputPullDownPinWorkMode:
            pinMode(pinNumber, INPUT_PULLDOWN);
            break;
            
        default: {}
    }
}

void PinsManager::process() {
    
    uint16_t initialByteArrayIndex = outgoingByteBuffer->index;
    
    for (uint8_t i = 0; i < 8 && (outgoingByteBuffer->index - initialByteArrayIndex < doNotPutIntoBufferMoreThan); i++) processDigitalPin(&digitalPins[i]);
    for (uint8_t i = 0; i < 8 && (outgoingByteBuffer->index - initialByteArrayIndex < doNotPutIntoBufferMoreThan); i++) processAnalogPin(&analogPins[i]);
}

void PinsManager::processDigitalPin(DigitalPinRuntimeConfig* digitalPinRuntimeConfig) {
    if(digitalPinRuntimeConfig->pinWorkMode == InputPinWorkMode || digitalPinRuntimeConfig->pinWorkMode == InputPullUpPinWorkMode || digitalPinRuntimeConfig->pinWorkMode == InputPullDownPinWorkMode) {
        if(digitalPinRuntimeConfig->probeTimeInMillisForInputMode <= 0 || millis() - digitalPinRuntimeConfig->lastProbeTime >= digitalPinRuntimeConfig->probeTimeInMillisForInputMode) {
            
            digitalPinRuntimeConfig->lastProbeTime = millis();
            
            switch(digitalPinRuntimeConfig->readNotificationTypeForInputMode) {
                case EachDigitalProbeValueReadNotificationType: {

                    digitalPinValue.pinNumber = digitalPinRuntimeConfig->pinNumber;
                    digitalPinValue.pinValue = digitalRead(getSparkDigitalPinNumber(digitalPinRuntimeConfig));
                    
                    serializers->serialize(&digitalPinValue); // TODO: what if serialization will fail?
                    
                    break;
                }
                    
                case EachDigitalProbeChangeReadNotificationType: {
                    
                    uint8_t currentPinValue = digitalRead(getSparkDigitalPinNumber(digitalPinRuntimeConfig));
                    
                    if(currentPinValue != digitalPinRuntimeConfig->previousPinValue) {
                        
                        digitalPinRuntimeConfig->previousPinValue = currentPinValue;
                        
                        digitalPinValue.pinNumber = digitalPinRuntimeConfig->pinNumber;
                        digitalPinValue.pinValue = currentPinValue;
                        
                        serializers->serialize(&digitalPinValue); // TODO: what if serialization will fail?
                    }
                    
                    break;
                }
                
                default: {}
            }
        }
    }
}

void PinsManager::processAnalogPin(AnalogPinRuntimeConfig* analogPinRuntimeConfig) {
    if(analogPinRuntimeConfig->pinWorkMode == InputPinWorkMode || analogPinRuntimeConfig->pinWorkMode == InputPullUpPinWorkMode || analogPinRuntimeConfig->pinWorkMode == InputPullDownPinWorkMode) {
        if(analogPinRuntimeConfig->probeTimeInMillisForInputMode <=0 || millis() - analogPinRuntimeConfig->lastProbeTime >= analogPinRuntimeConfig->probeTimeInMillisForInputMode) {
            
            analogPinRuntimeConfig->lastProbeTime = millis();

            switch(analogPinRuntimeConfig->readNotificationTypeForInputMode) {
                case EachAnalogProbeValueReadNotificationType: {
  
                    analogPinValue.pinNumber = analogPinRuntimeConfig->pinNumber;
                    analogPinValue.pinValue = analogRead(getSparkAnalogPinNumber(analogPinRuntimeConfig));
                    
                    serializers->serialize(&analogPinValue); // TODO: what if serialization will fail?
                    
                    break;
                }
                    
                case EachAnalogProbeChangeReadNotificationType: {
                    
                    uint16_t currentPinValue = analogRead(getSparkAnalogPinNumber(analogPinRuntimeConfig));
                    
                    if(currentPinValue != analogPinRuntimeConfig->previousPinValue) {
                        
                        analogPinRuntimeConfig->previousPinValue = currentPinValue;
                        
                        analogPinValue.pinNumber = analogPinRuntimeConfig->pinNumber;
                        analogPinValue.pinValue = currentPinValue;
                        
                        serializers->serialize(&analogPinValue); // TODO: what if serialization will fail?
                    }
                    
                    break;
                }
                
                default: {}
            }
        }
    }
}

void PinsManager::setPinValue(SetDigitalPinValue* setDigitalPinValue) {
    DigitalPinRuntimeConfig* digitalPinRuntimeConfig = &digitalPins[setDigitalPinValue->pinNumber];

    if(digitalPinRuntimeConfig->pinWorkMode == OutputPinWorkMode) {
        uint8_t pinValue = setDigitalPinValue->pinValue;
                            
        digitalPinRuntimeConfig->initialPinValueForOutputMode = pinValue;

        digitalWrite(getSparkDigitalPinNumber(digitalPinRuntimeConfig), pinValue);   
    }
}

void PinsManager::setPinValue(SetAnalogPinValue* setAnalogPinValue) {
    AnalogPinRuntimeConfig* analogPinRuntimeConfig = &analogPins[setAnalogPinValue->pinNumber];

    if(analogPinRuntimeConfig->pinWorkMode == OutputPinWorkMode) {
        uint8_t pinValue = setAnalogPinValue->pinValue;
                            
        analogPinRuntimeConfig->initialPinValueForOutputMode = pinValue;

        analogWrite(getSparkAnalogPinNumber(analogPinRuntimeConfig), pinValue);   
    }
}

// =============================================================
