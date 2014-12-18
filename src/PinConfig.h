// =============================================================

#define outputPinWorkMode 0
#define inputPinWorkMode 1
#define inputPullUpPinWorkMode 2
#define inputPullDownPinWorkMode 3

#define eachAnalogProbeValueReadNotificationType 0
#define eachDigitalProbeValueReadNotificationType 0

// =============================================================

#ifndef PINCONFIG_H
#define PINCONFIG_H

#include "application.h"

class PinConfig
{
	public:
	    uint8_t pinNumber;
	    uint8_t pinWorkMode;
	    uint8_t initialPinValueForOutputMode;
	    uint16_t probeTimeInMillisForInputMode;
	    uint8_t readNotificationTypeForInputMode;
	    
		PinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode);
        
		virtual ~PinConfig();
};

#endif /* PINCONFIG_H */

// =============================================================

#ifndef ANALOGPINCONFIG_H
#define ANALOGPINCONFIG_H

class AnalogPinConfig: public PinConfig
{
	public:
		AnalogPinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode);
        
		virtual ~AnalogPinConfig();
};

#endif /* ANALOGPINCONFIG_H */

// =============================================================

#ifndef DIGITALPINCONFIG_H
#define DIGITALPINCONFIG_H

class DigitalPinConfig: public PinConfig
{
	public:
		DigitalPinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode);
        
		virtual ~DigitalPinConfig();
};

#endif /* DIGITALPINCONFIG_H */

// =============================================================

#ifndef PINSCONFIG_H
#define PINSCONFIG_H

class PinsConfig
{
	public:
	    DigitalPinConfig digitalPins[8] = { DigitalPinConfig(0, outputPinWorkMode, 0, 500, eachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(1, outputPinWorkMode, 0, 500, eachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(2, outputPinWorkMode, 0, 500, eachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(3, outputPinWorkMode, 0, 500, eachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(4, outputPinWorkMode, 0, 500, eachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(5, outputPinWorkMode, 0, 500, eachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(6, outputPinWorkMode, 0, 500, eachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(7, outputPinWorkMode, 0, 500, eachDigitalProbeValueReadNotificationType)
	                                    };
	                                    
	    AnalogPinConfig analogPins[8] = { AnalogPinConfig(0, outputPinWorkMode, 0, 500, eachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(1, outputPinWorkMode, 0, 500, eachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(2, outputPinWorkMode, 0, 500, eachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(3, outputPinWorkMode, 0, 500, eachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(4, outputPinWorkMode, 0, 500, eachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(5, outputPinWorkMode, 0, 500, eachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(6, outputPinWorkMode, 0, 500, eachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(7, outputPinWorkMode, 0, 500, eachAnalogProbeValueReadNotificationType)
	                                    };	                                    
	
		PinsConfig();
        
		virtual ~PinsConfig();
};

#endif /* PINSCONFIG_H */

// =============================================================

#ifndef PINRUNTIMECONFIG_H
#define PINRUNTIMECONFIG_H

class PinRuntimeConfig
{
	public:
	    uint8_t pinNumber;
	    uint32_t lastProbeTime = millis();
	
		PinRuntimeConfig(uint8_t _pinNumber);
        
		virtual ~PinRuntimeConfig();
};

#endif /* PINRUNTIMECONFIG_H */

// =============================================================

#ifndef ANALOGPINRUNTIMECONFIG_H
#define ANALOGPINRUNTIMECONFIG_H

class AnalogPinRuntimeConfig: public PinRuntimeConfig
{
	public:
		AnalogPinRuntimeConfig(uint8_t _pinNumber);
        
		virtual ~AnalogPinRuntimeConfig();
};

#endif /* ANALOGPINRUNTIMECONFIG_H */

// =============================================================

#ifndef DIGITALPINRUNTIMECONFIG_H
#define DIGITALPINRUNTIMECONFIG_H

class DigitalPinRuntimeConfig: public PinRuntimeConfig
{
	public:
		DigitalPinRuntimeConfig(uint8_t _pinNumber);
        
		virtual ~DigitalPinRuntimeConfig();
};

#endif /* DIGITALPINRUNTIMECONFIG_H */

// =============================================================
