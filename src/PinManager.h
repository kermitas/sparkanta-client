// =============================================================

#ifndef PINSMANAGER_H
#define PINSMANAGER_H

#include "application.h"
#include "PinConfig.h"
#include "Message.h"
#include "ByteArray.h"

class PinsManager
{
	public:
		PinsManager(IndexedByteArray* _outgoingByteBuffer, Serializers* _serializers, uint8_t _doNotPutIntoBufferMoreThan);
        
        virtual void configurePins(PinsConfig* newPinsConfig);
        virtual void process();
        virtual void setPinValue(SetDigitalPinValue* setDigitalPinValue);
        virtual void setPinValue(SetAnalogPinValue* setAnalogPinValue);
		
	protected:

	    DigitalPinRuntimeConfig digitalPins[8] = { DigitalPinRuntimeConfig(0, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
	                                               DigitalPinRuntimeConfig(1, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
        	                                       DigitalPinRuntimeConfig(2, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
        	                                       DigitalPinRuntimeConfig(3, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
        	                                       DigitalPinRuntimeConfig(4, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
        	                                       DigitalPinRuntimeConfig(5, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
        	                                       DigitalPinRuntimeConfig(6, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType),
        	                                       DigitalPinRuntimeConfig(7, OutputPinWorkMode, 0, 500, EachDigitalProbeValueReadNotificationType)
	                                             };
	                                    
	    AnalogPinRuntimeConfig analogPins[8] = { AnalogPinRuntimeConfig(0, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
        	                                     AnalogPinRuntimeConfig(1, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
        	                                     AnalogPinRuntimeConfig(2, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
        	                                     AnalogPinRuntimeConfig(3, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
        	                                     AnalogPinRuntimeConfig(4, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
        	                                     AnalogPinRuntimeConfig(5, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
        	                                     AnalogPinRuntimeConfig(6, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType),
        	                                     AnalogPinRuntimeConfig(7, OutputPinWorkMode, 0, 500, EachAnalogProbeValueReadNotificationType)
	                                           };	                                    
		                
	    DigitalPinValue digitalPinValue;
	    AnalogPinValue analogPinValue;
	    
	    IndexedByteArray* outgoingByteBuffer;
	    Serializers* serializers;
	    uint8_t doNotPutIntoBufferMoreThan;
	    
	    virtual void configureDigitalPin(DigitalPinRuntimeConfig* digitalPinRuntimeConfig);                                       
        virtual void configureAnalogPin(AnalogPinRuntimeConfig* analogPinRuntimeConfig);
        
	    virtual void processDigitalPin(DigitalPinRuntimeConfig* digitalPinRuntimeConfig);                                       
        virtual void processAnalogPin(AnalogPinRuntimeConfig* analogPinRuntimeConfig);
        
        virtual uint16_t getSparkDigitalPinNumber(DigitalPinRuntimeConfig* digitalPinRuntimeConfig);
        virtual uint16_t getSparkAnalogPinNumber(AnalogPinRuntimeConfig* analogPinRuntimeConfig);
};

#endif /* PINSMANAGER_H */

// =============================================================
