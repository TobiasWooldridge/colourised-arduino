#include "Arduino.h"

#ifndef Subpixel_h
#define Subpixel_h

#define SUBPIXEL_OUTPUT_STOCK 0
#define SUBPIXEL_OUTPUT_TLC 1

#define SUBPIXEL_OUTPUT_MODE OUTPUT_STOCK

#define SUBPIXEL_DEFAULT_BRIGHTNESS 1
#define SUBPIXEL_DEFAULT_CROSSFADE_STEPS 240

#if SUBPIXEL_OUTPUT_MODE == SUBPIXEL_OUTPUT_TLC
	#define SUBPIXEL_PWM_INVERT 1
	#define SUBPIXEL_setSubPixel(c, v) Tlc.set(c, v)
#elif SUBPIXEL_OUTPUT_MODE == SUBPIXEL_OUTPUT_STOCK
	#define SUBPIXEL_setSubPixel(c, v) analogWrite(c, v)
	#define SUBPIXEL_PWM_INVERT 0
#endif


class Subpixel {
  private:
	#if SUBPIXEL_OUTPUT_MODE == SUBPIXEL_OUTPUT_TLC
		static const int pwm_range_max = 4095;
		static const int pwm_range_min = 0;
	#elif SUBPIXEL_OUTPUT_MODE == SUBPIXEL_OUTPUT_STOCK
		static const int pwm_range_max = 255;
		static const int pwm_range_min = 0;
	#endif

    unsigned int channel;
	int target;
    double val;
	double stepDistance;
	int remainingSteps;
	double brightnessScaler;
        
  public:

    Subpixel();
    void set(const double &newtarget);
	void set(const double &newtarget, const int &steps);
    void setChannel(const int &c);
    void step(int times);
};

#endif

