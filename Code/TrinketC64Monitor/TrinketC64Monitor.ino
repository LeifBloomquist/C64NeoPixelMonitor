#include <avr/power.h>

#include <Adafruit_NeoPixel.h>
#include <SimpleTimer.h>

#define NUM_PIXELS 8
#define MAX_BRIGHT 128

// Proto board Wiring
// RED      = 5V
// BROWN    = GND

#define PIN_PIXELS 0   // ORANGE = DIN on Neopixels
#define PIN_C64IRQ 1   // WHITE  1A/1Y on 74LS404  
#define PIN_C64ROM 2   // YELLOW 2A/2Y on 74LS404  
#define PIN_C64GAM 3   // BLUE   3A/3Y on 74LS404  
#define PIN_C64NMI 4   // GREEN  4A/4Y on 74LS404  

// TODO: PinChangeInterrupts?   https://github.com/NicoHood/PinChangeInterrupt

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);

// The timer object
SimpleTimer timer;

void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__) 
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // End of trinket special code

    // Inputs
    pinMode(PIN_C64IRQ, INPUT_PULLUP);
    pinMode(PIN_C64ROM, INPUT_PULLUP);
    pinMode(PIN_C64GAM, INPUT_PULLUP);
    pinMode(PIN_C64NMI, INPUT_PULLUP);
                             
    timer.setInterval(100, Animate);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'   
}

int pixel = 4;
int pixeldir = 1;
float bright[NUM_PIXELS] = { 0 };

byte c64irq = 0;
byte c64rom = 0;
byte c64gam = 0;
byte c64nmi = 0;

void loop()
{
    c64irq = digitalRead(PIN_C64IRQ);
    c64rom = digitalRead(PIN_C64ROM);
    c64gam = digitalRead(PIN_C64GAM);
    c64nmi = digitalRead(PIN_C64NMI);

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

    Fade();
    Refresh();
}

void Fade()
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        bright[i] *= 0.5;
    }
}

void Refresh()
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        byte rgbval = bright[i] * MAX_BRIGHT;

       // if (c64irq == 1)       // IRQ = RED
       // {
        //    strip.setPixelColor(i, rgbval, 0, 0);
        //}   
        //else 
        if (c64rom == 1)  // EXROM = GREEN
        {
            strip.setPixelColor(i, 0, rgbval, 0);
        }
        else if (c64gam == 1)  // GAME = YELLOW
        {
            strip.setPixelColor(i, rgbval, rgbval, 0);
        }
        else if (c64nmi == 1)  // NMI = WHITE
        {
            strip.setPixelColor(i, rgbval, rgbval, rgbval);
        }
        else                  // Normal = BLUE
        {
            strip.setPixelColor(i, 0, 0, rgbval);
        }
    }
    strip.show();
}
