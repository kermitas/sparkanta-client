#ifndef DEVICE_HELLO_H
#define DEVICE_HELLO_H

#include "application.h"
#include "Message.h"

class DeviceHello: public Message
{
	public:
		DeviceHello();
        
        virtual void send(TCPClient* tcpClient);
        virtual bool is(byte messageAsBytes[], uint8_t length);

		virtual ~DeviceHello();
};

#endif /* DEVICE_HELLO_H */
