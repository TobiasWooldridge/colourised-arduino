#include "Arduino.h"
#include "Pixel.h"

Pixel::Pixel(const int &r, const int &g, const int &b)
{
  red.setChannel(r);
  green.setChannel(g);
  blue.setChannel(b);
}

void Pixel::set(const uint16_t &r, const uint16_t &g, const uint16_t &b)
{
  red.set(r);
  green.set(g);
  blue.set(b);
}

void Pixel::step()
{
	step(1);
}

void Pixel::step(int times)
{
  red.step(times);
  green.step(times);
  blue.step(times);
}