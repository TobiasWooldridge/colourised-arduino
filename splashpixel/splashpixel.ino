// Copyright 2013 Tobias Wooldridge
// Protocol originally based upon https://github.com/iKenndac/Arduino-Dioder-Playground

#include <Tlc5940.h>

// Configure LEDs
const int procolPreamble = 0xBA;

const int protocolInstructionVersion = 0x00;
const int protocolInstructionSetColours = 0xBE;
const int protocolInstructionPowerDown = 0xBF;

const int protocolHeaderLength = 4;
const int protocolBodyLength = 12;
const int protocolChecksumLength = 1;

const long idleTimeout = 120000;

int channelCount = 16;

byte receivedMessage[protocolBodyLength];

long steppedUntil = millis();

long messageStarted = 0;


void setup() {
  Serial.begin(115200);
  Tlc.init();  
}


unsigned int SerialReadUInt16() {
  while (Serial.available() < 2) {}
  unsigned int in = Serial.read();
  in = (in << 8) + Serial.read();

//  char buf[2];
//  Serial.write("0x");
//  Serial.write(itoa(in, buf, 16));
//  Serial.write("\n");
  
  return in;  
}


void loop() {
  while (Serial.available() >= protocolHeaderLength) {
    messageStarted = millis();

    // Sync
    if (Serial.read() != procolPreamble) {
      continue;
    }
    
    Serial.write("Matched preamble\n");

    byte instruction = Serial.read();
    unsigned int messageLength = SerialReadUInt16();

    switch (instruction) {
      case protocolInstructionSetColours: readColours(messageLength); break;
//      case protocolInstructionVersion: sendVersion(); break;
//      case protocolInstructionPowerDown: powerDown(); break;
      default: ;
    }
    
    Serial.write("Finished instruction\n");
  }
  
  if (lastMessageHeader() > idleTimeout)
  {
    powerDown();
  }
}


long lastMessageHeader() {
  return millis() - messageStarted;
}

void readColours(unsigned int messageLength) {
  Serial.write("readColours\n");
  
  unsigned int specifiedChannels = messageLength/4;
  
  for (unsigned int i = 0; i < specifiedChannels; i++) {
    unsigned int channel =   SerialReadUInt16();
    unsigned int value =   SerialReadUInt16();
    set(channel, value);
  }

  Tlc.update();
}

void set (unsigned int channel, unsigned int value) {
    Tlc.set(channel, 4095 - value);
}

void powerDown() {
  for (int i = 0; i < channelCount; i++) {
    set(i, 0);
  }

  Tlc.update();
}

void sendVersion() {
  Serial.write(0x00); // Model
  Serial.write(0x00); // Major version
  Serial.write(0x01); // Minor version
}

