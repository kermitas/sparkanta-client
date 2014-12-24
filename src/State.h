// =============================================================

#ifndef STATE_H
#define STATE_H

#include "application.h"

class State
{
	public:
	    String name;
	    
		State(uint8_t _number, String _name, bool _logToSerail, uint32_t _stateTimeoutInMillis);
        
        virtual void enteringState();
        virtual void leavingState();
        virtual uint32_t timeFromEnteringInMillis();
        virtual State* process();
        
    protected:
    	uint8_t number;
	    uint32_t enteringTimeInMillis;
	    bool logToSerail;
	    uint32_t stateTimeoutInMillis;
	    
	    virtual State* processStateTimeout();
	    virtual State* processState() = 0;
};

#endif /* STATE_H */

// =============================================================

#ifndef NETWORKING_STATE_H
#define NETWORKING_STATE_H

#include "Message.h"
#include "SparkantaConnection.h"

class NetworkingState: public State
{
	public:
	    State* connectingState;
	    
		NetworkingState(uint8_t _number, 
		                String _name, 
		                bool _logToSerail,
		                SparkantaConnection* _connection,
		                Serializers* _serializers, 
		                Deserializers* _deserializers,
		                uint32_t _stateTimeoutInMillis,
		                bool _resetInsteadOfDisconnecting);
    
    protected:
        SparkantaConnection* connection;
        Serializers* serializers;
        Deserializers* deserializers;  
        bool resetInsteadOfDisconnecting;
        Pong pong;
        Ack ack;
        
        bool processPing();
        void processDisconnect(Disconnect* disconnect);
        void processDisconnect(uint8_t delayBeforeNextConnectionAttemptInSeconds);   
        
        bool sendReceivedAckIfNeeded(IncomingMessage* incomingMessage);
        
        void disconnect();
};

#endif /* NETWORKING_STATE_H */

// =============================================================

#ifndef OPERATING_STATE_H
#define OPERATING_STATE_H

#include "ByteArray.h"
#include "PinManager.h"
#include "PinConfig.h"

class OperatingState: public NetworkingState
{
	public:
		OperatingState(bool _logToSerail,
		               SparkantaConnection* _connection, 
		               PinsManager* _pinsManager, 
		               PinsConfig* _defaultPinsConfig,
		               Serializers* _serializers, 
		               Deserializers* _deserializers,
		               bool _resetInsteadOfDisconnecting);

        virtual void leavingState();
        
    protected:
	    PinsManager* pinsManager;
	    PinsConfig* defaultPinsConfig;
        
        virtual State* processState();
        virtual State* processIncomingMessage(IncomingMessage* incomingMessage);
};

#endif /* OPERATING_STATE_H */

// =============================================================

#ifndef WAIT_FOR_SERVER_HELLO_STATE_H
#define WAIT_FOR_SERVER_HELLO_STATE_H

class WaitForServerHelloState: public NetworkingState
{
	public:
        OperatingState* operatingState;
        
		WaitForServerHelloState(bool _logToSerail,
		                        SparkantaConnection* _connection, 
		                        uint32_t _waitForServerHelloStateTimeoutInMillis, 
		                        OperatingState* _operatingState,
		                        Serializers* _serializers, 
		                        Deserializers* _deserializers,
		                        bool _resetInsteadOfDisconnecting);
		                        
    protected:
        virtual State* processStateTimeout();
        virtual State* processState();
        virtual State* processIncomingMessage(IncomingMessage* incomingMessage);
};

#endif /* WAIT_FOR_SERVER_HELLO_STATE_H */

// =============================================================

#ifndef WAIT_FOR_GATEWAY_HELLO_STATE_H
#define WAIT_FOR_GATEWAY_HELLO_STATE_H

class WaitForGatewayHelloState: public NetworkingState
{
	public:
	    WaitForServerHelloState* waitingForServerHelloState;
	    
		WaitForGatewayHelloState(bool _logToSerail,
		                         SparkantaConnection* _connection, 
		                         uint32_t _waitForGatewayHelloStateTimeoutInMillis, 
		                         WaitForServerHelloState* _waitingForServerHelloState,
		                         Serializers* _serializers, 
		                         Deserializers* _deserializers,
		                         bool _resetInsteadOfDisconnecting);

    protected:
        virtual State* processStateTimeout();
        virtual State* processState();        
        virtual State* processIncomingMessage(IncomingMessage* incomingMessage);
};

#endif /* WAIT_FOR_GATEWAY_HELLO_STATE_H */

// =============================================================

#ifndef CONNECTING_STATE_H
#define CONNECTING_STATE_H

class ConnectingState: public State
{
	public:
	    uint16_t delayBetweenConnestionReattemptsInMillis;
	    SparkantaConnection* connection;
	    WaitForGatewayHelloState* waitingForGatewayHelloState;
	    PinsConfig* defaultPinsConfig;
	    PinsManager* pinsManager;
        Serializers* serializers;
        Deserializers* deserializers;
        uint32_t lastConnectionAttemptTime;
	    
		ConnectingState(bool _logToSerail, 
		                uint16_t _maxConnectionRetries, 
		                uint16_t _delayBetweenConnestionReattemptsInMillis, 
		                SparkantaConnection* _connection, 
		                WaitForGatewayHelloState* _waitingForGatewayHelloState, 
		                PinsConfig* _defaultPinsConfig,
		                PinsManager* _pinsManager, 
		                Serializers* _serializers,
		                Deserializers* _deserializers);

        virtual void enteringState();
        
    protected:
        virtual State* processStateTimeout();
        virtual State* processState();
};

#endif /* CONNECTING_STATE_H */

// =============================================================
