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
        
        virtual void serialize(IndexedByteArray* byteArray) = 0;
        virtual bool is(IndexedByteArray* byteArray) = 0;

		virtual ~Message();
};

#endif /* MESSAGE_H */

// =============================================================

#ifndef PING_H
#define PING_H

class Ping: public Message
{
	public:
		Ping();
        
        virtual void serialize(IndexedByteArray* byteArray);
        virtual bool is(IndexedByteArray* byteArray);

		virtual ~Ping();
};

#endif /* PING_H */

// =============================================================

#ifndef PONG_H
#define PONG_H

class Pong: public Message
{
	public:
		Pong();
        
        virtual void serialize(IndexedByteArray* byteArray);
        virtual bool is(IndexedByteArray* byteArray);

		virtual ~Pong();
};

#endif /* PONG_H */

// =============================================================

#ifndef SERVER_HELLO_H
#define SERVER_HELLO_H

class ServerHello: public Message
{
	public:
		ServerHello();
        
        virtual void serialize(IndexedByteArray* byteArray);
        virtual bool is(IndexedByteArray* byteArray);

		virtual ~ServerHello();
};

#endif /* SERVER_HELLO_H */

// =============================================================

#ifndef GATEWAY_HELLO_H
#define GATEWAY_HELLO_H

class GatewayHello: public Message
{
	public:
		GatewayHello();
        
        virtual void serialize(IndexedByteArray* byteArray);
        virtual bool is(IndexedByteArray* byteArray);

		virtual ~GatewayHello();
};

#endif /* GATEWAY_HELLO_H */

// =============================================================

#ifndef DEVICE_HELLO_H
#define DEVICE_HELLO_H

class DeviceHello: public Message
{
	public:
		DeviceHello();
        
        virtual void serialize(IndexedByteArray* byteArray);
        virtual bool is(IndexedByteArray* byteArray);

		virtual ~DeviceHello();
};

#endif /* DEVICE_HELLO_H */

// =============================================================

#ifndef DISCONNECT_H
#define DISCONNECT_H

class Disconnect: public Message
{
	public:
		Disconnect();
        
        virtual void serialize(IndexedByteArray* byteArray);
        virtual bool is(IndexedByteArray* byteArray);
        virtual uint8_t getDelayBeforeNextConnectionAttemptInSeconds(IndexedByteArray* byteArray);

		virtual ~Disconnect();
};

#endif /* DISCONNECT_H */

// =============================================================

#ifndef PINCONFIGURATION_H
#define PINCONFIGURATION_H

#include "PinConfig.h"

class PinConfiguration: public Message
{
	public:
		PinConfiguration();
        
        virtual void serialize(IndexedByteArray* byteArray);
        virtual bool is(IndexedByteArray* byteArray);
        virtual void parse(PinsConfig* pinConfig, IndexedByteArray* byteArray);

		virtual ~PinConfiguration();
		
	protected:
	    virtual void parseDigitalPins(DigitalPinConfig digitalPinsConfig[], IndexedByteArray* byteArray);
	    virtual void parseDigitalPin(DigitalPinConfig* digitalPinConfig, IndexedByteArray* byteArray);
	    virtual void parseAnalogPins(AnalogPinConfig analogPinsConfig[], IndexedByteArray* byteArray);
	    virtual void parseAnalogPin(AnalogPinConfig* analogPinConfig, IndexedByteArray* byteArray);
};

#endif /* PINCONFIGURATION_H */

// =============================================================

#ifndef DIGITAL_PIN_VALUE_H
#define DIGITAL_PIN_VALUE_H

class DigitalPinValue: public Message
{
	public:
		DigitalPinValue();
        
        virtual void serialize(IndexedByteArray* byteArray);
        virtual void serialize(uint8_t pinNumber, uint8_t pinValue, IndexedByteArray* byteArray);
        virtual bool is(IndexedByteArray* byteArray);
        virtual uint8_t getPinNumber(IndexedByteArray* byteArray);
        virtual uint8_t getPinValue(IndexedByteArray* byteArray);
        virtual void consume(IndexedByteArray* byteArray);

		virtual ~DigitalPinValue();
};

#endif /* DIGITAL_PIN_VALUE_H */

// =============================================================

#ifndef ANALOG_PIN_VALUE_H
#define ANALOG_PIN_VALUE_H

class AnalogPinValue: public Message
{
	public:
		AnalogPinValue();
        
        virtual void serialize(IndexedByteArray* byteArray);
        virtual void serialize(uint8_t pinNumber, uint16_t pinValue, IndexedByteArray* byteArray);
        virtual bool is(IndexedByteArray* byteArray);
        virtual uint8_t getPinNumber(IndexedByteArray* byteArray);
        virtual uint8_t getPinValue(IndexedByteArray* byteArray);
        virtual void consume(IndexedByteArray* byteArray);
        
		virtual ~AnalogPinValue();
};

#endif /* ANALOG_PIN_VALUE_H */

// =============================================================

