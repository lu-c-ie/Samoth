#include "FastLED.h"

// Hardware definitions
#define DATA_PIN    6
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    73

// Mapping constants
#define TEMPLE_LENGTH   20
#define FORHEAD_LENGTH   15

// Software settings
#define BRIGHTNESS         255

// Instantiate color array object, which is the code-representation of the
// physical pixel strip.
CRGB leds[NUM_LEDS];

// Maps of continuous linear physical pixel arrays
uint8_t rightTemple[TEMPLE_LENGTH] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};
uint8_t leftTemple[TEMPLE_LENGTH] = {18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};
uint8_t rightForehead[FORHEAD_LENGTH] = {64, 63, 62, 61, 60, 59, 58, 57, 43, 44, 45, 46, 47, 48, 49};
uint8_t leftForhead[FORHEAD_LENGTH] = {65, 66, 67, 68, 69, 70, 71, 72, 19,20, 21, 22, 23, 24, 25};

void setup() {
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    symmetricalChaser();
}

// Send a single red chaser along the path of the physical LED data circuit
void testChaser() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(20);
        FastLED.clear();
    }
}

void symmetricalChaser() {
    fadeToBlackBy(leds, NUM_LEDS, 20);
    uint8_t templePosition = beatsin8(60, 0, TEMPLE_LENGTH);
    leds[rightTemple[templePosition]] = CRGB::Red;
    uint8_t forheadPosition = beatsin8(30, 0, FORHEAD_LENGTH);
    leds[rightForehead[forheadPosition]] = CRGB::Blue;
    mirrorRightFrontCrownToRear();
    mirrorRightSideToLeft();
}

// Utility Functions
void mirrorRightSideToLeft() {
    for (uint8_t i = 0; i < TEMPLE_LENGTH; i++) {
        leds[leftTemple[i]] = leds[rightTemple[i]];
        if (i < FORHEAD_LENGTH) {
            leds[leftForhead[i]] = leds[rightForehead[i]];
        }
    }
}

void mirrorRightFrontCrownToRear() {
    leds[56] = leds[43];
    leds[55] = leds[44];
    leds[54] = leds[45];
    leds[53] = leds[46];
    leds[52] = leds[47];
    leds[51] = leds[48];
    leds[50] = leds[49];
}
