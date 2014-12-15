#ifndef SERVER_HELLO_H
#define SERVER_HELLO_H

#include "application.h"
#include "Message.h"

class ServerHello: public Message
{
	public:
		ServerHello();
        
        virtual void send(TCPClient* tcpClient);
        virtual bool is(byte messageAsBytes[], uint8_t length);

		virtual ~ServerHello();
};

#endif /* SERVER_HELLO_H */
