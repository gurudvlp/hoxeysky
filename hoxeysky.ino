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
  unsigned char randomScript = random(0, 10);
  bool doDelay = ((random(0, 100) > 50) ? true : false);
  uint16_t delayTime = random(1000, 5000);
  
    
  if(randomScript == 0) { scriptFireball(); }
  else if(randomScript == 1) { scriptSlideRandom(); }
  else if(randomScript == 2) { scriptClear(); }
  else if(randomScript == 3) { scriptPingPong(); }
  else if(randomScript == 4) { scriptBlink(); }
  else if(randomScript == 5) { scriptColorRotate(); }
  else if(randomScript == 6) { scriptBreathe(); }
  else if(randomScript == 7) { scriptRainbowSpin(); }
  else if(randomScript == 8) { scriptFillAndSpin(); }
  else { scriptRainbowStep(); }
  
  if(doDelay)
  {
    delay(delayTime);
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

//  Initiate a randomly generated fireball effect.  This uses 4 steps.
void scriptFireball()
{
  unsigned char steps = 4;
  hoxeyColor colors[4];
  
  uint16_t cycles = random(4, 33);
  unsigned char frequency = random(2, 65);
  bool preserve = ((random(0, 100) > 50) ? true : false);
  
  unsigned char oColor = random(0, 6);
  if(oColor == 0)
  { 
    colors[0].r = 4095;
    colors[0].g = 0;
    colors[0].b = 0;

  }
  else if(oColor == 1)
  {
    colors[0].r = 4095;
    colors[0].g = 4095;
    colors[0].b = 0;
  }
  else if(oColor == 2)
  {
    colors[0].r = 0;
    colors[0].g = 4095;
    colors[0].b = 0;
  }
  else if(oColor == 3)
  {
    colors[0].r = 0;
    colors[0].g = 4095;
    colors[0].b = 4095;
  }
  else if(oColor == 4)
  {
    colors[0].r = 0;
    colors[0].g = 0;
    colors[0].b = 4095;
  }
  else
  {
    colors[0].r = 4095;
    colors[0].g = 0;
    colors[0].b = 4095;
  }
  
  for(unsigned char estep =1; estep < steps; estep++)
  {
    colors[estep].r = colors[0].r / estep;
    colors[estep].g = colors[0].g / estep;
    colors[estep].b = colors[0].b / estep;
  }
  

  effectFireball((hoxeyColor *)&colors, steps, cycles, frequency, preserve);
}

void scriptSlideRandom()
{
  uint16_t r = random(0, 4096);
  uint16_t g = random(0, 4096);
  uint16_t b = random(0, 4096);
  unsigned char steps = random(4, 65);
  unsigned char frequency = random(4, 129);
  
  effectSlideAll(r, g, b, steps, frequency);
}

void scriptClear()
{
  unsigned char steps = random(8, 65);
  unsigned char frequency = random(4, 129);
  
  unsigned char clearColor = random(0, 4);
  
  effectSlideAll
  (
    ((clearColor == 1) ? 512 : 0), 
    ((clearColor == 2) ? 512 : 0), 
    ((clearColor == 3) ? 512 : 0), 
    steps, 
    frequency
  );
  
  hoxeyClear
  (
    ((clearColor == 1) ? 512 : 0), 
    ((clearColor == 2) ? 512 : 0), 
    ((clearColor == 3) ? 512 : 0)   
  );
}

void scriptPingPong()
{
  //  Ping pong several times
  unsigned char colorScheme = random(0, 2);
  
  hoxeyColor colorLeft;
  colorLeft.r = ((colorScheme == 0) ? 4095 : 0);
  colorLeft.g = ((colorScheme == 0) ? 0 : 4095);
  colorLeft.b = ((colorScheme == 0) ? 0 : 0);
  
  hoxeyColor colorRight;
  colorRight.r = ((colorScheme == 0) ? 3071 : 0);
  colorRight.g = ((colorScheme == 0) ? 4095 : 0);
  colorRight.b = ((colorScheme == 0) ? 0 : 4095);
  
  unsigned char cycles = random(4, 17);
  unsigned char frequency = random(4, 33);
  bool tracer = ((random(0, 100) > 50) ? true : false);
  
  for(unsigned char ecycle = 0; ecycle < cycles; ecycle++)
  {
    //  Initiate ping pong
    effectPingPong(colorLeft, colorRight, frequency, tracer);
  }
}

void scriptBlink()
{
  //  Blink green and blue several times
  unsigned char cycles = random(8, 33);
  uint16_t delayTime = random(100, 501);
  unsigned char colorScheme = random(0, 2);
  
  for(unsigned char ecycle = 0; ecycle < cycles; ecycle++)
  {
    hoxeyClear
    (
      ((colorScheme == 0) ? 0 : 4095), 
      ((colorScheme == 0) ? 0 : 0), 
      ((colorScheme == 0) ? 4095 : 0)
    );
    
    delay(delayTime);
    
    hoxeyClear
    (
      ((colorScheme == 0) ? 0 : 4095), 
      ((colorScheme == 0) ? 4095 : 3071),
      ((colorScheme == 0) ? 0 : 0)
    );
    
    delay(delayTime);
  }
}

//  Slide all LEDs to new color
void effectSlideAll(uint16_t r, uint16_t g, uint16_t b, unsigned char steps, unsigned char frequency)
{
  //  Determine how much each light needs to change on each step
  unsigned short factors[LIGHTCOUNT][3];
  for(unsigned char el = 0; el < LIGHTCOUNT; el++)
  {
    factors[el][LED_R] = calculateSlideFactor(steps, lightState[el][LED_R], r);
    factors[el][LED_G] = calculateSlideFactor(steps, lightState[el][LED_G], g);
    factors[el][LED_B] = calculateSlideFactor(steps, lightState[el][LED_B], b);
  }
  
  //  Loop through the number of steps and set the color for each step
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

void scriptColorRotate()
{
  hoxeyColor color;
  unsigned char colorScheme = random(0, 3);
  unsigned char frequency = random(4, 65);
  unsigned char cycles = random(1, 9);
  bool preserve = ((random(0, 100) > 50) ? true : false);
  
  if(colorScheme == 0) { color.r = 4095; } else { color.r = 0; }
  if(colorScheme == 1) { color.g = 4095; } else { color.g = 0; }
  if(colorScheme == 2) { color.b = 4095; } else { color.b = 0; }
  
  for(unsigned char ecycle = 0; ecycle < cycles; ecycle++)
  {
    effectColorRotate(color.r, color.g, color.b, frequency, preserve);
  }
}

void scriptRainbowStep()
{
  unsigned char steps = random(4, 65);
  unsigned char frequency = random(steps / 2, 129);
  unsigned char cycles = random(1, 5);
  
  bool fullSpectrum = ((random(0, 100) > 50) ? true : false);
  bool fullSlide = ((random(0, 100) > 50) ? true : false);
  
  for(unsigned char erb = 0; erb < cycles; erb++)
  {
    if(fullSlide)
    {
      effectSlideAll(4095, 0, 0, steps, frequency);             //  To Red
      if(fullSpectrum) { effectSlideAll(4095, 4095, 0, steps, frequency); } //  To Red + Green
      effectSlideAll(0, 4095, 0, steps, frequency);             //  To Green
      if(fullSpectrum) { effectSlideAll(0, 4095, 4095, steps, frequency); } //  To Green + Blue
      effectSlideAll(0, 0, 4095, steps, frequency);             //  To Blue
      if(fullSpectrum) { effectSlideAll(4095, 0, 4095, steps, frequency); } //  To Blue + Red
    }
    else
    {
      //  Clear to each color
      bool clearEach = ((random(0, 100) > 50) ? true : false);
      
      //  Red
      hoxeyClear(4095, 0, 0);
      delay((1000 / frequency) * steps);
      if(clearEach) { hoxeyClear(0, 0, 0); }
      delay((1000 / frequency) * steps);
      
      //  Red + Green
      if(fullSpectrum)
      {
        hoxeyClear(4095, 0, 0);
        delay((1000 / frequency) * steps);
        if(clearEach) { hoxeyClear(0, 0, 0); }
        delay((1000 / frequency) * steps);
      }
      
      //  Green
      hoxeyClear(0, 4095, 0);
      delay((1000 / frequency) * steps);
      if(clearEach) { hoxeyClear(0, 0, 0); }
      delay((1000 / frequency) * steps);
      
      //  Green + Blue
      if(fullSpectrum)
      {
        hoxeyClear(0, 4095, 4095);
        delay((1000 / frequency) * steps);
        if(clearEach) { hoxeyClear(0, 0, 0); }
        delay((1000 / frequency) * steps);
      }
      
      //  Blue
      hoxeyClear(0, 0, 4095);
      delay((1000 / frequency) * steps);
      if(clearEach) { hoxeyClear(0, 0, 0); }
      delay((1000 / frequency) * steps);
      
      //  Blue + Red
      if(fullSpectrum)
      {
        hoxeyClear(4095, 0, 4095);
        delay((1000 / frequency) * steps);
        if(clearEach) { hoxeyClear(0, 0, 0); }
        delay((1000 / frequency) * steps);
      }
    }
  }
}

void scriptBreathe()
{
  //  Breath will quickly slide to the new color.  It will then either quickly
  //  or slowly breath for a minute.
  
  unsigned char frequency = ((random(0, 100) > 50) ? 32 : 8);
  unsigned char delayTime = ((random(0, 100) > 50) ? 100 : 500);
  bool breatheBlack = ((random(0, 100) > 50) ? true : false);
  //bool breathFast = ((random(0, 100) > 50) ? true : false);
  
  hoxeyColor color;
  hoxeyColor colorOff;
  unsigned char colorScheme = random(0, 3);
  
  color.r = 0;
  color.g = 0;
  color.b = 0;
  colorOff.r = 0;
  colorOff.g = 0;
  colorOff.b = 0;
  
  if(colorScheme == 0) { color.r = 4095; }
  else if(colorScheme == 1) { color.g = 4095; }
  else { color.b = 4095; }
  
  if(!breatheBlack)
  {
    if(colorScheme == 0) { colorOff.g = 4095; colorOff.b = 4095; }
    if(colorScheme == 1) { colorOff.r = 4095; colorOff.b = 4095; }
    else { colorOff.r = 4095; colorOff.g = 4095; }
  }
  
  //  Slide to the new color in like half a second
  effectSlideAll(color.r, color.g, color.b, 32, 64);
  
  //  Clear to the new color to make sure it's right
  hoxeyClear(color.r, color.g, color.b);
  
  for(unsigned char cycles = 0; cycles < 16; cycles++)
  {
    effectSlideAll(colorOff.r, colorOff.g, colorOff.b, 8, frequency);
    delay(delayTime);
    
    effectSlideAll(color.r, color.g, color.b, 8, frequency);
    delay(delayTime);
  }
}

void scriptRainbowSpin()
{
  hoxeyColor colorRed;
  hoxeyColor colorGreen;
  hoxeyColor colorBlue;
  
  colorRed.r = 4095;
  colorGreen.g = 4095;
  colorBlue.b = 4095;
  
  unsigned char frequency = ((random(0, 100) > 50) ? 8 : 16);
  bool reverse = ((random(0, 100) > 50) ? true : false);
  unsigned char rotations = random(4, 17);
  
  //  Set all of the LEDs to initial pattern
  for(unsigned char el = 0; el < LIGHTCOUNT; el += 3)
  {
    hoxeySetLED(el, colorRed.r, colorRed.g, colorRed.b);
    if(el + 2 < LIGHTCOUNT) { hoxeySetLED(el + 1, colorGreen.r, colorGreen.g, colorGreen.b); }
    if(el + 3 < LIGHTCOUNT) { hoxeySetLED(el + 2, colorBlue.r, colorBlue.g, colorBlue.b); }
  }
  
  hoxeyWrite();
  
  //  Do the spin
  for(unsigned char erotation = 0; erotation < rotations; erotation++)
  {
    for(unsigned char el = 0; el < LIGHTCOUNT; el++)
    {
      effectRotateAll(reverse);
      delay(1000 / frequency);
    }
  }
}

void scriptFillAndSpin()
{
	unsigned char colorScheme = random(0, 3);
	unsigned char rotations = random(4, 17);
	bool reverse = ((random(0, 100) > 50) ? true : false);
	unsigned char frequency = ((random(0, 100) > 50) ? 8 : 16);

	hoxeyClear(0, 0, 0);
	
	if(colorScheme == 0)
	{
		//	Blue and green
		hoxeySetLED(0, 0, 0, 4095);
		hoxeySetLED(LIGHTCOUNT / 2, 0, 4095, 0);
	}
	else if(colorScheme == 1)
	{
		//	Red >> Yellow >> Green >> Yellow >> Red
		hoxeySetLED(0, 3263, 0, 0);
		hoxeySetLED(LIGHTCOUNT / 2, 0, 3263, 0);
		
		hoxeySetLED(LIGHTCOUNT / 4, 3263, 3263, 0);
		hoxeySetLED(LIGHTCOUNT - (LIGHTCOUNT / 4), 3263, 3263, 0);
	}
	else
	{
		//	Completely random
		for(unsigned char el = 0; el < LIGHTCOUNT; el++)
		{
			hoxeySetLED(el, random(0, 4096), random(0, 4096), random(0, 4096));
		}
	}
	
	hoxeyWrite();
	
	//  Do the spin
	for(unsigned char erotation = 0; erotation < rotations; erotation++)
	{
		for(unsigned char el = 0; el < LIGHTCOUNT; el++)
		{
			effectRotateAll(reverse);
			delay(1000 / frequency);
		}
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
  //  Create an array for the original color of the LED that is being changed
  //  during each step.  We initialize this to off so it can be used to
  //  clear the LED if we are not preserving the color.
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

//  Ping pong LED on either side
//    *colorLeft:   uint16_t[3] of first color
//    *colorRight:  uint16_t[3] of second color
//    frequency:    speed to move at
//    tracer:     use tracer effect
void effectPingPong(hoxeyColor colorLeft, hoxeyColor colorRight, unsigned char frequency, bool tracer)
{
  for(unsigned char el = 0; el < (LIGHTCOUNT / 2); el++)
  {
    //  If we have already previously set a light, it needs to be blanked
    //  back out
    if(el > 0)
    {
      hoxeySetLED(el - 1, 0, 0, 0);
      hoxeySetLED(LIGHTCOUNT - el, 0, 0, 0);
    }
    
    //  Set current lights to specified colors
    hoxeySetLED(
      el,
      colorRight.r,
      colorRight.g,
      colorRight.b
    );
    
    hoxeySetLED(
      (LIGHTCOUNT - 1) - el,
      colorLeft.r,
      colorLeft.g,
      colorLeft.b
    );
    
    hoxeyWrite();
    
    delay(1000 / frequency);
  }
  
  //  Now to loop back the way the lights came (for the actual effect)
  for(unsigned char el = (LIGHTCOUNT / 2) - 1; el > -1; el--)
  {
    if(el == 255) break;

    //  If we have already previously set a light, it needs to be blanked
    //  back out
    if(el < ((LIGHTCOUNT / 2) - 1))
    {
      hoxeySetLED(el + 1, 0, 0, 0);
      hoxeySetLED(LIGHTCOUNT - 2 - el, 0, 0, 0);
    }
    
    //  Set current lights to specified colors
    hoxeySetLED(
      el,
      colorRight.r, 
      colorRight.g,
      colorRight.b
    );
    
    hoxeySetLED(
      (LIGHTCOUNT - 1) - el,
      colorLeft.r, 
      colorLeft.g, 
      colorLeft.b
    );
    
    hoxeyWrite();
    
    delay(1000 / frequency);
    
    
    
  }
}

//  Spin a set of colors around
void effectFireball(hoxeyColor * colors, unsigned char steps, uint16_t cycles, unsigned char frequency, bool preserve)
{
  //  Grab the current color state so that we can preserve if needed
  uint16_t originalColors[LIGHTCOUNT][3];
  for(unsigned char el = 0; el < LIGHTCOUNT; el++)
  {
    originalColors[el][LED_R] = hoxeyGetLEDColor(el, LED_R);
    originalColors[el][LED_G] = hoxeyGetLEDColor(el, LED_G);
    originalColors[el][LED_B] = hoxeyGetLEDColor(el, LED_B);
  }
  
  //  Loop the number of cycles to go around
  for(uint16_t ecycle = 0; ecycle < cycles; ecycle++)
  {
    //  Loop over each LED
    for(unsigned char el = 0; el < LIGHTCOUNT; el++)
    {
      //  Restore all of the preserved colors
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
      
      //  Set main fireball LED
      hoxeySetLED(
        el,
        colors[0].r,
        colors[0].g,
        colors[0].b
      );
      
      //  Loop backwards to set prior LEDs
      for(unsigned char estep = 1; estep < steps; estep++)
      {
        //  Little bit of checking here.  If we haven't done any steps
        //  or cycles, then we shouldn't back fill anything
        if(el > 0)
        {
          //  Currently past LED 0
          //  Check if the step is greater than the current LED.  If
          //  so, then it needs to wrap around
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
            //  On the initial cycle, no trail should be created
          }
          else
          {
            //  A trail needs to be created behind the main LED
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

//  Rotate all LEDs.
//    reverse:  rotate the other way
void effectRotateAll(bool reverse)
{
  hoxeyColor holdColor;
  
  //	Going forward doesn't work...  maybe I can debug this later
  reverse = true;
  
  if(!reverse)
  {
    //  Rotate lights clockwise.  This actually loops counterclockwise to
    //  write the values of the light before to the current light
    
    holdColor.r = hoxeyGetLEDColor(LIGHTCOUNT - 1, LED_R);
    holdColor.g = hoxeyGetLEDColor(LIGHTCOUNT - 1, LED_G);
    holdColor.b = hoxeyGetLEDColor(LIGHTCOUNT - 1, LED_B);
    
    for(unsigned char el = (LIGHTCOUNT - 2); el > 0; el--)
    {
      hoxeySetLED(
        el,
        hoxeyGetLEDColor(el - 1, LED_R),
        hoxeyGetLEDColor(el - 1, LED_G),
        hoxeyGetLEDColor(el - 1, LED_B)
      );
    }
    
    hoxeySetLED(
      0,
      holdColor.r,
      holdColor.g,
      holdColor.b
    );
    
  }
  else
  {
    //  Rotate lights counterclockwise
    
    holdColor.r = hoxeyGetLEDColor(0, LED_R);
    holdColor.g = hoxeyGetLEDColor(0, LED_G);
    holdColor.b = hoxeyGetLEDColor(0, LED_B);
    
    for(unsigned char el = 0; el < (LIGHTCOUNT - 1); el++)
    {
      hoxeySetLED(
        el,
        hoxeyGetLEDColor(el + 1, LED_R),
        hoxeyGetLEDColor(el + 1, LED_G),
        hoxeyGetLEDColor(el + 1, LED_B)
      );
    }
    
    hoxeySetLED(
      LIGHTCOUNT - 1,
      holdColor.r,
      holdColor.g,
      holdColor.b
    );
    
  }
  
  hoxeyWrite();
}
