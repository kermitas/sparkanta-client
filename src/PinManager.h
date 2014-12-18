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
		PinsManager();
        
        virtual void configurePins(PinsConfig* pinsConfig);
        virtual void process(PinsConfig* pinsConfig, IndexedByteArray* byteArray);
        
		virtual ~PinsManager();
		
	protected:
	    DigitalPinRuntimeConfig digitalPins[8] = { DigitalPinRuntimeConfig(0),
	                                               DigitalPinRuntimeConfig(1),
	                                               DigitalPinRuntimeConfig(2),
	                                               DigitalPinRuntimeConfig(3),
	                                               DigitalPinRuntimeConfig(4),
	                                               DigitalPinRuntimeConfig(5),
	                                               DigitalPinRuntimeConfig(6),
	                                               DigitalPinRuntimeConfig(7)
	                                           };
	                                    
	    AnalogPinRuntimeConfig analogPins[8] = { AnalogPinRuntimeConfig(0),
	                                             AnalogPinRuntimeConfig(1),
	                                             AnalogPinRuntimeConfig(2),
	                                             AnalogPinRuntimeConfig(3),
	                                             AnalogPinRuntimeConfig(4),
	                                             AnalogPinRuntimeConfig(5),
	                                             AnalogPinRuntimeConfig(6),
	                                             AnalogPinRuntimeConfig(7)
	                                           };	
	                
	    DigitalPinValue digitalPinValue;
	    AnalogPinValue analogPinValue;
	    
	    virtual void configureDigitalPin(DigitalPinConfig* digitalPinConfig, DigitalPinRuntimeConfig* digitalPinRuntimeConfig);                                       
        virtual void configureAnalogPin(AnalogPinConfig* analogPinConfig, AnalogPinRuntimeConfig* analogPinRuntimeConfig);
        
	    virtual void processDigitalPin(DigitalPinConfig* digitalPinConfig, DigitalPinRuntimeConfig* digitalPinRuntimeConfig, IndexedByteArray* byteArray);                                       
        virtual void processAnalogPin(AnalogPinConfig* analogPinConfig, AnalogPinRuntimeConfig* analogPinRuntimeConfig, IndexedByteArray* byteArray);
        
        virtual uint16_t getSparkDigitalPinNumber(uint8_t digiralPinNumber);
        virtual uint16_t getSparkAnalogPinNumber(uint8_t analogPinNumber);
};

#endif /* PINSMANAGER_H */

// =============================================================
