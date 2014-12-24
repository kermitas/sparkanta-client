// =============================================================

#include "PinConfig.h"

PinConfig::PinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode) {
    pinNumber = _pinNumber;
    pinWorkMode = _pinWorkMode;
    initialPinValueForOutputMode = _initialPinValueForOutputMode;
    probeTimeInMillisForInputMode = _probeTimeInMillisForInputMode;
    readNotificationTypeForInputMode = _readNotificationTypeForInputMode;
}

// =============================================================

AnalogPinConfig::AnalogPinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode): PinConfig(_pinNumber, _pinWorkMode, _initialPinValueForOutputMode, _probeTimeInMillisForInputMode, _readNotificationTypeForInputMode) {
}

// =============================================================

DigitalPinConfig::DigitalPinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode): PinConfig(_pinNumber, _pinWorkMode, _initialPinValueForOutputMode, _probeTimeInMillisForInputMode, _readNotificationTypeForInputMode) {
}

// =============================================================

PinsConfig::PinsConfig() { }

// =============================================================

AnalogPinRuntimeConfig::AnalogPinRuntimeConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode): AnalogPinConfig(_pinNumber, _pinWorkMode, _initialPinValueForOutputMode, _probeTimeInMillisForInputMode, _readNotificationTypeForInputMode) {
    lastProbeTime = millis();
    previousPinValue = -1;
}

// =============================================================

DigitalPinRuntimeConfig::DigitalPinRuntimeConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode): DigitalPinConfig(_pinNumber, _pinWorkMode, _initialPinValueForOutputMode, _probeTimeInMillisForInputMode, _readNotificationTypeForInputMode) {
    lastProbeTime = millis();
    previousPinValue = -1;
}

// =============================================================
