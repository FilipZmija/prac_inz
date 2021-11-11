#include <Ethernet.h>
#include <SPI.h>
#include <EthernetUdp.h>

byte arduinoMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress arduinoIP(192, 168, 137, 177);//desired IP for Arduino
unsigned int arduinoPort = 8888;     //port of Arduino

IPAddress receiverIP(192, 168, 137, 1);//IP of udp packets receiver
unsigned int receiverPort = 6000;     //port to listen on my PC

EthernetUDP Udp;
int sensorPin = 3; //define sensor pin
float sensorValue;

void setup() {
  Ethernet.begin(arduinoMac,arduinoIP);
  Udp.begin(arduinoPort);
}

void loop() {
  sensorValue = digitalRead(sensorPin);//read sensor value from 0 to 1023 

  Udp.beginPacket(receiverIP, receiverPort); //start udp packet
  Udp.write(sensorValue); //write sensor data to udp packet
  Udp.endPacket();//end packet

  delay(1000);
}
