// INCLUDEs ====================================================

#include "State.h"
#include "ByteArray.h"
#include "TcpConnection.h"
#include "SparkantaConnection.h"
#include "Message.h"
#include "PinConfig.h"
#include "PinManager.h"

// DEFs ========================================================

#define logToSerail true
#define logToSerailFromConnection logToSerail
#define systemMode AUTOMATIC // AUTOMATIC or MANUAL

#define resetInsteadOfDisconnecting false
#define deviceUniqieName Spark.deviceID()

#define serialSpeed 19200
// CONSTs ======================================================

const String sparkantaGatewayIp = "192.168.2.25";
const uint16_t sparkantaGatewayPort = 8080;

const String sparkantaIdentyficationString = "SPARKANTA";
const uint8_t softwareVersion = 1;
const uint8_t hardwareVersion = 1;

const uint16_t delayBetweenConnestionReattemptsInMillis = 1000;
const uint16_t maxConnectionRetriesToSparkantaGateway = delayBetweenConnestionReattemptsInMillis > 0 ? (30 * 1000) / delayBetweenConnestionReattemptsInMillis : 0; // after this will srestart Spark

const uint32_t warmingUpWiFiTimeoutInMillis = 10 * 1000;

const uint32_t incomingMessageInactivityTimeoutInMillis = 3 * 1000; // after this time will disconnect and go to "connecting" state

const uint32_t waitForGatewayHelloStateTimeoutInMillis = 2 * 1000; // after this time will disconnect and go to "connecting" state
const uint32_t waitForServerHelloStateTimeoutInMillis = 2 * 1000; // after this time will disconnect and go to "connecting" state

const uint16_t incomingByteBufferSize = 260;
const uint16_t outgoingByteBufferSize = 550;

const uint8_t pinsManagerShouldNotPutIntoOutgoingBufferBytesMoreThan = 45;

// FIELDs ======================================================

uint8_t incomingByteBufferByteArray[incomingByteBufferSize];
uint8_t outgmingByteBufferByteArray[outgoingByteBufferSize];

IndexedByteArray incomingByteBuffer(incomingByteBufferByteArray, incomingByteBufferSize, 0);
IndexedByteArray outgoingByteBuffer(outgmingByteBufferByteArray, outgoingByteBufferSize, 0);

SparkantaConnection connection(logToSerailFromConnection, 
                               sparkantaGatewayIp, 
                               sparkantaGatewayPort, 
                               deviceUniqieName,
                               incomingMessageInactivityTimeoutInMillis, 
                               sparkantaIdentyficationString, 
                               softwareVersion, 
                               hardwareVersion,
                               &incomingByteBuffer, 
                               &outgoingByteBuffer);

Serializers serializers(logToSerail, &outgoingByteBuffer);
Deserializers deserializers(logToSerail, &incomingByteBuffer);

PinsConfig defaultPinsConfig;
PinsConfig pinsConfig;
PinsManager pinsManager(&outgoingByteBuffer, &serializers, pinsManagerShouldNotPutIntoOutgoingBufferBytesMoreThan);

OperatingState operatingState(logToSerail, 
                              &connection, 
                              &pinsManager,
                              &defaultPinsConfig,
                              &serializers, 
                              &deserializers,
                              resetInsteadOfDisconnecting);

WaitForServerHelloState waitingForServerHelloState(logToSerail, 
                                                   &connection, 
                                                   waitForServerHelloStateTimeoutInMillis, 
                                                   &operatingState, 
                                                   &serializers, 
                                                   &deserializers,
                                                   resetInsteadOfDisconnecting);
                                                   
WaitForGatewayHelloState waitingForGatewayHelloState(logToSerail, 
                                                     &connection, 
                                                     waitForGatewayHelloStateTimeoutInMillis, 
                                                     &waitingForServerHelloState, 
                                                     &serializers, 
                                                     &deserializers,
                                                     resetInsteadOfDisconnecting);
                                                     
ConnectingState connectingState(logToSerail, 
                                maxConnectionRetriesToSparkantaGateway, 
                                delayBetweenConnestionReattemptsInMillis, 
                                &connection, 
                                &waitingForGatewayHelloState, 
                                &defaultPinsConfig,
                                &pinsManager, 
                                &serializers,
                                &deserializers);

State* currentState = &connectingState;

// INIT ========================================================

SYSTEM_MODE(systemMode);

void setup() {

    if(logToSerail) {
        Serial.begin(serialSpeed);
        //delay(10000);
        Serial.println("============= START OF SPARKANTA CLIENT ================");
    }

    if(systemMode == MANUAL) warmUpWiFi();
    
    currentState->enteringState();
}

// OPER ========================================================

void loop() {
    
    if(systemMode == MANUAL && !WiFi.ready()) {
        if(logToSerail) {
            Serial.println("WiFi is not ready, resetting Spark Core!");
            delay(500);
        }
        System.reset();
    }

    if(!connection.connected()) gotoState(&connectingState);
    
    State* nextState = currentState->process();
    connection.sendMessages();
    if(nextState != NULL) gotoState(nextState);
}

void gotoState(void* _nextState) {
    
    State* nextState = static_cast<State*>(_nextState);
    
    if(nextState != currentState) {
        if(logToSerail) {
            Serial.print("changing state \"");
            Serial.print(currentState->name);
            Serial.print("\" --> \"");
            Serial.print(nextState->name);            
            Serial.println("\"");            
        }
            
        currentState->leavingState();
        nextState->enteringState();
        currentState = nextState;
    }
}

void warmUpWiFi() {
    uint32_t warmingUpWiFiStartTime = millis();
        
    if(logToSerail) Serial.println("Turning on WiFi...");
    WiFi.on();
    if(logToSerail) Serial.println("Turning on WiFi...done");
        
    WiFi.connect();
        
    if(logToSerail) Serial.print("Connecting to WiFi...");    
    while(WiFi.connecting()) {
        if(logToSerail) Serial.print(".");     
        if(millis() - warmingUpWiFiStartTime > warmingUpWiFiTimeoutInMillis) {
            if(logToSerail) {
                Serial.println("Warming up WiFi timeout reached, resetting Spark Core!");
                delay(500);
                System.reset();
            }
        }
        delay(200);
    }
    if(logToSerail) Serial.println("done");
        
    if(logToSerail) Serial.print("Waiting for WiFi do be ready...");
    while(!WiFi.ready()) {
        if(logToSerail) Serial.print(".");
        if(millis() - warmingUpWiFiStartTime > warmingUpWiFiTimeoutInMillis) {
            if(logToSerail) {
                Serial.println("Warming up WiFi timeout reached, resetting Spark Core!");
                delay(500);
                System.reset();
            }
        }            
        delay(200);
    }
    if(logToSerail) Serial.println("done");    
}
