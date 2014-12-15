#ifndef SPARKANTA_CONNECTION_H
#define SPARKANTA_CONNECTION_H

#include "application.h"
#include "TcpConnection.h"

class SparkantaConnection: public TcpConnection
{
	public:
		SparkantaConnection(String _ip, uint16_t _port, byte _maxConnectionRetries, uint16_t _delayBetweenConnestionReattemptsInMillis, uint16_t _incomingDataInactivityTimeoutInMillis, String _identyficationString, byte _softwareVersion, byte _hardwareVersion);

        virtual bool connect();
        virtual int readMessage(byte bytes[]);
		virtual ~SparkantaConnection();

	protected:
		String identyficationString;
	    byte softwareVersion;
	    byte hardwareVersion;
	    
	    virtual void sendIdentyficationStringSoftwareAndHardwareVersion(String identyficationString, byte softwareVersion, byte hardwareVersion);
};

#endif /* SPARKANTA_CONNECTION_H */
