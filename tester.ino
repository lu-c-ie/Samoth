#ifdef INCLUDE_TESTER

#include "FastLED.h"

// Hardware definitions:
#define DATA_PIN    6
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    73

// Software settings
#define BRIGHTNESS         255

// Instantiate color array object, which is the code-representation of the
// physical pixel strip.
CRGB leds[NUM_LEDS];

void setup() {
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    testChaser();
}

// Send a single red chaser along the of the path physical LED data circuit:
void testChaser() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(20);
        FastLED.clear();
    }
}

#endif //INCLUDE_TESTER
