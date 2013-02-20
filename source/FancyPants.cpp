#include "FancyPants_main.h"

#include "LPD8806.h"
#include "SPI.h"
#include "pixels.h"
#include "TimerOne.h"

// Number of RGB LEDs in strand:
int nLEDs = 22 * 6; //22 LEDs per section, 6 sections

//brightness
int potPin = 5;

LPD8806 strip = LPD8806(nLEDs);//Not using dataPin and clockPin, because Software SPI is lame

int cycle_max = 384;
int cycle_step = 0; //Where we are in the cycle_max cycle
int rb_step = 4; //How much to step by frame - 384/4 = 96 total steps

uint8_t anim = 0; //First animation to run
uint8_t loop_step = 0; //Where we are in the iterations of each animation
uint8_t t_steps = 5; //How many iterations of each animation

//begin and end indicies of each strip
uint8_t sBegin[6] = {0, 22, 44, 66, 88, 110};
uint8_t sEnd[6]   = {21, 43, 65, 87, 109, 131};

//Input a value 0 to cycle_max to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
  case 0:
    r = 127 - WheelPos % 128;   //Red down
    g = WheelPos % 128;      // Green up
    b = 0;                  //blue off
    break; 
  case 1:
    g = 127 - WheelPos % 128;  //green down
    b = WheelPos % 128;      //blue up
    r = 0;                  //red off
    break; 
  case 2:
    b = 127 - WheelPos % 128;  //blue down 
    r = WheelPos % 128;      //red up
    g = 0;                  //green off
    break; 
  }

  r *= 2;
  g *= 2;
  b *= 2;

  return(strip.Color(r,g,b));
}

void setup() {
  //Read the trim pot to get the current brightness
  int val =  analogRead(potPin);
  //Confine to 25% - 80% 
  val = map(val, 0, 1023, 25, 80);
  
  //Init strip and set brightness
  strip.begin();
  strip.setBrightness((float)val / 100.0f);

  //strip data is empty, this clears it
  strip.show();
  
  //Setup hardware timer
  Timer1.initialize();
  Timer1.attachInterrupt(animStep, 1000000 / 60); // 60 frames/second
}

void loop() {
  //No need. Everything handled by the timer
}

//increment all counters
inline void do_step() {
  cycle_step += rb_step;
  if(cycle_step > cycle_max) 
  {
    //done with current cycle, interrate animation
    loop_step++;
    cycle_step = 0;
    if(loop_step >= t_steps)
    {
      loop_step = 0;
      anim++;
      if(anim > 5) anim = 0; 
    }
  }
}

void animStep() {
  
  /*
  strip.show() is basically constant time. 
  Show the data from the last update so that
  the display of each frame is evenly spaced.
  Otherwise we are subjet to however long it 
  takes to calc each frame.
  */
  strip.show();
  
  //Choose the animation to run
  switch(anim)
  {
  case 0: 
    t_steps = 6;
    rainbow_sash();
    break;
  case 1:
	t_steps = 5;
	rainbow_rain(true); 
    break;
  case 2: 
	bloom(true);
    break;
  case 3:
	alternating_rainbows();
	break;
  case 4:
	rainbow_v_wipe();
	break;
  case 5:
	rainbow_h_wipe();
	break;
  }
  
  do_step();
  
}

//Falling discrete rainbow colors
//diag = true -> colors on the diagonal instead of straight
int8_t cur;
float old_bright;
void rainbow_rain(bool diag) {
	//temporarily reset brightness because this is much brighter than most
	old_bright = strip.Brightness();
	strip.setBrightness(old_bright * 0.7f); 
    cur = (380 - cycle_step / 16) % 6;
    for(int8_t y=0; y<22; y++)
    {
		for(int8_t x=0; x<6; x++)
		{
			uint32_t c;
			switch((cur + y + (diag?x:0)) % 6)
			{
				case 0:
					c = VIOLET;
					break;
				case 1:
					c = BLUE;
					break;
				case 2:
					c = GREEN;
					break;
				case 3:
					c = YELLOW;
					break;
				case 4:
					c = ORANGE;
					break;
				case 5:
					c = RED;
					break;
				default:
					c = SKY_BLUE;
					break;
			}
			strip.setPixelColor(pixel(x,y), c);
		}
    }
    
    if(cycle_step >= 380)
		cycle_step = 385; //skip the last bit so it's even :)
		
	strip.setBrightness(old_bright);
}

