// =============================================================

#include "State.h"

State::State(uint8_t _number, 
             String _name, 
             bool _logToSerail, 
             uint32_t _stateTimeoutInMillis) {
                 
    number = _number;
    name = _name;
    logToSerail = _logToSerail;
    stateTimeoutInMillis = _stateTimeoutInMillis;
}

void State::enteringState() {
    enteringTimeInMillis = millis();
    
    if(logToSerail) {
        Serial.print("State.enteringState: enterting state \"");
        Serial.print(name);
        Serial.println("\"");
    }
}

void State::leavingState() { 
    if(logToSerail) {
        Serial.print("State.leavingState: leaving state \"");
        Serial.print(name);
        Serial.println("\"");
    }
}

uint32_t State::timeFromEnteringInMillis() {
    return millis() - enteringTimeInMillis;
}

State* State::process() {
    if(stateTimeoutInMillis > 0 && millis() - enteringTimeInMillis > stateTimeoutInMillis) {
        return processStateTimeout();
    } else {
        return processState();
    }
}

State* State::processStateTimeout() {
    
    if(logToSerail) {
        Serial.print("State.processStateTimeout: state \"");
        Serial.print(name);
        Serial.print("\" timeout (");
        Serial.print(stateTimeoutInMillis);
        Serial.println(" ms)");
    }
    
    return NULL;
}

// =============================================================

NetworkingState::NetworkingState(uint8_t _number, 
        		                 String _name, 
        		                 bool _logToSerail,
        		                 SparkantaConnection* _connection,
        		                 Serializers* _serializers, 
        		                 Deserializers* _deserializers,
        		                 uint32_t _stateTimeoutInMillis,
        		                 bool _resetInsteadOfDisconnecting): State(_number , _name, _logToSerail, _stateTimeoutInMillis) {

    connection = _connection;
    serializers = _serializers;
    deserializers = _deserializers;
    resetInsteadOfDisconnecting = _resetInsteadOfDisconnecting;
}

bool NetworkingState::processPing() {
    return serializers->serialize(&pong);
}

void NetworkingState::processDisconnect(Disconnect* disconnect) {
    processDisconnect(disconnect->delayBeforeNextConnectionAttemptInSeconds);
}

void NetworkingState::processDisconnect(uint8_t delayBeforeNextConnectionAttemptInSeconds) {
    if(logToSerail) {
        Serial.print("NetworkingState.processDisconnect: received Disconnect, will close connection and wait ");    
        Serial.print(delayBeforeNextConnectionAttemptInSeconds);    
        Serial.println(" seconds before continue");    
    }
    
    disconnect();
    delay(delayBeforeNextConnectionAttemptInSeconds * 1000);
}

bool NetworkingState::sendReceivedAckIfNeeded(IncomingMessage* incomingMessage) {
    if(incomingMessage->ackType == ReceivedAckType) {
        ack.ackMessageCode = incomingMessage->messageCode;
        ack.ackType = incomingMessage->ackType;
        return serializers->serialize(&ack);
    } else {
        return true;
    }
}

void NetworkingState::disconnect() {
    if(resetInsteadOfDisconnecting) {
        if(logToSerail) {
            Serial.println("NetworkingState.disconnect: resetting Spark Core (instead of disconnecting)!");
            delay(500);
        }
        System.reset();
    } else {
        if(logToSerail) Serial.println("NetworkingState.disconnect: closing connection");    
        connection->stop();
    }
}

// =============================================================

OperatingState::OperatingState(bool _logToSerail,
                               SparkantaConnection* _connection, 
                               PinsManager* _pinsManager, 
                               PinsConfig* _defaultPinsConfig,
        		               Serializers* _serializers, 
		                       Deserializers* _deserializers,
		                       bool _resetInsteadOfDisconnecting): NetworkingState(4 , "OPERATING", _logToSerail, _connection, _serializers, _deserializers, 0, _resetInsteadOfDisconnecting) {
                		           
    pinsManager = _pinsManager;
    defaultPinsConfig = _defaultPinsConfig;
}

void OperatingState::leavingState() { 
    pinsManager->configurePins(defaultPinsConfig);
    State::leavingState();
}

State* OperatingState::processState() {
    
    if(connection->isIncomingTrafficInactivityExceeded()) {
    
        if(logToSerail) Serial.println("OperatingState.process: incoming trafic inactivity timeout");
        disconnect();
        return connectingState;
        
    } else {
    
        if(connection->readMessage()) {
                
            IncomingMessage* incomingMessage = deserializers->deserialize();
                
            if(incomingMessage == NULL) {
                if(logToSerail) Serial.println("OperatingState.process: received message could not be deserialized");    
                disconnect();
                return connectingState;   
            } else {
                sendReceivedAckIfNeeded(incomingMessage); // TODO: what if serialization will fail?
                return processIncomingMessage(incomingMessage); 
            }
        } else {
            return NULL;
        }
    }
    
    if(logToSerail) Serial.println("!!!!!!!!!!!!!!!! OperatingState.process: you should never see this message !!!!!!!!!!!!!!!!");  
    disconnect();
    return connectingState;    
}

