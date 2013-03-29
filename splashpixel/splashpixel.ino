// Copyright 2013 Tobias Wooldridge
// Protocol originally based upon https://github.com/iKenndac/Arduino-Dioder-Playground

#include "TLC5940.h"

// Configure LEDs
const int procolPreamble = 0xBA;

const int protocolInstructionVersion = 0x00;
const int protocolInstructionSetColours = 0xBE;
const int protocolInstructionPowerDown = 0xBF;

const int protocolHeaderLength = 2;
const int protocolBodyLength = 12;
const int protocolChecksumLength = 1;

const long idleTimeout = 120000;

int channelCount = 16;

byte receivedMessage[protocolBodyLength];

long steppedUntil = millis();

long messageStarted = millis();


void setup() {
  Serial.begin(115200);
}

int SerialReadInt16() {
	(Serial.read() << 8) + Serial.read();
}


void loop() {
	while (Serial.available() >= protocolHeaderLength) {
		messageStarted = millis();

		// Sync
		if (Serial.read() != procolPreamble)
			continue;

		byte instruction = Serial.read();

		int messageLength = SerialReadInt16();

		switch (instruction) {
			case protocolInstructionSetColours: readColours(messageLength); break;
			case protocolInstructionVersion: sendVersion(); break;
			case protocolInstructionPowerDown: powerDown(); break;
			
			default: return;
		}
	}
	
	if (lastMessageHeader() > idleTimeout)
	{
		powerDown();
	}
}


long lastMessageHeader() {
	return millis() - messageStarted;
}

void readColours(int messageLength) {
	if (messageLength % 4 != 0) {
		return; // TODO add some clever error handling
	}
	int specifiedChannels = messageLength/4;

	for (int i = 0; i < specifiedChannels; i++) {
		Tlc.set(SerialReadInt16(), SerialReadInt16());
	}
}

void powerDown() {
	for (int i = 0; i < channelCount; i++) {
		Tlc.set(i, 0);
	}
}

void sendVersion() {
	Serial.write(0x00); // Model
	Serial.write(0x00); // Major version
	Serial.write(0x01); // Minor version
}

