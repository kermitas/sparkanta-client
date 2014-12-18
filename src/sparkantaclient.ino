#include "ByteArray.h"
#include "TcpConnection.h"
#include "SparkantaConnection.h"
#include "Message.h"
#include "PinConfig.h"
#include "PinManager.h"

// DEFs ========================================================

#define useSerial false

// VALs ========================================================

const String sparkantaGatewayIp = "192.168.2.25";
const uint16_t sparkantaGatewayPort = 8080;

const uint8_t maxConnectionRetriesToSparkantaGateway = 59;
const uint16_t delayBetweenConnestionReattemptsInMillis = 1001;

const uint32_t incomingMessageInactivityTimeoutInMillis = 20 * 1000;

const String sparkantaIdentyficationString = "SPARKANTA";
const uint8_t softwareVersion = 1;
const uint8_t hardwareVersion = 1;

const uint8_t waitForGatewayHelloStateNumber = 1;
const uint8_t waitForServerHelloStateNumber = 2;
const uint8_t normalOperatingStateNumber = 3;

const uint32_t waitForGatewayHelloStateTimeoutInMillis = 10 * 1000;
const uint32_t waitForServerHelloStateTimeoutInMillis = 10 * 1000;

const uint16_t incomingByteBufferSize = 260;
const uint16_t outgoingByteBufferSize = 550;

// VARs ========================================================

uint8_t state = 0;

uint8_t incomingByteBufferByteArray[incomingByteBufferSize];
uint8_t outgomingByteBufferByteArray[outgoingByteBufferSize];

uint32_t stateEnteringTimeInMillis = millis();

// =============================================================

SparkantaConnection connection(sparkantaGatewayIp, sparkantaGatewayPort, maxConnectionRetriesToSparkantaGateway, delayBetweenConnestionReattemptsInMillis, incomingMessageInactivityTimeoutInMillis, sparkantaIdentyficationString, softwareVersion, hardwareVersion);

Ping ping;
Pong pong;
Disconnect disconnect;
GatewayHello gatewayHello;
ServerHello serverHello;

PinsConfig pinsConfig;
PinsManager pinsManager;

PinConfiguration pinConfiguration;

DigitalPinValue digitalPinValue;
AnalogPinValue analogPinValue;

IndexedByteArray incomingByteBuffer(incomingByteBufferByteArray, incomingByteBufferSize, 0);
IndexedByteArray outgomingByteBuffer(outgomingByteBufferByteArray, outgoingByteBufferSize, 0);

// INIT ========================================================

SYSTEM_MODE(AUTOMATIC);
//SYSTEM_MODE(MANUAL);

void setup() {
    
    if(useSerial) {
        Serial.begin(9600);
        Serial.println("============= START OF SPARKANTA CLIENT ================");
    }
    
    pinsManager.configurePins(&pinsConfig);
}

// OPER ========================================================

void loop() {
    connection.process();
    
    if (connection.connected()) {
      
        switch(state) {

            case normalOperatingStateNumber:
                normalOperating();
                break;
                
            case waitForGatewayHelloStateNumber:
                waitingForGatewayHello();
                break;
    
            case waitForServerHelloStateNumber:
                waitingForServerHello();
                break;
    
            default: {}
        }
        
        //sendOutgointData();
        
    } else { 
        if(useSerial) Serial.println("connecting...");
        
        if(connection.connect(&outgomingByteBuffer)) {
            
            if(useSerial) Serial.println("connected!");
            
            //sendOutgointData();
            
            gotoState(waitForGatewayHelloStateNumber);
        }
    }
    
    if (connection.connected()) sendOutgointData();
}

void gotoState(byte stateNumber) {
    state = stateNumber;
    stateEnteringTimeInMillis = millis();
    
    if(useSerial) {
        Serial.print("going to state ");
        Serial.println(state);
    }
}

void disconnectAndWait(uint8_t delayBeforeNextConnectionAttemptInSeconds) {
    connection.stop();
    
    if(delayBeforeNextConnectionAttemptInSeconds > 0) {
        Spark.process();
        delay(delayBeforeNextConnectionAttemptInSeconds * 1000);
    }
}

/*void blink(uint8_t count) {
    pinMode(D7, OUTPUT);
    
    for (uint8_t i = 0; i < count; i++) {
        digitalWrite(D7, HIGH);
        
        Spark.process();
        delay(150);
        Spark.process();
        
        digitalWrite(D7, LOW);
        
        Spark.process();
        delay(150);     
        Spark.process();
    }
}*/

void sendOutgointData() {
    if(outgomingByteBuffer.containsData()) {
        uint16_t wroteBytesCount = connection.write(outgomingByteBuffer.bytes, outgomingByteBuffer.index);
            
        if(useSerial) {
            Serial.print("sending ");
            Serial.print(outgomingByteBuffer.index);
            Serial.print(" bytes; ");
            Serial.print(wroteBytesCount);
            Serial.println(" bytes was send");
        }
            
        outgomingByteBuffer.index = 0;
    }
}