State* OperatingState::processIncomingMessage(IncomingMessage* incomingMessage) {
    
    /*if(logToSerail) {
        Serial.print("OperatingState.process: trying to match received message with code ");    
        Serial.println(incomingMessage->messageCode);    
    }*/
            
    switch(incomingMessage->messageCode) {
                        
        case(Ping::staticMessageCode): {
            processPing(); // TODO: what if serialization will fail?
            break;
        }
                    
        case(Disconnect::staticMessageCode): {
            processDisconnect(static_cast<Disconnect*>(incomingMessage));
            return connectingState; 
        }
                        
        case(PinConfiguration::staticMessageCode): {
            PinConfiguration* pinConfiguration = static_cast<PinConfiguration*>(incomingMessage);
                            
            if(logToSerail) Serial.println("OperatingState.process: received PinConfiguration, configuring pins");
            pinsManager->configurePins(&pinConfiguration->pinsConfig);       
            break;
        }
                        
        case(SetDigitalPinValue::staticMessageCode): {
            SetDigitalPinValue* setDigitalPinValue = static_cast<SetDigitalPinValue*>(incomingMessage);
            pinsManager->setPinValue(setDigitalPinValue);
            break;
        }
                   
        case(SetAnalogPinValue::staticMessageCode): {
            SetAnalogPinValue* setAnalogPinValue = static_cast<SetAnalogPinValue*>(incomingMessage);
            pinsManager->setPinValue(setAnalogPinValue);
            break;
        }
                        
        default: {
            if(logToSerail) {
                Serial.print("OperatingState.process: received message ");    
                Serial.print(incomingMessage->messageCode);    
                Serial.println(" that is not supported in this state");      
            }
                    
            disconnect();
            return connectingState; 
                    
        }
    }  
                
    if(connection->connected()) pinsManager->process();
                        
    return NULL;
}
// =============================================================

WaitForServerHelloState::WaitForServerHelloState(bool _logToSerail,
                                                 SparkantaConnection* _connection, 
                                                 uint32_t _waitForServerHelloStateTimeoutInMillis, 
                                                 OperatingState* _operatingState,
                		                         Serializers* _serializers, 
		                                         Deserializers* _deserializers,
		                                         bool _resetInsteadOfDisconnecting): NetworkingState(3 , "WAITING FOR SERVER_HELLO", _logToSerail, _connection, _serializers, _deserializers, _waitForServerHelloStateTimeoutInMillis, _resetInsteadOfDisconnecting) {
		                                             
    operatingState = _operatingState;
}

State* WaitForServerHelloState::processStateTimeout() {
    State::processStateTimeout();
    if(logToSerail) Serial.println("WaitForServerHelloState.processStateTimeout: timeout while waiting for SERVER_HELLO");
    disconnect();
    return connectingState;    
}

State* WaitForServerHelloState::processState() {
    if(connection->readMessage()) {
            
        IncomingMessage* incomingMessage = deserializers->deserialize();
            
        if(incomingMessage == NULL) {
            if(logToSerail) Serial.println("WaitForServerHelloState.process: received message could not be deserialized");    
            disconnect();
            return connectingState;   
        } else {            
            sendReceivedAckIfNeeded(incomingMessage); // TODO: what if serialization will fail?
            return processIncomingMessage(incomingMessage);
        }

    } else {
        return NULL; 
    }
}

State* WaitForServerHelloState::processIncomingMessage(IncomingMessage* incomingMessage) {
    switch(incomingMessage->messageCode) {
                
        case(Ping::staticMessageCode): {
            processPing(); // TODO: what if serialization will fail?
            return NULL; 
        }
                    
        case(Disconnect::staticMessageCode): {
            processDisconnect(static_cast<Disconnect*>(incomingMessage));
            return connectingState; 
        }
                    
        case(ServerHello::staticMessageCode): {
            return operatingState; 
        }
                    
        default: {
            if(logToSerail) {
                Serial.print("WaitForServerHelloState.process: received message ");    
                Serial.print(incomingMessage->messageCode);    
                Serial.println(" that is not supported in this state");      
            }
                        
            disconnect();
            return connectingState;                     
        }
    }    
}

// =============================================================

WaitForGatewayHelloState::WaitForGatewayHelloState(bool _logToSerail,
                                                   SparkantaConnection* _connection, 
                                                   uint32_t _waitForGatewayHelloStateTimeoutInMillis, 
                                                   WaitForServerHelloState* _waitingForServerHelloState,
                		                           Serializers* _serializers, 
		                                           Deserializers* _deserializers,
		                                           bool _resetInsteadOfDisconnecting): NetworkingState(2 , "WAITING FOR GATEWAY_HELLO", _logToSerail, _connection, _serializers, _deserializers, _waitForGatewayHelloStateTimeoutInMillis, _resetInsteadOfDisconnecting) {

    waitingForServerHelloState = _waitingForServerHelloState;
}

