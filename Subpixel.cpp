#include "Arduino.h"
#include "Subpixel.h"

Subpixel::Subpixel()
{
	brightnessScaler = 20;
}

void Subpixel::setChannel(const int &c)
{
  channel = c;
}

void Subpixel::set(const double &newTarget)
{
  set(newTarget, SUBPIXEL_DEFAULT_CROSSFADE_STEPS);
}

void Subpixel::set(const double &newTarget, const int &steps)
{
	target = newTarget;
	remainingSteps = steps;
	stepDistance = (target - val)/steps;
}

void Subpixel::step(int times)
{
  for (int i = 0; i < times; i++) {
	val += stepDistance;
  }

  if (val > pwm_range_max)
    val = pwm_range_max;
  else if (val < pwm_range_min)
    val = pwm_range_min;
    
#if PWM_INVERT
  SUBPIXEL_setSubPixel(channel, pwm_range_max - (brightnessScaler * val));
#else
  SUBPIXEL_setSubPixel(channel, brightnessScaler * val);
#endif
}