void waitingForGatewayHello() {
    
    if(waitForGatewayHelloStateTimeoutInMillis > 0 && millis() - stateEnteringTimeInMillis > waitForGatewayHelloStateTimeoutInMillis) {
        if(useSerial) Serial.println("timeout while waiting for GATEWAY HELLO");
        connection.stop();
    } else {
        
        incomingByteBuffer.index = 0;
        bool receivedMessage = connection.readMessage(&incomingByteBuffer);
        incomingByteBuffer.index = 0;
        
        while(receivedMessage && connection.connected()) {
            
            if(ping.is(&incomingByteBuffer)) {
                pong.serialize(&outgomingByteBuffer);
            } else if(disconnect.is(&incomingByteBuffer)) {
                uint8_t delayBeforeNextConnectionAttemptInSeconds = disconnect.getDelayBeforeNextConnectionAttemptInSeconds(&incomingByteBuffer);
                disconnectAndWait(delayBeforeNextConnectionAttemptInSeconds);
            } else if(gatewayHello.is(&incomingByteBuffer)) {
                if(useSerial) Serial.println("received GATEWAY HELLO");
                gotoState(waitForServerHelloStateNumber);
            } else {
                if(useSerial) Serial.println("received unsupported message while waiting for GATEWAY HELLO");
                connection.stop();
            }
            
            if(state == waitForGatewayHelloStateNumber) {
                incomingByteBuffer.index = 0;
                receivedMessage = connection.readMessage(&incomingByteBuffer);
                incomingByteBuffer.index = 0;
            } else {
                receivedMessage = false;
            }
        }
    }
}

void waitingForServerHello() {

    if(waitForServerHelloStateTimeoutInMillis > 0 && millis() - stateEnteringTimeInMillis > waitForServerHelloStateTimeoutInMillis) {
        if(useSerial) Serial.println("timeout while waiting for SERVER HELLO");
        connection.stop();
    } else {
        
        incomingByteBuffer.index = 0;
        bool receivedMessage = connection.readMessage(&incomingByteBuffer);
        incomingByteBuffer.index = 0;
        
        while(receivedMessage && connection.connected()) {
            
            if(ping.is(&incomingByteBuffer)) {
                pong.serialize(&outgomingByteBuffer);
            } else if(disconnect.is(&incomingByteBuffer)) {
                uint8_t delayBeforeNextConnectionAttemptInSeconds = disconnect.getDelayBeforeNextConnectionAttemptInSeconds(&incomingByteBuffer);
                disconnectAndWait(delayBeforeNextConnectionAttemptInSeconds);
            } else if(serverHello.is(&incomingByteBuffer)) {
                if(useSerial) Serial.println("received SERVER HELLO");
                gotoState(normalOperatingStateNumber);
            } else {
                if(useSerial) Serial.println("received unsupported message while waiting for SERVER HELLO");
                connection.stop();
            }
            
            if(state == waitForServerHelloStateNumber) {
                incomingByteBuffer.index = 0;
                receivedMessage = connection.readMessage(&incomingByteBuffer);
                incomingByteBuffer.index = 0;
            } else {
                receivedMessage = false;
            }
        }        
    }
}

void normalOperating() {

    // TODO if no traffic for X then send Ping

    // ----- eventually receiving (and parsing) the data -----

    incomingByteBuffer.index = 0;
    bool receivedMessage = connection.readMessage(&incomingByteBuffer);
    incomingByteBuffer.index = 0;
        
    while(receivedMessage && connection.connected()) {
            
        if(ping.is(&incomingByteBuffer)) {
            pong.serialize(&outgomingByteBuffer);
        } else if(pong.is(&incomingByteBuffer)) {
            // on Pong do nothing 
        } else if(digitalPinValue.is(&incomingByteBuffer)) {
            
            uint8_t pinNumber = digitalPinValue.getPinNumber(&incomingByteBuffer);
            
            if(useSerial) {
                Serial.print("received digitalPinValue for pin ");
                Serial.println(pinNumber);
            }
            
            if(pinsConfig.digitalPins[pinNumber].pinWorkMode == outputPinWorkMode) {
                uint8_t pinValue = digitalPinValue.getPinValue(&incomingByteBuffer);
                
                if(useSerial) {
                    Serial.print("setting (digitalPinValue) to ");
                    Serial.println(pinValue);
                }
            
                digitalWrite(pinNumber, pinValue);   
            }
            
            digitalPinValue.consume(&incomingByteBuffer);
            
        } else if(analogPinValue.is(&incomingByteBuffer)) {
            
            uint8_t pinNumber = analogPinValue.getPinNumber(&incomingByteBuffer);
            
            if(useSerial) {
                Serial.print("received analogPinValue for pin ");
                Serial.println(pinNumber);
            }
            
            if(pinsConfig.analogPins[pinNumber].pinWorkMode == outputPinWorkMode) { 
                uint8_t pinValue = analogPinValue.getPinValue(&incomingByteBuffer);
                
                if(useSerial) {
                    Serial.print("setting (analogPinValue) to ");
                    Serial.println(pinValue);
                }
                
                analogWrite(pinNumber + 10, pinValue);
            }
            
            analogPinValue.consume(&incomingByteBuffer);
            
        } else if(pinConfiguration.is(&incomingByteBuffer)) {
            if(useSerial) Serial.println("received PinConfiguration: parsing received bytes...");
            pinConfiguration.parse(&pinsConfig, &incomingByteBuffer);
            if(useSerial) Serial.println("received PinConfiguration: configuring pins...");
            pinsManager.configurePins(&pinsConfig);
        } else if(disconnect.is(&incomingByteBuffer)) {
            uint8_t delayBeforeNextConnectionAttemptInSeconds = disconnect.getDelayBeforeNextConnectionAttemptInSeconds(&incomingByteBuffer);
            disconnectAndWait(delayBeforeNextConnectionAttemptInSeconds);
        } else {
            if(useSerial) Serial.println("received unsupported message while normal operating");
            connection.stop();
        }
        
        if(state == normalOperatingStateNumber) {
            incomingByteBuffer.index = 0;
            receivedMessage = connection.readMessage(&incomingByteBuffer);
            incomingByteBuffer.index = 0;
        } else {
            receivedMessage = false;
        }
    } 
    
    // ----- collect data from input pins -----
    
    if(connection.connected()) pinsManager.process(&pinsConfig, &outgomingByteBuffer);
}