//diagonal rainbow strip against sky blue
int8_t endP, startP;
void rainbow_sash() {
	endP = ((cycle_step/2) % (22 * 7)) / (rb_step) - 6;
	startP = endP - 11;
	
	for(int8_t x=0; x<6; x++)
	{
		for(int8_t y=0; y<22; y++)
		{
			if(y<startP+x || y>endP+x)
			{
				strip.setPixelColor(pixel(x,y), SKY_BLUE);
			}
			else
			{
				uint32_t c;
				switch(endP+x-y)
				{
					case 0:
					case 1:
						c = VIOLET;
						break;
					case 2:
					case 3:
						c = BLUE;
						break;
					case 4:
					case 5:
						c = GREEN;
						break;
					case 6:
					case 7:
						c = YELLOW;
						break;
					case 8:
					case 9:
						c = ORANGE;
						break;
					case 10:
					case 11:
						c = RED;
						break;
					default:
						c = SKY_BLUE;
						break;
				}
				strip.setPixelColor(pixel(x,y), c);
			}
		}
	}
	
	if(cycle_step >= 304)
		cycle_step = 385; //skip the last bit so it's even :)
}

/*
Looks like a blooming flower
dir = true -> bloom outward
dir = false -> fold inward
*/
void bloom(boolean dir) {
  for(int y=0; y<22; y++) 
  {
    for(int x=0; x<6; x++)
    {
       set_rb_pixel(pixel(x,y), vector(x,y), dir ? cycle_max - cycle_step : cycle_step, 36);
    }
  }
}

/*
Each strand is a single color in the rainbow wheel
*/
void rainbow_h_wipe() {
  for (int i=0; i <= 5; i++) {
    fill(Wheel( (i * 22 + cycle_step) % cycle_max), sBegin[i], sEnd[i]);
  }  
}

/*
Falling rainbows, same on each strand
Every other one has to be reversed because
the strips alternate directions
*/
void rainbow_v_wipe() {
  set_rb_cycle(cycle_step, sEnd[0], sBegin[0]);
  set_rb_cycle(cycle_step, sBegin[1], sEnd[1]);
  set_rb_cycle(cycle_step, sEnd[2], sBegin[2]);
  set_rb_cycle(cycle_step, sBegin[3], sEnd[3]);
  set_rb_cycle(cycle_step, sEnd[4], sBegin[4]);
  set_rb_cycle(cycle_step, sBegin[5], sEnd[5]);
}

/*
Same as above but opposing directions per strand
They are really all the same way but every other 
strand is in the opposite direction
*/
void alternating_rainbows() {
  set_rb_cycle(cycle_step, sBegin[0], sEnd[0]);
  set_rb_cycle(cycle_step, sBegin[1], sEnd[1]);
  set_rb_cycle(cycle_step, sBegin[2], sEnd[2]);
  set_rb_cycle(cycle_step, sBegin[3], sEnd[3]);
  set_rb_cycle(cycle_step, sBegin[4], sEnd[4]);
  set_rb_cycle(cycle_step, sBegin[5], sEnd[5]);
}

/*
Helper method. Generates rainbow spectrum from first to last
*/
void set_rb_cycle(uint16_t cycle_step, uint8_t first, uint8_t last) {
  uint16_t i;

  //Allow for reverse order
  if(first < last)
  {
    for (i=first; i <= last; i++) {
      set_rb_pixel(i, (last - i), cycle_step, (last - first));
    }  
  }
  else
  { 
    for (i=first; i >= last; i--) {
      set_rb_pixel(i, ((first - last) - (first - i)), cycle_step, (first - last));
      if(i==0) break;
    } 
  }
}

//Helper to for the wheel calc
void set_rb_pixel(uint16_t pixel, uint16_t pos, uint16_t cycle_step, uint8_t length)
{
  strip.setPixelColor(pixel, Wheel( ((pos * cycle_max / length) + cycle_step) % cycle_max));
}


//Fills one color from first to last.
void fill(uint32_t c, uint8_t first, uint8_t last) {
  int i;
  if(first > last)
  {
    uint8_t l = first;
    first = last;
    last = l;
  } 

  for(i=first; i <= last; i++) {
    strip.setPixelColor(i, c);
  } 
}

/*
//Needs motion and uses too much power (about 50% more)
void rainbow_stripes()
{
  fill(strip.Color(255, 0, 0), sBegin[0], sEnd[0]);
  fill(strip.Color(255, 127, 0), sBegin[1], sEnd[1]);
  fill(strip.Color(255, 255, 0), sBegin[2], sEnd[2]);
  fill(strip.Color(0, 255, 0), sBegin[3], sEnd[3]);
  fill(strip.Color(0, 0, 255), sBegin[4], sEnd[4]);
  fill(strip.Color(143, 0, 255), sBegin[5], sEnd[5]);
}
*/

/*
for testing pixel coordinates, not used
*/
/*
void matrix()
{
  uint32_t c = strip.Color(255, 0, 0);
  for(int y=0; y<22; y++) 
  {
    for(int x=0; x<6; x++)
    {
       fill(strip.Color(0,0,0), 0, nLEDs - 1);
       strip.setPixelColor(pixel(x,y), c);
    }
  }
}
*/

