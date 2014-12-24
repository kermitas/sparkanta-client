#ifndef SPARKANTA_CONNECTION_H
#define SPARKANTA_CONNECTION_H

#include "application.h"
#include "TcpConnection.h"
#include "ByteArray.h"

class SparkantaConnection: public TcpConnection
{
	public:
	    SparkantaConnection(bool _logToSerail, 
		                    String _ip, 
		                    uint16_t _port, 
		                    String _deviceUniqueName,
		                    uint32_t _incomingMessageInactivityTimeoutInMillis, 
		                    String _identyficationString, 
		                    uint8_t _softwareVersion, 
		                    uint8_t _hardwareVersion,
		                    IndexedByteArray* _incomingByteBuffer,
		                    IndexedByteArray* _outgoingByteBuffer);

        virtual bool connect();
        virtual bool readMessage();
        virtual void sendMessages();
        virtual bool isIncomingTrafficInactivityExceeded();

	protected:
	    String deviceUniqueName;
	    uint32_t incomingMessageInactivityTimeoutInMillis;
		String identyficationString;
	    uint8_t softwareVersion;
	    uint8_t hardwareVersion;
	    uint32_t lastIncomingMessageTimeInMillis;
        IndexedByteArray* outgoingByteBuffer;	
        IndexedByteArray* incomingByteBuffer;
};

#endif /* SPARKANTA_CONNECTION_H */
