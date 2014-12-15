#include "DeviceHello.h"

DeviceHello::DeviceHello(): Message(1) { }

void DeviceHello::send(TCPClient* tcpClient) {
    String sparkDeviceId = Spark.deviceID();
    
    tcpClient->write(3 + sparkDeviceId.length()); // length
    tcpClient->write(messageCode);
    tcpClient->write(1); // serialization version
    tcpClient->write(sparkDeviceId.length());
    tcpClient->write(reinterpret_cast<const uint8_t*>(sparkDeviceId.c_str()), sparkDeviceId.length());
    
    tcpClient->flush();
}

bool DeviceHello::is(byte messageAsBytes[], uint8_t length) {
    return false; // this message can be only send from device to server
}

DeviceHello::~DeviceHello() { }
