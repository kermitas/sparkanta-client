#ifndef GATEWAY_HELLO_H
#define GATEWAY_HELLO_H

#include "application.h"
#include "Message.h"

class GatewayHello: public Message
{
	public:
		GatewayHello();
        
        virtual void send(TCPClient* tcpClient);
        virtual bool is(byte messageAsBytes[], uint8_t length);

		virtual ~GatewayHello();
};

#endif /* GATEWAY_HELLO_H */
