#ifndef SPARKANTA_CONNECTION_H
#define SPARKANTA_CONNECTION_H

#include "application.h"
#include "TcpConnection.h"
#include "ByteArray.h"
#include "Message.h"

class SparkantaConnection: public TcpConnection
{
	public:
		SparkantaConnection(String _ip, uint16_t _port, uint8_t _maxConnectionRetries, uint16_t _delayBetweenConnestionReattemptsInMillis, uint32_t _incomingMessageInactivityTimeoutInMillis, String _identyficationString, uint8_t _softwareVersion, uint8_t _hardwareVersion);

        virtual bool connect(IndexedByteArray* byteArray);
        virtual bool readMessage(IndexedByteArray* byteArray);
        virtual void process();
		virtual ~SparkantaConnection();

	protected:
		String identyficationString;
	    uint8_t softwareVersion;
	    uint8_t hardwareVersion;
	    uint32_t incomingMessageInactivityTimeoutInMillis;
	    uint32_t lastIncomingMessageTimeInMillis = millis();
};

#endif /* SPARKANTA_CONNECTION_H */
