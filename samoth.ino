#include "FastLED.h"

// Hardware definitions
#define DATA_PIN    6
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    69

// Software settings
#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  120
#define RAINBOW_CYCLER_MILLISECONDS  20
#define PATTERN_CYCLER_SECONDS  10

// Utility function
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Mapping constants
#define TEMPLE_LENGTH   20 //19, 6, 6, 10, 6, 6, 8, 8
#define FOREHEAD_LENGTH   14

// Maps of continuous linear physical pixel arrays
uint8_t rightTemple[TEMPLE_LENGTH] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
uint8_t leftTemple[TEMPLE_LENGTH] = {18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
uint8_t rightForehead[FOREHEAD_LENGTH] = {60, 59, 58, 57, 56, 55, 54, 53, 41, 42, 43, 44, 45, 46};
uint8_t leftForehead[FOREHEAD_LENGTH] = {61, 62, 63, 64, 65, 66, 67, 68, 19, 20, 21, 22, 23, 24};

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

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {confetti, sinelon, juggle, bpm, addGlitterAdd, rainbowGlitter, rainbow, rainbowWithGlitter};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

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

    // symmetricalChaser();
    // FastLED.show();
    //delay(25);

    // Call the current pattern function once, updating the 'leds' array
    gPatterns[gCurrentPatternNumber]();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS(RAINBOW_CYCLER_MILLISECONDS) { gHue++; } // slowly cycle the "base color" through the rainbow
    EVERY_N_SECONDS(PATTERN_CYCLER_SECONDS) { nextPattern(); } // change patterns periodically
}

// Animation functions

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
    fadeToBlackBy(leds, NUM_LEDS, 5);
    uint8_t templePosition = beatsin8(30, 0, TEMPLE_LENGTH);
    leds[rightTemple[templePosition]] = CRGB::Red;
    uint8_t foreheadPosition = beatsin8(20, 0, FOREHEAD_LENGTH);
    leds[rightForehead[foreheadPosition]] = CRGB::Blue;
    fullMirror();
}


void rainbow()  {
    // FastLED's built-in rainbow generator
    CRGB temple[TEMPLE_LENGTH];
    CRGB forehead[FOREHEAD_LENGTH];
    fill_rainbow(temple, TEMPLE_LENGTH, gHue, 7);
    fill_rainbow(forehead, FOREHEAD_LENGTH, gHue, 7);
    for (uint8_t i = 0; i < TEMPLE_LENGTH; i++) {
      leds[rightTemple[i]] = temple[i];
      if (i < FOREHEAD_LENGTH) {
          leds[rightForehead[i]] = forehead[i];
      }
    }
    fullMirror();
}

void rainbowWithGlitter()   {
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow();
    addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter) {
    if(random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
    }
}

void addGlitterAdd()
{
    fadeToBlackBy(leds, NUM_LEDS, 50);
    leds[random16(NUM_LEDS)] += CRGB::White;
    delay(15);
}

void rainbowGlitter()
{
    CRGB onePix[1];
    fill_rainbow (onePix, 1, gHue, 7);
    leds[random16(NUM_LEDS)] = onePix[0];
    fadeToBlackBy(leds, NUM_LEDS, 50);
    delay(15);
}

void confetti()
{
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(leds, NUM_LEDS, 10);
    uint8_t templePosition = random16(TEMPLE_LENGTH);
    uint8_t foreheadPosition = random16(FOREHEAD_LENGTH);
    leds[rightTemple[templePosition]] += CHSV(gHue + random8(64), 200, 255);
    leds[rightForehead[foreheadPosition]] += CHSV(gHue + random8(64), 200, 255);
    fullMirror();
}

void sinelon()
{
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(leds, NUM_LEDS, 20);
    uint8_t templePosition = beatsin16(13, 0, TEMPLE_LENGTH);
    uint8_t foreheadPosition = beatsin16(13, 0, FOREHEAD_LENGTH);
    leds[rightTemple[templePosition]] += CHSV(gHue, 255, 192);
    leds[rightForehead[foreheadPosition]] += CHSV(gHue, 255, 192);
    fullMirror();
}

void bpm()
{
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for (uint8_t i = 0; i < TEMPLE_LENGTH; i++) {
        leds[rightTemple[i]] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
        if (i < FOREHEAD_LENGTH) {
            leds[rightForehead[i]] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
        }
    }
    fullMirror();
}

void juggle() {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(leds, NUM_LEDS, 20);
    byte dothue = 0;
    for (int i = 0; i < 8; i++) {
        uint8_t templePosition = beatsin16(i + 7, 0, TEMPLE_LENGTH);
        uint8_t foreheadPosition = beatsin16(i + 7, 0, FOREHEAD_LENGTH);
        leds[rightTemple[templePosition]] |= CHSV(dothue, 200, 255);
        leds[rightForehead[foreheadPosition]] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
    fullMirror();
}

// Utility Functions
void mirrorRightSideToLeft() {
    for (uint8_t i = 0; i < FOREHEAD_LENGTH; i++) {
        leds[leftForehead[i]] = leds[rightForehead[i]];
        if (i < 9) {
            leds[leftTemple[i]] = leds[rightTemple[i]];
        }
    }
}

void mirrorRightFrontCrownToRear() {
    leds[52] = leds[41];
    leds[51] = leds[42];
    leds[50] = leds[43];
    leds[49] = leds[44];
    leds[48] = leds[45];
    leds[47] = leds[46];
}

void mirrorLeftFrontCrownToRear() {
    leds[30] = leds[19];
    leds[29] = leds[20];
    leds[28] = leds[21];
    leds[27] = leds[22];
    leds[26] = leds[23];
    leds[25] = leds[24];
}

void fullMirror() {
    mirrorRightFrontCrownToRear();
    mirrorRightSideToLeft();
    mirrorLeftFrontCrownToRear();
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
