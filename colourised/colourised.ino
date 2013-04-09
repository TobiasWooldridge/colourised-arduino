// Copyright 2013 Tobias Wooldridge
// Protocol originally based upon https://github.com/iKenndac/Arduino-Dioder-Playground

#include <Tlc5940.h>

// Configure LEDs
const int procolPreamble = 0xBA;

const int instructionVersion = 0x00;
const int instructionSetColours = 0xBE;
const int instructionPowerDown = 0xFF;

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
    
    // Serial.write("Matched preamble\n");
    byte instruction = Serial.read();
    unsigned int messageLength = SerialReadUInt16();

    switch (instruction) {
      case instructionSetColours: readColours(messageLength); break;
      case instructionVersion: sendVersion(); break;
      case instructionPowerDown: powerDown(); break;
      default: ;
    }
    
    // Serial.write("Finished instruction\n");
  }
  
  if (lastMessageHeader() > idleTimeout)
  {
    powerDown();
  }
}


long lastMessageHeader() {
  return millis() - messageStarted;
}

// todo: remove hard-coded 3 in following function
void readColours(unsigned int messageLength) {
  unsigned int specifiedChannels = messageLength/3;
  Serial.print("Specified channels: ");
  Serial.print(specifiedChannels);
  Serial.print("\n");
  
  for (unsigned int i = 0; i < specifiedChannels; i++) {
    while (Serial.available() < 3) {
    }    
    
    // Note an int is used to hold just a byte below. This means I won't have to cast each variable below to bit-shift it   
    // todo: change this to an array
    byte a = Serial.read();
    byte b = Serial.read(); // shared byte
    byte c = Serial.read();
    
    unsigned int channel = (((int)a) << 8) + ((b & 0xF0) >> 4);
    unsigned int value = (((int)(b & 0x0F)) << 8) + c;
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

