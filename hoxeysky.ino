/*******************************************************************************
 * hoxeysky
 * 
 * If you have ever been to a music festival you may have had a hard time
 * finding your camp site after the show.  I mean, it's hard to find it in the
 * daylight sometimes.  Experienced campers bring flag poles that can be seen
 * from far away to mark their spot.  This works well, but at night, even those
 * can be hard to see.
 * 
 * One time before covid I was assigned the duty of creating a flag pole.  This
 * was actually pretty hard.  You can't just have some random garbage.  Everyone
 * that comes to your camp is going to associate you with that, so it's got to
 * be good.
 * 
 * During a small show in Northern Michigan, I saw something that made
 * everything click.  These guys used those pallet sized liquid containers with
 * lights inside as part of their light show.  The plastic diffused the lights.
 * This meant that they had these huge lights flashing and pulsating to the
 * music for probably next to nothing.
 * 
 * Our first go at this was simple and effective: a gallon milk jug on top of
 * a 15 foot pole.  This jug had a blinking LED light inserted, and it diffused
 * light perfectly.  It didn't take long before we noticed other campers using
 * this beacon as their own landmark.  Lots of comments like, "Dude, we're the
 * isle after the blinking light!"
 * 
 * That was Hoxeyville 2019.  We put a lot of thought into how to improve what
 * was there.  Huge grand plans.  Parts were ordered, prototypes were made, and
 * experiments were run.  Then covid happened.  Obviously it got put on a back
 * burner.
 * 
 * Hoxeyville 2021 is coming up, and I've brought some components out of
 * storage.  Rather than go with the full on badassery that was envisioned, I've
 * opted to keep it simple.  This is simply an Arduino, Adafruit 24-channel PWM
 * LED driver, and a clear bowl with paper on the inside.  It's a UFO, and it's
 * going to be perfect.
 * 
 * Copyright 2019-2021 Brian Murphy
 * https://github.com/gurudvlp
 * 
 * This requires an Adafruit TLC5947 24 channel, 12 bit LED driver.  I heavily
 * referenced their code in the original writing of this, so you should check
 * out their product:
 * 
 * http://www.adafruit.com/products/1429
 * 
 */

#include "Adafruit_TLC5947.h"

typedef struct {
	uint16_t r;
	uint16_t g;
	uint16_t b;
} hoxeyColor;

void effectFireball(hoxeyColor * colors, unsigned char steps, uint16_t cycles, unsigned char frequency, bool preserve);

// How many boards do you have chained?
#define NUM_TLC5974 1

#define PIN_DATA 4
#define PIN_CLOCK 5
#define PIN_LATCH 6

#define LIGHTCOUNT 8
#define LED_R 0
#define LED_G 1
#define LED_B 2

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, PIN_CLOCK, PIN_DATA, PIN_LATCH);

uint16_t lightState[LIGHTCOUNT][3];


void setup() {
	Serial.begin(9600);
  
	Serial.println("hoxeysky 2021");
	Serial.println("bmfs");
	Serial.println("");
	Serial.println("2019 - 2021 Brian Murphy");
	Serial.println("github.com/gurudvlp");
	
	tlc.begin();

	/*tlc.setLED(0, 0, 0, 0);
	tlc.setLED(1, 0, 0, 0);
	tlc.setLED(2, 0, 0, 0);*/
	
	for(unsigned char el = 0; el < LIGHTCOUNT; el++)
	{
		lightState[el][LED_R] = 0;
		lightState[el][LED_G] = 0;
		lightState[el][LED_B] = 0;
	}
}

