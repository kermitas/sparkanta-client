#ifndef PING_H
#define PING_H

#include "application.h"
#include "Message.h"

class Ping: public Message
{
	public:
		Ping();
        
        virtual void send(TCPClient* tcpClient);
        virtual bool is(byte messageAsBytes[], uint8_t length);

		virtual ~Ping();
};

#endif /* PING_H */
