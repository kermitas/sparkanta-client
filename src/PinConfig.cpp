// =============================================================

#include "PinConfig.h"

PinConfig::PinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode) {
    pinNumber = _pinNumber;
    pinWorkMode = _pinWorkMode;
    initialPinValueForOutputMode = _initialPinValueForOutputMode;
    probeTimeInMillisForInputMode = _probeTimeInMillisForInputMode;
    readNotificationTypeForInputMode = _readNotificationTypeForInputMode;
}

PinConfig::~PinConfig() { }

// =============================================================

AnalogPinConfig::AnalogPinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode): PinConfig(_pinNumber, _pinWorkMode, _initialPinValueForOutputMode, _probeTimeInMillisForInputMode, _readNotificationTypeForInputMode) {
}

AnalogPinConfig::~AnalogPinConfig() { }

// =============================================================

DigitalPinConfig::DigitalPinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode): PinConfig(_pinNumber, _pinWorkMode, _initialPinValueForOutputMode, _probeTimeInMillisForInputMode, _readNotificationTypeForInputMode) {
}

DigitalPinConfig::~DigitalPinConfig() { }

// =============================================================

PinsConfig::PinsConfig() { }

PinsConfig::~PinsConfig() { }

// =============================================================

PinRuntimeConfig::PinRuntimeConfig(uint8_t _pinNumber) {
    pinNumber = _pinNumber;
}

PinRuntimeConfig::~PinRuntimeConfig() { }

// =============================================================

AnalogPinRuntimeConfig::AnalogPinRuntimeConfig(uint8_t _pinNumber): PinRuntimeConfig(_pinNumber) { }

AnalogPinRuntimeConfig::~AnalogPinRuntimeConfig() { }

// =============================================================

DigitalPinRuntimeConfig::DigitalPinRuntimeConfig(uint8_t _pinNumber): PinRuntimeConfig(_pinNumber) { }

DigitalPinRuntimeConfig::~DigitalPinRuntimeConfig() { }

// =============================================================