void loop() 
{

	
	//	Fireball
	hoxeyColor fireballColors[4];
	fireballColors[0].r = 4095;
	fireballColors[0].g = 1152;
	fireballColors[0].b = 1344;
	fireballColors[1].r = 4095;
	fireballColors[1].g = 2096;
	fireballColors[1].b = 816;
	fireballColors[2].r = 4095;	
	fireballColors[2].g = 3040;
	fireballColors[2].b = 288;
	fireballColors[3].r = 4095;	
	fireballColors[3].g = 3504;
	fireballColors[3].b = 16;
	
	effectFireball((hoxeyColor *)&fireballColors, 4, 8, 8, true);
	
	//	Slide to purple
	effectSlideAll(2047, 0, 2047, 20, 10);
	
	//	Stay purple for 5 seconds
	delay(5000);
	
	//	Slide to off
	effectSlideAll(0, 0, 0, 16, 8);
	
	//	Stay off for 5 seconds
	delay(5000);
	
	//	Ping pong several times
	uint16_t colorLeft[3] = { 0, 0, 255 };
	uint16_t colorRight[3] = { 0, 255, 0 };
	for(unsigned char cycles = 0; cycles < 16; cycles++)
	{
		//	Update the colors that are ping ponging to morph shades
		colorLeft[LED_B] = (cycles * 256) + 255;
		colorLeft[LED_R] = (cycles * 128) + 127;
		
		colorRight[LED_G] = (cycles * 256) + 255;
		colorRight[LED_R] = (cycles * 128) + 127;
		
		//	Initiate ping pong
		effectPingPong((uint16_t *)&colorLeft, (uint16_t *)&colorRight, 6, true);
	}
	
	//	Blink green and blue several times
	for(unsigned char cycles = 0; cycles < 32; cycles++)
	{
		hoxeyClear(0, 0, 4095);
		delay(250);
		hoxeyClear(0, 4095, 0);
		delay(250);
	}
	
	//	Slide to red
	effectSlideAll(4096, 0, 0, 16, 8);
	
	//	Stay red for 3 seconds
	delay(3000);
	
	//	Circle blue around
	effectColorRotate(0, 0, 3071, 4, true);
	
	//	Circle blue around a bit faster
	effectColorRotate(0, 0, 3071, 6, true);
	
	//	Circle blue again but don't preserve
	effectColorRotate(0, 0, 3071, 8, false);
	
	//	Slide to dark green
	effectSlideAll(0, 2047, 0, 8, 8);
	
	//	Stay green for 3 seconds
	delay(3000);
	
	//	Circle blue in the green
	effectColorRotate(0, 0, 3071, 2, true);
	
	//	Blue again faster
	effectColorRotate(0, 0, 3071, 8, true);
	effectColorRotate(0, 0, 3071, 16, true);
	effectColorRotate(0, 0, 3071, 32, true);
	effectColorRotate(0, 0, 3071, 32, true);
	
	//	Spin the blue fast several times
	for(unsigned char cycles = 0; cycles < 16; cycles++)
	{
		effectColorRotate(0, 0, 3071, 64, true);
	}
	
	//	Slide to blue and hold for 3 seconds
	effectSlideAll(0, 0, 4095, 8, 16);
	delay(3000);
	
	//	Spin green around
	effectColorRotate(0, 4095, 0, 8, true);
	effectColorRotate(0, 4095, 0, 16, true);
	effectColorRotate(0, 4095, 0, 32, true);
	effectColorRotate(0, 4095, 0, 32, true);
	
	//	Sping green fast several times
	for(unsigned char cycles = 0; cycles < 15; cycles++)
	{
		effectColorRotate(0, 4095, 0, 64, true);
	}
	
	//	Clear lights then go to black and hold
	effectColorRotate(0, 4095, 0, 64, false);
	hoxeyClear(0, 0, 0);
	delay(5000);
	
	//	Flash green a few times
	for(unsigned char cycles = 0; cycles < 4; cycles++)
	{
		effectSlideAll(0, 4095, 0, 255, 64);
		delay(500);
		effectSlideAll(0, 0, 0, 255, 64);
		delay(500);
	}
	
	//	Some rainbow stuff
	for(unsigned char erb = 0; erb < 8; erb++)
	{
		effectSlideAll(4095, 0, 0, 255, 128);
		effectSlideAll(0, 4095, 0, 255, 128);
		effectSlideAll(0, 0, 4095, 255, 128);
	}
	
	//	Set to green, slowly get brighter while spinning blue
	effectSlideAll(0, 0, 0, 255, 64);
	hoxeyClear(0, 0, 0);
	delay(3000);
	
	for(unsigned char gloops = 0; gloops < 32; gloops++)
	{
		effectSlideAll(0, gloops * 128, 0, 16, 128);
		effectColorRotate(0, 0, 3071, 32, true);
	}
	
	//	Fast rainbow steps
	for(unsigned char erb = 0; erb < 128; erb++)
	{
		effectSlideAll(4095, 0, 0, 8, 128);			//	To Red
		effectSlideAll(4095, 4095, 0, 8, 128);		//	To Red + Green
		effectSlideAll(0, 4095, 0, 8, 128);			//	To Green
		effectSlideAll(0, 4095, 4095, 8, 128);		//	To Green + Blue
		effectSlideAll(0, 0, 4095, 8, 128);			//	To Blue
		effectSlideAll(4095, 0, 4095, 8, 128);		//	To Blue + Red
	}

}

