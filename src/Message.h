#ifndef MESSAGE_H
#define MESSAGE_H

#include "application.h"

class Message
{
	public:
	    byte messageCode;
	
		Message(byte _messageCode);
        
        virtual void send(TCPClient* tcpClient) = 0;
        virtual bool is(byte messageAsBytes[], uint8_t length) = 0;

		virtual ~Message();
};

#endif /* MESSAGE_H */
