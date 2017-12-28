#include <Adafruit_NeoPixel.h>
#include <SimpleTimer.h>

// Proto board Wiring
// RED         = 5V
// ORANGEGREEN = DIN   #0
// BROWN/BLACK = GND

#ifdef __AVR__
#include <avr/power.h>
#endif

#define NUM_PIXELS 8
#define PIN_PIXELS 0   // 0 for Trinket,

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);

// The timer object
SimpleTimer timer;

void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // End of trinket special code

    timer.setInterval(100, Animate);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'   
}

int pixel = 4;
int pixeldir = 1;
float bright[NUM_PIXELS] = { 0 };

void loop()
{
    timer.run();
}

void Animate()
{
    while (true)
    {
        pixel += pixeldir;

        if ((pixel >= NUM_PIXELS) || (pixel < 0))
        {
            pixeldir = -pixeldir;
            pixel += pixeldir;
            pixel += pixeldir;
            continue;  // to start of while()
        }

        bright[pixel] = 1.0;
        break;
    }

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
