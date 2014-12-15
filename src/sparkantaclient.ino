#include "TcpConnection.h"
#include "SparkantaConnection.h"
#include "Message.h"
#include "Ping.h"
#include "Pong.h"
#include "Disconnect.h"
#include "DeviceHello.h"
#include "GatewayHello.h"
#include "ServerHello.h"

const String sparkantaGatewayIp = "192.168.2.25";
const uint16_t sparkantaGatewayPort = 8080;

const byte maxConnectionRetriesToSparkantaGateway = 59;
const uint16_t delayBetweenConnestionReattemptsInMillis = 1001;
const uint16_t incomingDataInactivityTimeoutInMillis = 30000;

const String sparkantaIdentyficationString = "SPARKANTA";
const byte softwareVersion = 1;
const byte hardwareVersion = 1;

const byte WAIT_FOR_GATEWAYHELLO_STATE = 1;
const byte WAIT_FOR_SERVERHELLO_STATE = 2;
const byte NORMAL_WORK_STATE = 3;

byte state = WAIT_FOR_GATEWAYHELLO_STATE;

byte messageAsBytes[256];

SparkantaConnection connection(sparkantaGatewayIp, sparkantaGatewayPort, maxConnectionRetriesToSparkantaGateway, delayBetweenConnestionReattemptsInMillis, incomingDataInactivityTimeoutInMillis, sparkantaIdentyficationString, softwareVersion, hardwareVersion);

Ping ping;
Pong pong;
Disconnect disconnect;
GatewayHello gatewayHello;
ServerHello serverHello;

SYSTEM_MODE(AUTOMATIC);

//void setup() { }

void loop() {
  if (connection.connected()) {
      
    switch(state) {

      case WAIT_FOR_GATEWAYHELLO_STATE:
        waitingForGatewayHello();
        break;

      case WAIT_FOR_SERVERHELLO_STATE:
        waitingForServerHello();
        break;
        
      case NORMAL_WORK_STATE:
        delay(10000);
        connection.stop();
        break;

      default: {}
    }
  } else { 
    if(connection.connect()) state = WAIT_FOR_GATEWAYHELLO_STATE;
  }
}

void waitingForGatewayHello() {
    
    int messageLength = connection.readMessage(messageAsBytes);
    
    if(messageLength != -1) {
        if(ping.is(messageAsBytes, messageLength)) {
            pong.send(&connection);
        } else if(disconnect.is(messageAsBytes, messageLength)) {
            uint8_t delayBeforeNextConnectionAttemptInSeconds = disconnect.getDelayBeforeNextConnectionAttemptInSeconds(messageAsBytes);
            connection.stop();
            if(delayBeforeNextConnectionAttemptInSeconds > 0) delay(delayBeforeNextConnectionAttemptInSeconds * 1000);
        } else if(gatewayHello.is(messageAsBytes, messageLength)){
            state = WAIT_FOR_SERVERHELLO_STATE;  
        } else {
            connection.stop();
        }
    }
}

void waitingForServerHello() {
    
    int messageLength = connection.readMessage(messageAsBytes);
    
    if(messageLength != -1) {
        if(ping.is(messageAsBytes, messageLength)) {
            pong.send(&connection);
        } else if(disconnect.is(messageAsBytes, messageLength)) {
            uint8_t delayBeforeNextConnectionAttemptInSeconds = disconnect.getDelayBeforeNextConnectionAttemptInSeconds(messageAsBytes);
            connection.stop();
            if(delayBeforeNextConnectionAttemptInSeconds > 0) delay(delayBeforeNextConnectionAttemptInSeconds * 1000);
        } else if(serverHello.is(messageAsBytes, messageLength)){
            state = NORMAL_WORK_STATE;  
        } else {
            connection.stop();
        }
    }
}
