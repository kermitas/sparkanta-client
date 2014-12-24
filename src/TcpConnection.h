#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "application.h"

class TcpConnection: public TCPClient
{
	public:
		TcpConnection(String _ip, uint16_t _port, bool _logToSerail);

		virtual bool connect();
		virtual void stop();

	protected:
	    uint8_t ip[4];
	    uint16_t port;
	    bool logToSerail;

		virtual void ipArrayFromString(uint8_t ipAsByteArray[], String ip);
};

#endif /* TCP_CONNECTION_H */
