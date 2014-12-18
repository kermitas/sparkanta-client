// =============================================================

#include "PinManager.h"

PinsManager::PinsManager() { }

void PinsManager::configurePins(PinsConfig* pinsConfig) {
    for (uint8_t i = 0; i < 8; i++) configureDigitalPin(&pinsConfig->digitalPins[i], &digitalPins[i]);
    for (uint8_t i = 0; i < 8; i++) configureAnalogPin(&pinsConfig->analogPins[i], &analogPins[i]);
}

uint16_t PinsManager::getSparkDigitalPinNumber(uint8_t digiralPinNumber) {
    return digiralPinNumber;
}

uint16_t PinsManager::getSparkAnalogPinNumber(uint8_t analogPinNumber) {
    return analogPinNumber + 10;
}
        
void PinsManager::configureDigitalPin(DigitalPinConfig* digitalPinConfig, DigitalPinRuntimeConfig* digitalPinRuntimeConfig) {
    
    digitalPinRuntimeConfig->lastProbeTime = millis();
    
    uint16_t pinNumber = getSparkDigitalPinNumber(digitalPinConfig->pinNumber);
    
    switch(digitalPinConfig->pinWorkMode) {
        case outputPinWorkMode:
            pinMode(pinNumber, OUTPUT);
            digitalWrite(pinNumber, digitalPinConfig->initialPinValueForOutputMode > 0 ? HIGH : LOW);
            break;
            
        case inputPinWorkMode:
            pinMode(pinNumber, INPUT);
            break;
            
        case inputPullUpPinWorkMode:
            pinMode(pinNumber, INPUT_PULLUP);
            break;
            
        case inputPullDownPinWorkMode:
            pinMode(pinNumber, INPUT_PULLDOWN);
            break;
            
        default: {}
    }
}

void PinsManager::configureAnalogPin(AnalogPinConfig* analogPinConfig, AnalogPinRuntimeConfig* analogPinRuntimeConfig) {
   
    analogPinRuntimeConfig->lastProbeTime = millis();
    
    uint16_t pinNumber = getSparkAnalogPinNumber(analogPinConfig->pinNumber);
    
    switch(analogPinConfig->pinWorkMode) {
        case outputPinWorkMode:
            pinMode(pinNumber, OUTPUT);
            analogWrite(pinNumber, analogPinConfig->initialPinValueForOutputMode);
            break;
            
        case inputPinWorkMode:
            pinMode(pinNumber, INPUT);
            break;
            
        case inputPullUpPinWorkMode:
            pinMode(pinNumber, INPUT_PULLUP);
            break;
            
        case inputPullDownPinWorkMode:
            pinMode(pinNumber, INPUT_PULLDOWN);
            break;
            
        default: {}
    }
}

void PinsManager::process(PinsConfig* pinsConfig, IndexedByteArray* byteArray) {
    for (uint8_t i = 0; i < 8; i++) processDigitalPin(&pinsConfig->digitalPins[i], &digitalPins[i], byteArray);
    for (uint8_t i = 0; i < 8; i++) processAnalogPin(&pinsConfig->analogPins[i], &analogPins[i], byteArray);
}

void PinsManager::processDigitalPin(DigitalPinConfig* digitalPinConfig, DigitalPinRuntimeConfig* digitalPinRuntimeConfig, IndexedByteArray* byteArray) {
    if(digitalPinConfig->pinWorkMode == inputPinWorkMode || digitalPinConfig->pinWorkMode == inputPullUpPinWorkMode || digitalPinConfig->pinWorkMode == inputPullDownPinWorkMode) {
        if(millis() - digitalPinRuntimeConfig->lastProbeTime >= digitalPinConfig->probeTimeInMillisForInputMode) {
            
            digitalPinRuntimeConfig->lastProbeTime = millis();
            
            uint16_t pinNumber = getSparkDigitalPinNumber(digitalPinConfig->pinNumber);
            
            switch(digitalPinConfig->readNotificationTypeForInputMode) {
                case eachDigitalProbeValueReadNotificationType:  {
                    uint8_t pinValue = digitalRead(pinNumber);
                    
                    /*
                    // TODO to remove -------------------------
                    Serial.print("digital read (interval ");
                    Serial.print(digitalPinConfig->probeTimeInMillisForInputMode);
                    Serial.print(" ms) from pin ");
                    Serial.print(digitalPinConfig->pinNumber);
                    Serial.print("=");
                    Serial.println(pinValue);
                    // ----------------------------------------
                    */

                    digitalPinValue.serialize(digitalPinConfig->pinNumber, pinValue, byteArray);
                    
                    break;
                }
                    
                default: {}
            }
        }
    }
}

void PinsManager::processAnalogPin(AnalogPinConfig* analogPinConfig, AnalogPinRuntimeConfig* analogPinRuntimeConfig, IndexedByteArray* byteArray) {
    if(analogPinConfig->pinWorkMode == inputPinWorkMode || analogPinConfig->pinWorkMode == inputPullUpPinWorkMode || analogPinConfig->pinWorkMode == inputPullDownPinWorkMode) {
        if(millis() - analogPinRuntimeConfig->lastProbeTime >= analogPinConfig->probeTimeInMillisForInputMode) {
            
            analogPinRuntimeConfig->lastProbeTime = millis();
            
            uint16_t pinNumber = getSparkAnalogPinNumber(analogPinConfig->pinNumber);
            
            switch(analogPinConfig->readNotificationTypeForInputMode) {
                case eachAnalogProbeValueReadNotificationType:  {
                    uint16_t pinValue = analogRead(pinNumber);
                    
                    /*
                    // TODO to remove -------------------------
                    Serial.print("analog read (interval ");
                    Serial.print(analogPinConfig->probeTimeInMillisForInputMode);
                    Serial.print(" ms) from pin ");
                    Serial.print(analogPinConfig->pinNumber);
                    Serial.print("=");
                    Serial.println(pinValue);
                    // ----------------------------------------
                    */
                    
                    analogPinValue.serialize(analogPinConfig->pinNumber, pinValue, byteArray);
                    
                    break;
                }
                    
                default: {}
            }
        }
    }
}

PinsManager::~PinsManager() { }

// =============================================================