State* WaitForGatewayHelloState::processStateTimeout() {
    State::processStateTimeout();
    if(logToSerail) Serial.println("WaitForGatewayHelloState.processStateTimeout: timeout while waiting for GATEWAY_HELLO");
    disconnect();
    return connectingState;   
}

State* WaitForGatewayHelloState::processState() {
    if(connection->readMessage()) {
            
        IncomingMessage* incomingMessage = deserializers->deserialize();
            
        if(incomingMessage == NULL) {
            if(logToSerail) Serial.println("WaitForGatewayHelloState.process: received message could not be deserialized");    
            disconnect();
            return connectingState;   
        } else { 
            sendReceivedAckIfNeeded(incomingMessage); // TODO: what if serialization will fail?
            return processIncomingMessage(incomingMessage);
        }
    } else {
        return NULL; 
    }
}

State* WaitForGatewayHelloState::processIncomingMessage(IncomingMessage* incomingMessage) {
    switch(incomingMessage->messageCode) {
                    
        case(Ping::staticMessageCode): {
            processPing(); // TODO: what if serialization will fail?
            return NULL; 
        }
                    
        case(Disconnect::staticMessageCode): {
            processDisconnect(static_cast<Disconnect*>(incomingMessage));
            return connectingState; 
        }
                    
        case(GatewayHello::staticMessageCode): {
            return waitingForServerHelloState; 
        }
                    
        default: {
            if(logToSerail) {
                Serial.print("WaitForGatewayHelloState.process: received message ");    
                Serial.print(incomingMessage->messageCode);    
                Serial.println(" that is not supported in this state");    
            }
                        
            disconnect();
            return connectingState;                     
        }
    }
}

// =============================================================

ConnectingState::ConnectingState(bool _logToSerail, 
                                 uint16_t _maxConnectionRetries, 
                                 uint16_t _delayBetweenConnestionReattemptsInMillis, 
                                 SparkantaConnection* _connection, 
                                 WaitForGatewayHelloState* _waitingForGatewayHelloState,
                                 PinsConfig* _defaultPinsConfig,
                                 PinsManager* _pinsManager, 
                                 Serializers* _serializers,
                                 Deserializers* _deserializers): State(1 , "CONNECTING", _logToSerail, _maxConnectionRetries * _delayBetweenConnestionReattemptsInMillis) {
                                     
    delayBetweenConnestionReattemptsInMillis = _delayBetweenConnestionReattemptsInMillis;
    
    connection = _connection;
    waitingForGatewayHelloState = _waitingForGatewayHelloState;
    defaultPinsConfig = _defaultPinsConfig;
    pinsManager = _pinsManager;
    serializers = _serializers;
    deserializers = _deserializers;
    
    waitingForGatewayHelloState->connectingState = this;
    waitingForGatewayHelloState->waitingForServerHelloState->connectingState = this;
    waitingForGatewayHelloState->waitingForServerHelloState->operatingState->connectingState = this;
    
    lastConnectionAttemptTime = 0;
}

void ConnectingState::enteringState() {
    State::enteringState();
    
    lastConnectionAttemptTime = 0;

    serializers->messageNumber = 0;
    deserializers->messageNumber = 0;
    
    /*if(connection->available() > 0) {
        
        if(logToSerail && connection->available() > 0) {
            Serial.print("ConnectingState.enteringState: before entering this state read out all ");
            Serial.print(connection->available());
            Serial.println(" available bytes and perform flush");
        }
    
        uint16_t i = 0;
        int readByte = 0;
        while(connection->available() > 0 && readByte >= 0 && i < 1024 * 8) {
            readByte = connection->read();
            i++;
        }
        
        connection->flush();
    }*/    
    
    pinsManager->configurePins(defaultPinsConfig);
}

State* ConnectingState::processStateTimeout() {
    State::processStateTimeout();
    
    if(logToSerail) {
        Serial.println("ConnectingState.processStateTimeout: timeout while connecting, resetting Spark Core!");
        delay(500);
    }
    
    System.reset();
    return NULL;   
}

State* ConnectingState::processState() {
    if(connection->connected()) {
        return waitingForGatewayHelloState;   
    } else {
        if(delayBetweenConnestionReattemptsInMillis <= 0 || millis() - lastConnectionAttemptTime > delayBetweenConnestionReattemptsInMillis) {
            lastConnectionAttemptTime = millis();
            
            if(logToSerail) Serial.println("ConnectingState.process: connection attempt...");
            
            if(connection->connect()) {
                
                if(logToSerail) Serial.println("ConnectingState.process: connected!");
                
                DeviceHello deviceHello;
                serializers->serialize(&deviceHello); // TODO: what if serialization will fail?

                return waitingForGatewayHelloState;   
            } else {
                return NULL;      
            }
        } else {
            return NULL;      
        }
    }
}

// =============================================================
