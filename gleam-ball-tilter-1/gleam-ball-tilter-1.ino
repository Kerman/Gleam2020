// A single pixel 'ball' starts in the center of the strip. tilting the sensor causes the ball to move appropriately
//
// (The priginal idea was to simulate gravity, and have multiple balls) 

#include <FastLED.h>
#include <Wire.h>                 // Must include Wire library for I2C
#include "SparkFun_MMA8452Q.h"    // Click here to get the library: http://librarymanager/All#SparkFun_MMA8452Q



#define NUM_LEDS 300
float STRIP_LENGTH_METERS = 5; // the length of the strip in meters. (this will be really cool later.... )

#define NUM_BALLS  1              // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way)

CRGB leds[NUM_LEDS];   

int   pos[NUM_BALLS] ;            // The integer position of the dot on the strip (LED index)
float realpos[NUM_BALLS];         // The position of the ball, in meters
float velocity[NUM_BALLS];        // velocity of each ball 

MMA8452Q accel;                   // create instance of the MMA8452 class

void setup() {
  
  Wire.begin(); 

  //debug
  Serial.begin(38400);
  Serial.println("G1");
  Serial.print(F("L: "));
  Serial.println(NUM_LEDS);
  Serial.print(F("SLM: "));
  Serial.println(STRIP_LENGTH_METERS);

  
  if (accel.begin() == false) {
    Serial.println("Accelerometer Not Detected. Did you change the address to 0x1c");
    while (1);
  }
  delay(3000); // delay in case turning on the LED's overdraws power.

  FastLED.addLeds<WS2812,7,RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);


 for (int i = 0 ; i < NUM_BALLS ; i++) {    // Initialize variables
    //pos[i] = NUM_LEDS/2;                              // Ball start
    realpos[i] = STRIP_LENGTH_METERS/2;
    velocity[i] = 0;
  }

}
  unsigned long lastupdate=millis();
  unsigned long thisupdate=1;
void loop() {
  
    //figure out how long it has been since we last updated our position
    thisupdate = (millis() - lastupdate); 
    lastupdate = millis(); 
    //Serial.print(thisupdate);

    // adjust accellerations
    for (int i = 0; i < NUM_BALLS; i++) {
    if (accel.available()) { // wait for accel data
      velocity[i]=(accel.getCalculatedY()*9.81)/1000; // someday this should work like real gravity
    }

    //debug
    Serial.print("Velocity ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(velocity[i],4);

    // move the ball
    realpos[i]=realpos[i]+(velocity[i]);
    if (realpos[i]>STRIP_LENGTH_METERS) realpos[i]=STRIP_LENGTH_METERS;
    if (realpos[i]<0) realpos[i]=0;
    pos[i]=round(realpos[i]/(STRIP_LENGTH_METERS/NUM_LEDS));
    if (pos[i]>NUM_LEDS-1) pos[i]=NUM_LEDS-1; 

    //debug
    Serial.print(" RealPos: ");
    Serial.print(realpos[i],4);
    Serial.print(" Pos: ");
    Serial.print(pos[i]);
    Serial.print(" Time: ");
    Serial.println(thisupdate);
    //Serial.print(" ");
    //Serial.println(lastupdate);
    
  }
    for (int i = 0 ; i < NUM_BALLS ; i++) leds[pos[i]] = CHSV( uint8_t (i * 40) , 255, 255);
    FastLED.show();
    
    //all black to set up for the next round
    for (int i = 0 ; i < NUM_BALLS ; i++) {
    
    leds[pos[i]] = CRGB::Black;
}
    
  }
  
