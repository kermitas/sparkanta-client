#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "application.h"

class TcpConnection: public TCPClient
{
	public:
		TcpConnection(String _ip, uint16_t _port, uint8_t _maxConnectionRetries, uint16_t _delayBetweenConnestionReattemptsInMillis);

		virtual bool connect();
		virtual uint8_t connected();
		virtual void process();
		virtual ~TcpConnection();

	protected:
	    uint8_t ip[4];
	    uint16_t port;
	    uint8_t maxConnectionRetries;
	    uint8_t connectionRetry;
	    uint16_t delayBetweenConnestionReattemptsInMillis;
		
		virtual void ipArrayFromString(uint8_t ipAsByteArray[], String ip);
		virtual bool connectNow();
};

#endif /* TCP_CONNECTION_H */
