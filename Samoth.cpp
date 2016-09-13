#include "FastLED.h"

// Hardware definitions
#define DATA_PIN    6
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    73

// Software settings
#define BRIGHTNESS         255

// Mapping constants
#define TEMPLE_LENGTH   20
#define FOREHEAD_LENGTH   15

// Maps of continuous linear physical pixel arrays
uint8_t rightTemple[TEMPLE_LENGTH] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};
uint8_t leftTemple[TEMPLE_LENGTH] = {18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};
uint8_t rightForehead[FOREHEAD_LENGTH] = {64, 63, 62, 61, 60, 59, 58, 57, 43, 44, 45, 46, 47, 48, 49};
uint8_t leftForehead[FOREHEAD_LENGTH] = {65, 66, 67, 68, 69, 70, 71, 72, 19,20, 21, 22, 23, 24, 25};

// Instantiate color array object, which is the code-representation of the
// physical pixel strip.
CRGB leds[NUM_LEDS];

void setup() {
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    delay(1000);
}

void loop() {
    // testRightTemple();
    // delay(500);
    // testRightForehead();
    // delay(500);
    // testLeftTemple();
    // delay(500);
    // testLeftForehead();
    // delay(500);

    // testMirrorTemple();
    // testMirrorForehead();

    symmetricalChaser();
    FastLED.show();
    //delay(25);
}

// Send a single red chaser along the path of the physical LED data circuit
void testChaser() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(25);
        FastLED.clear();
    }
}

void testRightTemple() {
    for (uint8_t i = 0; i < TEMPLE_LENGTH; i++) {
        leds[rightTemple[i]] = CRGB::Red;
        FastLED.show();
        delay(25);
        FastLED.clear();
    }
}

void testRightForehead() {
    for (uint8_t i = 0; i < FOREHEAD_LENGTH; i++) {
        leds[rightForehead[i]] = CRGB::Blue;
        mirrorRightFrontCrownToRear();
        FastLED.show();
        delay(25);
        FastLED.clear();
    }
}

void testLeftTemple() {
    for (uint8_t i = 0; i < TEMPLE_LENGTH; i++) {
        leds[leftTemple[i]] = CRGB::Green;
        FastLED.show();
        delay(25);
        FastLED.clear();
    }
}

void testLeftForehead() {
    for (uint8_t i = 0; i < FOREHEAD_LENGTH; i++) {
        leds[leftForehead[i]] = CRGB::Yellow;
        FastLED.show();
        delay(25);
        FastLED.clear();
    }
}

void testMirrorTemple() {
    for (uint8_t i = 0; i < TEMPLE_LENGTH; i++) {
        leds[rightTemple[i]] = CRGB::Red;
        mirrorRightSideToLeft();
        FastLED.show();
        delay(25);
        fadeToBlackBy(leds, NUM_LEDS, 60);
    }
}

void testMirrorForehead() {
    for (uint8_t i = 0; i < FOREHEAD_LENGTH; i++) {
        leds[rightForehead[i]] = CRGB::Blue;
        fullMirror();
        FastLED.show();
        delay(25);
        fadeToBlackBy(leds, NUM_LEDS, 60);
    }
}

void symmetricalChaser() {
    fadeToBlackBy(leds, NUM_LEDS, 60);
    uint8_t templePosition = beatsin8(30, 0, TEMPLE_LENGTH);
    leds[rightTemple[templePosition]] = CRGB::Red;
    uint8_t foreheadPosition = beatsin8(20, 0, FOREHEAD_LENGTH);
    leds[rightForehead[foreheadPosition]] = CRGB::Blue;
    fullMirror();
}

// Utility Functions
void mirrorRightSideToLeft() {
    for (uint8_t i = 0; i < TEMPLE_LENGTH; i++) {
        leds[leftTemple[i]] = leds[rightTemple[i]];
        if (i < FOREHEAD_LENGTH) {
            leds[leftForehead[i]] = leds[rightForehead[i]];
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

void mirrorLeftFrontCrownToRear() {
    leds[32] = leds[19];
    leds[31] = leds[20];
    leds[30] = leds[21];
    leds[29] = leds[22];
    leds[28] = leds[23];
    leds[27] = leds[24];
    leds[26] = leds[25];
}

void fullMirror() {
    mirrorRightFrontCrownToRear();
    mirrorRightSideToLeft();
    mirrorLeftFrontCrownToRear();
}
