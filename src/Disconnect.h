#ifndef DISCONNECT_H
#define DISCONNECT_H

#include "application.h"
#include "Message.h"

class Disconnect: public Message
{
	public:
		Disconnect();
        
        virtual void send(TCPClient* tcpClient);
        virtual bool is(byte messageAsBytes[], uint8_t length);
        virtual uint8_t getDelayBeforeNextConnectionAttemptInSeconds(byte messageAsBytes[]);

		virtual ~Disconnect();
};

#endif /* DISCONNECT_H */
