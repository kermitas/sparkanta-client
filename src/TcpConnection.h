#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "application.h"

class TcpConnection: public TCPClient
{
	public:
		TcpConnection(String _ip, uint16_t _port, byte _maxConnectionRetries, uint16_t _delayBetweenConnestionReattemptsInMillis, uint16_t _incomingDataInactivityTimeoutInMillis);

		virtual bool connect();
		virtual uint8_t connected();
		virtual void inactivityCheck();
		virtual void stop();
		virtual ~TcpConnection();

	protected:
	    byte ip[4];
	    uint16_t port;
	    byte maxConnectionRetries;
	    byte connectionRetry;
	    uint16_t delayBetweenConnestionReattemptsInMillis;
	    uint16_t incomingDataInactivityTimeoutInMillis;
	    long lastIncomingDataTime;
		
		virtual void ipArrayFromString(byte ipAsByteArray[], String ip);
		virtual bool connectNow();
};

#endif /* TCP_CONNECTION_H */
