#include "Arduino.h"
#include "Subpixel.h"

#ifndef Pixel_h
#define Pixel_h


class Pixel {
  private:
    Subpixel red;
    Subpixel green;
    Subpixel blue;
    
  public:
    Pixel(const int &redChannel, const int &greenChannel, const int &blueChannel);
    void set(const uint16_t &r, const uint16_t &g, const uint16_t &b);
	void step();
	void step(int times);
};


#endif
