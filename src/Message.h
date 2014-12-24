// =============================================================

#define NoAckAckType 0
#define ReceivedAckType 1

// =============================================================

#ifndef MESSAGE_H
#define MESSAGE_H

#include "application.h"
#include "ByteArray.h"

class Message
{
	public:
	    uint8_t messageCode;
	
		Message(uint8_t _messageCode);
};

#endif /* MESSAGE_H */

// =============================================================

#ifndef INCOMING_MESSAGE_H
#define INCOMING_MESSAGE_H

class IncomingMessage: public Message
{
	public:
	    uint8_t ackType;
	
		IncomingMessage(uint8_t _messageCode);
};

#endif /* INCOMING_MESSAGE_H */

// =============================================================

#ifndef OUTGOING_MESSAGE_H
#define OUTGOING_MESSAGE_H

class OutgoingMessage: public Message
{
	public:
		OutgoingMessage(uint8_t _messageCode);
};

#endif /* OUTGOING_MESSAGE_H */

// =============================================================

#ifndef PING_H
#define PING_H

class Ping: public IncomingMessage
{
	public:
	    static const uint8_t staticMessageCode = 3;
	
		Ping();
};

#endif /* PING_H */

// =============================================================

#ifndef PONG_H
#define PONG_H

class Pong: public OutgoingMessage
{
	public:
	    static const uint8_t staticMessageCode = 4;
	
		Pong();
};

#endif /* PONG_H */

// =============================================================

#ifndef SERVER_HELLO_H
#define SERVER_HELLO_H

class ServerHello: public IncomingMessage
{
	public:
	    static const uint8_t staticMessageCode = 6;
	
		ServerHello();
};

#endif /* SERVER_HELLO_H */

// =============================================================

#ifndef GATEWAY_HELLO_H
#define GATEWAY_HELLO_H

class GatewayHello: public IncomingMessage
{
	public:
	    static const uint8_t staticMessageCode = 5;
	    
		GatewayHello();
};

#endif /* GATEWAY_HELLO_H */

// =============================================================

#ifndef DEVICE_HELLO_H
#define DEVICE_HELLO_H

class DeviceHello: public OutgoingMessage
{
	public:
	    static const uint8_t staticMessageCode = 1;
	    
		DeviceHello();
};

#endif /* DEVICE_HELLO_H */

// =============================================================

#ifndef DISCONNECT_H
#define DISCONNECT_H

class Disconnect: public IncomingMessage
{
	public:
	    static const uint8_t staticMessageCode = 2;
	    uint8_t delayBeforeNextConnectionAttemptInSeconds;
	    
		Disconnect();
};

#endif /* DISCONNECT_H */

// =============================================================

#ifndef PINCONFIGURATION_H
#define PINCONFIGURATION_H

#include "PinConfig.h"

class PinConfiguration: public IncomingMessage
{
	public:
	    static const uint8_t staticMessageCode = 7;
	    PinsConfig pinsConfig;
	    
		PinConfiguration();
};

#endif /* PINCONFIGURATION_H */

// =============================================================

#ifndef DIGITAL_PIN_VALUE_H
#define DIGITAL_PIN_VALUE_H

class DigitalPinValue: public OutgoingMessage
{
	public:
	    static const uint8_t staticMessageCode = 8;
	    uint8_t pinNumber;
        uint8_t pinValue;
            
		DigitalPinValue();
};

#endif /* DIGITAL_PIN_VALUE_H */

// =============================================================

#ifndef ANALOG_PIN_VALUE_H
#define ANALOG_PIN_VALUE_H

class AnalogPinValue: public OutgoingMessage
{
	public:
	    static const uint8_t staticMessageCode = 9;
	    uint8_t pinNumber;
        uint16_t pinValue;
            
		AnalogPinValue();
};

#endif /* ANALOG_PIN_VALUE_H */

// =============================================================

#ifndef SET_DIGITAL_PIN_VALUE_H
#define SET_DIGITAL_PIN_VALUE_H

class SetDigitalPinValue: public IncomingMessage
{
	public:
	    static const uint8_t staticMessageCode = 10;
	    uint8_t pinNumber;
        uint8_t pinValue;
            
		SetDigitalPinValue();
};

#endif /* SET_DIGITAL_PIN_VALUE_H */

// =============================================================

#ifndef SET_ANALOG_PIN_VALUE_H
#define SET_ANALOG_PIN_VALUE_H

class SetAnalogPinValue: public IncomingMessage
{
	public:
	    static const uint8_t staticMessageCode = 11;
	    uint8_t pinNumber;
        uint8_t pinValue;
            
		SetAnalogPinValue();
};

#endif /* SET_ANALOG_PIN_VALUE_H */

// =============================================================

#ifndef ACK_H
#define ACK_H

class Ack: public OutgoingMessage
{
	public:
	    static const uint8_t staticMessageCode = 12;
	    uint8_t ackMessageCode;
        uint8_t ackType;
            
		Ack();
};

#endif /* ACK_H */

// =============================================================

#ifndef SERIALIZERS_H
#define SERIALIZERS_H

class Serializers
{
	public:
	    uint8_t messageNumber;
	
		Serializers(bool _logToSerail, IndexedByteArray* _byteArray);
        
        virtual bool serialize(OutgoingMessage* message);
        
    protected:
        static const uint8_t serializationVersion = 1;
        
	    bool logToSerail;
	    IndexedByteArray* byteArray;
	    
        virtual void writeHeader(uint8_t length, uint8_t messageCode);
};

#endif /* SERIALIZERS_H */

// =============================================================

#ifndef DESERIALIZERS_H
#define DESERIALIZERS_H

class Deserializers
{
	public:
	    uint8_t messageNumber;
	    
		Deserializers(bool _logToSerail, IndexedByteArray* _byteArray);
        
        virtual IncomingMessage* deserialize();
		
	protected:
	    static const uint8_t serializationVersion = 1;
	
	    bool logToSerail;
	    IndexedByteArray* byteArray;
	    
	    Ping ping;
	    GatewayHello gatewayHello;
	    ServerHello serverHello;
	    Disconnect disconnect;
	    PinConfiguration pinConfiguration;
	    SetDigitalPinValue setDigitalPinValue;
	    SetAnalogPinValue setAnalogPinValue;	    
	    
	    virtual bool isSerializationVersionValid(uint8_t incomingSerializationVersion, uint8_t messageCode);
	    virtual bool isMessageNumberValid(uint8_t incomingMessageNumber, uint8_t messageCode);
	    virtual IncomingMessage* deserialize(uint8_t messageCode);
        virtual void parsePinConfiguration();
        virtual void parseDigitalPins(DigitalPinConfig digitalPinsConfig[]);
        virtual void parseDigitalPin(DigitalPinConfig* digitalPinConfig);
        virtual void parseAnalogPins(AnalogPinConfig analogPinsConfig[]);
        virtual void parseAnalogPin(AnalogPinConfig* analogPinConfig);
};

#endif /* DESERIALIZERS_H */

// =============================================================
