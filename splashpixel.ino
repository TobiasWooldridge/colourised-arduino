// Copyright 2013 Tobias Wooldridge
// Protocol originally based upon https://github.com/iKenndac/Arduino-Dioder-Playground
#include "Subpixel.h"
#include "Pixel.h"
#include "Subpixel.h"
#include "Pixel.h"

// Configure LEDs
Pixel pixels[] = { Pixel(3, 2, 4), Pixel(6, 5, 7), Pixel(9, 8, 10), Pixel(12, 11, 13)};
int pixelCount = 4;

const int protocolHeaderLeadingByte = 0xBA;
const int protocolHeaderSetColoursByte = 0xBE;

const int protocolHeaderLength = 2;
const int protocolBodyLength = 12;
const int protocolChecksumLength = 1;

byte receivedMessage[protocolBodyLength];

long steppedUntil = millis();

long messageStarted = millis();


void setup() {
  Serial.begin(115200);
}

void loop() {
	if (lastMessageHeader() > 120000)
	{
		for (int i = 0; i < pixelCount; i++) {
			pixels[i].set(0, 0, 0);
		}
	}

	while (Serial.available() >= protocolHeaderLength) {
		messageStarted = millis();

		byte leadingByte = Serial.read();
		if (leadingByte != protocolHeaderLeadingByte)
			continue;

		byte instructionByte = Serial.peek();
		if (instructionByte == protocolHeaderSetColoursByte) {
			// Header match; remove second byte
			Serial.read();
        
			setColors();
		}
	}

	advanceAnimation();
}


inline long lastMessageHeader() {
	return millis() - messageStarted;
}

inline boolean messageTimedOut() {
  return lastMessageHeader() > 20;
}

void advanceAnimation() {
	int steps = (millis() - steppedUntil)/5;
	if (steps > 0)
	{
		for (int i = 0; i < pixelCount; i++) {
			pixels[i].step(steps);
		}

		steppedUntil += 5 * steps;
	}
}

void setColors() {
	while (Serial.available() < (protocolBodyLength + protocolChecksumLength)) {
		if (messageTimedOut()) {
			return;
		}
	}
  
    byte calculatedChecksum = 0;
    
    for (int i = 0; i < protocolBodyLength; i++) {
		receivedMessage[i] = Serial.read();
		calculatedChecksum ^= receivedMessage[i];
    }
    
    byte receivedChecksum = Serial.read();

    if (receivedChecksum == calculatedChecksum) {
	  for (int i = 0; i < pixelCount; i++) {
		  int offset = i * 3;
		  uint16_t red, green, blue;
      
		  red = (double)receivedMessage[offset + 0];
		  green = (double)receivedMessage[offset + 1];
		  blue = (double)receivedMessage[offset + 2];
		  
		  pixels[i].set(red, green, blue);
	  }
      
      Serial.println("Success");
    } else { 
      Serial.println("Fail");
    }
}