// =============================================================

#define OutputPinWorkMode 0
#define InputPinWorkMode 1
#define InputPullUpPinWorkMode 2
#define InputPullDownPinWorkMode 3

#define EachAnalogProbeValueReadNotificationType 0
#define EachAnalogProbeChangeReadNotificationType 1

#define EachDigitalProbeValueReadNotificationType 0
#define EachDigitalProbeChangeReadNotificationType 1

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
};

#endif /* PINCONFIG_H */

// =============================================================

#ifndef ANALOGPINCONFIG_H
#define ANALOGPINCONFIG_H

class AnalogPinConfig: public PinConfig
{
	public:
		AnalogPinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode);
};

#endif /* ANALOGPINCONFIG_H */

// =============================================================

#ifndef DIGITALPINCONFIG_H
#define DIGITALPINCONFIG_H

class DigitalPinConfig: public PinConfig
{
	public:
		DigitalPinConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode);
};

#endif /* DIGITALPINCONFIG_H */

// =============================================================

#ifndef PINSCONFIG_H
#define PINSCONFIG_H

class PinsConfig
{
	public:
	    DigitalPinConfig digitalPins[8] = { DigitalPinConfig(0, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(1, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(2, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(3, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(4, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(5, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(6, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
	                                        DigitalPinConfig(7, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType)
	                                    };
	                                    
	    AnalogPinConfig analogPins[8] = { AnalogPinConfig(0, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(1, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(2, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(3, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(4, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(5, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(6, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
	                                      AnalogPinConfig(7, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType)
	                                    };	                                    
	
		PinsConfig();
};

#endif /* PINSCONFIG_H */

// =============================================================

#ifndef ANALOGPINRUNTIMECONFIG_H
#define ANALOGPINRUNTIMECONFIG_H

class AnalogPinRuntimeConfig: public AnalogPinConfig
{
	public:
	    uint32_t lastProbeTime;
	    int32_t previousPinValue;
	
	    AnalogPinRuntimeConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode);
};

#endif /* ANALOGPINRUNTIMECONFIG_H */

// =============================================================

#ifndef DIGITALPINRUNTIMECONFIG_H
#define DIGITALPINRUNTIMECONFIG_H

class DigitalPinRuntimeConfig: public DigitalPinConfig
{
	public:
	    uint32_t lastProbeTime;
	    int32_t previousPinValue;
	    
	    DigitalPinRuntimeConfig(uint8_t _pinNumber, uint8_t _pinWorkMode, uint8_t _initialPinValueForOutputMode, uint16_t _probeTimeInMillisForInputMode, uint8_t _readNotificationTypeForInputMode);
};

#endif /* DIGITALPINRUNTIMECONFIG_H */

// =============================================================
