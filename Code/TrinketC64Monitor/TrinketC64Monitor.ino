#include <Adafruit_NeoPixel.h>

// Proto board Wiring
// RED         = 5V
// ORANGEGREEN = DIN   #0
// BROWN/BLACK = GND

#ifdef __AVR__
#include <avr/power.h>
#endif

#define NUM_PIXELS 8

#define PIN_PIXELS 0   // 0 for Trinket, 13 for Nano

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // End of trinket special code

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'   
}

int pixel = 4;
int pixeldir = 1;
float bright[NUM_PIXELS] = { 0 };

void loop()
{
    pixel += pixeldir;

    if ((pixel >= NUM_PIXELS) || (pixel < 0))
    {
        pixeldir = -pixeldir;
        pixel += pixeldir;
        return;  // to start of loop()
    }

    bright[pixel] = 1.0;

    delay(100);
    Refresh();
}

void Refresh()
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        bright[i] *= 0.5;
        strip.setPixelColor(i, 0, 0, bright[i] * 128);
    }
    strip.show();
}
