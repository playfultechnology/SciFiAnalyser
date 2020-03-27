/**
 * Sci-Fi Material Analysis Console
 * (c) 2020 Playful Technology
 *
 * Inserting a sample and pressing the "analyse" button causes a series of "beep boop" noises 
 * while an LED ring flashes for a few seconds (the colour of which can be changed by sliding a 
 * linear potentiometer slide). Then an LCD screen displays the result of the material analysis -
 * a string of technobabble.
 */
 
// DEFINES
// The number of LEDs in the Neopixel ring
#define NUM_LEDS 16
// DEFINES don't have to be values - they can be functions too. This returns the number of elements
// in the array of technobabble below
#define numElements (sizeof(elements)/sizeof(char *))

// INCLUDES
// Generic base library for any module that use I2C communication
#include <Wire.h>
// For the LCD display
#include <LiquidCrystal_PCF8574.h>
// For the Neopixel ring
#include <FastLED.h>

// CONSTANTS
// Define the pins on the Arduino to which each component is connected
const byte buttonPin = A2;
const byte sliderPin = A7;
const byte neoPixelPin = 2;
const byte piezoPin = 4;
// Add as many random words to this list as you want to be displayed as "results" of the analysis
const char* elements[] = {"Adamantium", "Arcanite", "Azmodium", "Balonium", "Biphase Carbide",
"Blastolene", "Calculon", "Collapsium", "Corruptite", "Cosmonium", "Dronk", "Eludium",
"Etherium", "Flooftron", "Flubber", "Froon", "Geranium", "Hadox", "Illudium", "Jumbonium", "Kilp", 
"Lux", "Magicite", "Mithril", "Neverendium", "Octagen",
"Octocellulose", "Phrostlite", "Planktanium", "Plasphalt", "Poo", "Porkanite",
"Protonite", "Quadox", "Quantium", "Rage-90", "Stupidium", "Tictacium", "Thaum", "Trioxyn", "Turbonium", "Twonk", "Unobtainium",
"Vespene", "Wangel", "Xylem", "Yorbis", "Zexonite"};

// GLOBALS
// Create an object to address the 16x2 LCD display, on I2C address 0x27
LiquidCrystal_PCF8574 lcd(0x27);
// Create an array of pixels in the ring
CRGB leds[NUM_LEDS];
// The current hue with which pixels are shown
int hue;

void setup() {
  // Initialise the button
  pinMode(buttonPin, INPUT_PULLUP);

  // Intialise the LEDs
  FastLED.addLeds<NEOPIXEL, neoPixelPin>(leds, NUM_LEDS);

  // Initialise the I2C interface
  Wire.begin();
  
  // Initialise the display
  lcd.begin(16,2);
  lcd.setBacklight(0);
  lcd.clear();

  // Test audio
  pinMode(piezoPin, OUTPUT);
  tone(piezoPin, 440, 1000);

  // Take a reading from an (unconnected) analog pin to get random seed value
  randomSeed(analogRead(A0));
}

void loop(){

  // If the "analyse" button has been pressed
  if(digitalRead(buttonPin) == LOW){

    // Play a random sound effect
    soundFX(100.0, 30.0);

    // Turn on the display
    lcd.clear();
    lcd.setBacklight(255);
    lcd.home();
    lcd.print("ANALYSING...");

    // How many iterations of analysis before result displayed
    for(int cycles=0; cycles<5; cycles++){
      // Play a random sound effect
      for(int i=0; i<3; i++){
        soundFX(3000.0, i);
      }
      // Loop around the LED ring
      for(int i=0; i<NUM_LEDS; i++){
        // Read the value of the "wavelength modulator" slider
        int sliderValue = analogRead(sliderPin);
        // Use this value to set the hue of the LEDs
        hue = map(sliderValue, 0, 1023, 0, 255);
        leds[i] = CHSV(hue, 255, 96);
        // Make the trail of LEDs fade out slowly over time
        fadeToBlackBy( leds, NUM_LEDS, 30);
        FastLED.show();
        delay(80);
      }
    }

    // Finished analysing, so pick a random string from the elements array
    char* result = elements[random(0, numElements)];

    // Display the result
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RESULT:");
    lcd.setCursor(0,1);
    lcd.print(result);
    
    // Play a tone
    tone(piezoPin, 440, 1000);

    for(int i=0; i<100; i++) {
      // Fade to black
      fadeToBlackBy( leds, NUM_LEDS, 5);
      FastLED.show();
      delay(50);
    }
    lcd.clear();
    lcd.setBacklight(0);
  }

  // Fade to black
  fadeToBlackBy( leds, NUM_LEDS, 5);
  FastLED.show();
}

// Plays a pseudo-random sound effect
void soundFX(float amplitude, float period){
  // Calculate a delay based on desired periodicity and elapsed time
  int uDelay = 2 + amplitude + amplitude * sin(millis()/period);
  // Send oscillating tone
  for(int i=0; i<10; i++){
    digitalWrite(piezoPin,HIGH);
    delayMicroseconds(uDelay);
    digitalWrite(piezoPin,LOW);
    delayMicroseconds(uDelay);
  }
}
