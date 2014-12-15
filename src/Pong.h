#ifndef PONG_H
#define PONG_H

#include "application.h"
#include "Message.h"

class Pong: public Message
{
	public:
		Pong();
        
        virtual void send(TCPClient* tcpClient);
        virtual bool is(byte messageAsBytes[], uint8_t length);

		virtual ~Pong();
};

#endif /* PONG_H */