void hoxeySetLED(unsigned char led, uint16_t r, uint16_t g, uint16_t b)
{
	lightState[led][LED_R] = r;
	lightState[led][LED_G] = g;
	lightState[led][LED_B] = b;
}

void hoxeyGetLED(unsigned char led, uint16_t * ledData)
{
	ledData[LED_R] = hoxeyGetLEDColor(led, LED_R);
	ledData[LED_G] = hoxeyGetLEDColor(led, LED_G);
	ledData[LED_B] = hoxeyGetLEDColor(led, LED_B);
}

unsigned short hoxeyGetLEDColor(unsigned char led, unsigned char color)
{
	return lightState[led][color];
}

void hoxeyWrite()
{
	for(unsigned char el = 0; el < LIGHTCOUNT; el++)
	{
		tlc.setLED(
			el,
			lightState[el][LED_R],
			lightState[el][LED_G],
			lightState[el][LED_B]
		);
	}
	
	tlc.write();
}

//	Slide all LEDs to new color
void effectSlideAll(uint16_t r, uint16_t g, uint16_t b, unsigned char steps, unsigned char frequency)
{
	//	Determine how much each light needs to change on each step
	unsigned short factors[LIGHTCOUNT][3];
	for(unsigned char el = 0; el < LIGHTCOUNT; el++)
	{
		factors[el][LED_R] = calculateSlideFactor(steps, lightState[el][LED_R], r);
		factors[el][LED_G] = calculateSlideFactor(steps, lightState[el][LED_G], g);
		factors[el][LED_B] = calculateSlideFactor(steps, lightState[el][LED_B], b);
	}
	
	//	Loop through the number of steps and set the color for each step
	for(unsigned char estep = 0; estep < steps; estep++)
	{
		for(unsigned char el = 0; el < LIGHTCOUNT; el++)
		{
			hoxeySetLED(
				el,
				hoxeyGetLEDColor(el, LED_R) + factors[el][LED_R],
				hoxeyGetLEDColor(el, LED_G) + factors[el][LED_G],
				hoxeyGetLEDColor(el, LED_B) + factors[el][LED_B]
			);
		}
		
		hoxeyWrite();
		
		delay(1000 / frequency);
	}
}

uint16_t calculateSlideFactor(unsigned char steps, uint16_t originalColor, uint16_t newColor)
{
	if(newColor > originalColor)
	{
		uint16_t amount = newColor - originalColor;
		return (uint16_t)(amount / steps);
	}
	else if(newColor < originalColor)
	{
		uint16_t amount = originalColor - newColor;
		return (uint16_t)(amount / steps) * -1;
	}
	
	return 0;
}


void hoxeyClear(uint16_t r, uint16_t g, uint16_t b)
{
	for(unsigned char el = 0; el < LIGHTCOUNT; el++)
	{
		hoxeySetLED(el, r, g, b);
	}
	
	hoxeyWrite();
}



//  Rotate a color around the center
//    r, g, b are 12-bit color values (0 - 4095)
//    frequency:  frequency (spaces per second)
//    preserve: preserve the original color in a node
void effectColorRotate(uint16_t r, uint16_t g, uint16_t b, unsigned char frequency, bool preserve)
{
	//	Create an array for the original color of the LED that is being changed
	//	during each step.  We initialize this to off so it can be used to
	//	clear the LED if we are not preserving the color.
	uint16_t originalColor[3];
	originalColor[LED_R] = 0;
	originalColor[LED_G] = 0;
	originalColor[LED_B] = 0;
	
	for(unsigned char el = 0; el < LIGHTCOUNT; el++)
	{
		if(el > 0)
		{
			hoxeySetLED(
				el - 1, 
				originalColor[LED_R],
				originalColor[LED_G],
				originalColor[LED_B]
			);
		}
		
		if(preserve) { hoxeyGetLED(el, (uint16_t *)&originalColor); }
		
		hoxeySetLED(el, r, g, b);
		hoxeyWrite();
		
		delay(1000 / frequency);
	}
	
	hoxeySetLED(
		LIGHTCOUNT - 1,
		originalColor[LED_R],
		originalColor[LED_G],
		originalColor[LED_B]
	);
	hoxeyWrite();
}

