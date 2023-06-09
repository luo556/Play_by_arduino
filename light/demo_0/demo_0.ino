/* fill_grad
 *
 * By: Andrew Tuline
 *
 * Date: August, 2015
 *
 * Here's a simple method to blend between a couple of colours across a strand of LED's.
 *
 * It uses FastLED's:
 *
 * - fill_gradient() function to blend between two colours.
 * - beatsin8() to provide a time based smooth varying input to blend().
 *
 * An issue when using the fill_gradient is which way you want the gradient to sweep around the colour wheel. Options are:
 *
 * FORWARD_HUES: hue always goes clockwise
 * BACKWARD_HUES: hue always goes counter-clockwise
 * SHORTEST_HUES: hue goes whichever way is shortest and is the DEFAULT
 * LONGEST_HUES: hue goes whichever way is longest
 *
 * When the start and end colours are animated, the sweep will flip in the other direction at certain points, thus causing a disruption of the smooth gradient.
 * 
 * This routine demonstrates one method to animate the start/end hues and to ensure the fill_gradient remains consistently smooth.
 *
 */

#include "FastLED.h"                                        // FastLED library. Preferably the latest copy of FastLED 2.1.
  
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed global definitions.
#define LED_DT 3                                             // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE WS2812                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 60                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.



void setup() {
  
  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
  
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // This is used by the power management functionality and is currently set at 5V, 500mA.
  FastLED.setBrightness(max_bright);

} // setup()



void loop () {
  
  fill_grad();
  FastLED.show();                         // Power managed display of LED's.
  
} // loop()



void fill_grad() {
  
  uint8_t starthue = beatsin8(5, 0, 255);
  uint8_t endhue = beatsin8(7, 0, 255);
  
  if (starthue < endhue) {
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), FORWARD_HUES);    // If we don't have this, the colour fill will flip around. 
  } else {
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), BACKWARD_HUES);
  }
  
} // fill_grad()