//	Ping pong LED on either side
//		*colorLeft:		uint16_t[3] of first color
//		*colorRight:	uint16_t[3] of second color
//		frequency:		speed to move at
//		tracer:			use tracer effect
void effectPingPong(uint16_t * colorLeft, uint16_t * colorRight, unsigned char frequency, bool tracer)
{
	for(unsigned char el = 0; el < (LIGHTCOUNT / 2); el++)
	{
		//	If we have already previously set a light, it needs to be blanked
		//	back out
		if(el > 0)
		{
			hoxeySetLED(el - 1, 0, 0, 0);
			hoxeySetLED(LIGHTCOUNT - el, 0, 0, 0);
		}
		
		//	Set current lights to specified colors
		hoxeySetLED(
			el,
			colorRight[LED_R],
			colorRight[LED_G],
			colorRight[LED_B]
		);
		
		hoxeySetLED(
			(LIGHTCOUNT - 1) - el,
			colorLeft[LED_R],
			colorLeft[LED_G],
			colorLeft[LED_B]
		);
		
		hoxeyWrite();
		
		delay(1000 / frequency);
	}
	
	//	Now to loop back the way the lights came (for the actual effect)
	for(unsigned char el = (LIGHTCOUNT / 2) - 1; el > -1; el--)
	{
		if(el == 255) break;

		//	If we have already previously set a light, it needs to be blanked
		//	back out
		if(el < ((LIGHTCOUNT / 2) - 1))
		{
			hoxeySetLED(el + 1, 0, 0, 0);
			hoxeySetLED(LIGHTCOUNT - 2 - el, 0, 0, 0);
		}
		
		//	Set current lights to specified colors
		hoxeySetLED(
			el,
			colorRight[LED_R],
			colorRight[LED_G],
			colorRight[LED_B]
		);
		
		hoxeySetLED(
			(LIGHTCOUNT - 1) - el,
			colorLeft[LED_R],
			colorLeft[LED_G],
			colorLeft[LED_B]
		);
		
		hoxeyWrite();
		
		delay(1000 / frequency);
		
		
		
	}
}

//	Spin a set of colors around
void effectFireball(hoxeyColor * colors, unsigned char steps, uint16_t cycles, unsigned char frequency, bool preserve)
{
	//	Grab the current color state so that we can preserve if needed
	uint16_t originalColors[LIGHTCOUNT][3];
	for(unsigned char el = 0; el < LIGHTCOUNT; el++)
	{
		originalColors[el][LED_R] = hoxeyGetLEDColor(el, LED_R);
		originalColors[el][LED_G] = hoxeyGetLEDColor(el, LED_G);
		originalColors[el][LED_B] = hoxeyGetLEDColor(el, LED_B);
	}
	
	//	Loop the number of cycles to go around
	for(uint16_t ecycle = 0; ecycle < cycles; ecycle++)
	{
		//	Loop over each LED
		for(unsigned char el = 0; el < LIGHTCOUNT; el++)
		{
			//	Restore all of the preserved colors
			if(preserve)
			{
				for(unsigned char ep = 0; ep < LIGHTCOUNT; ep++)
				{
					hoxeySetLED(
						ep,
						originalColors[ep][LED_R],
						originalColors[ep][LED_G],
						originalColors[ep][LED_B]
					);
				}
			}
			
			//	Set main fireball LED
			hoxeySetLED(
				el,
				colors[0].r,
				colors[0].g,
				colors[0].b
			);
			
			//	Loop backwards to set prior LEDs
			for(unsigned char estep = 1; estep < steps; estep++)
			{
				//	Little bit of checking here.  If we haven't done any steps
				//	or cycles, then we shouldn't back fill anything
				if(el > 0)
				{
					//	Currently past LED 0
					//	Check if the step is greater than the current LED.  If
					//	so, then it needs to wrap around
					if(estep > el)
					{
						hoxeySetLED(
							LIGHTCOUNT - (estep - el),
							colors[estep].r,
							colors[estep].g,
							colors[estep].b
						);
					}
					else
					{
						hoxeySetLED(
							el - estep,
							colors[estep].r,
							colors[estep].g,
							colors[estep].b
						);
					}
				}
				else
				{
					if(ecycle == 0)
					{
						//	On the initial cycle, no trail should be created
					}
					else
					{
						//	A trail needs to be created behind the main LED
						hoxeySetLED(
							LIGHTCOUNT - estep,
							colors[estep].r,
							colors[estep].g,
							colors[estep].b
						);
					}
				}
			}
			
			hoxeyWrite();
			delay(1000 / frequency);
		}
	}
}
