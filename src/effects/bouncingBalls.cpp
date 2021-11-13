/*
 * Bouncing Balls from Stewart Taylor (HSIBOY)
 */
#include "effects/bouncingBalls.h"


#define GRAVITY           -1                  // Downward (negative) acceleration of gravity in m/s^2
#define h0                1                   // Starting height, in meters, of the ball (strip length)

float h[MAX_BALLS] ;                          // An array of heights
float vImpact0 = sqrt( -2 * GRAVITY * h0 );   // Impact velocity of the ball when it hits the ground if "dropped" from the top of the strip
float vImpact[MAX_BALLS] ;                    // As time goes on the impact velocity will change, so make an array to store those values
float tCycle[MAX_BALLS] ;                     // The time since the last time the ball struck the ground
int   pos[MAX_BALLS] ;                        // The integer position of the dot on the strip (LED index)
long  tLast[MAX_BALLS] ;                      // The clock time of the last ground strike
float COR[MAX_BALLS] ;                        // Coefficient of Restitution (bounce damping)

void SetupBouncingBalls() {
  for (int i = 0 ; i < MAX_BALLS ; i++) {    // Initialize variables
    tLast[i] = millis();
    h[i] = random(3,10)/10;
    pos[i] = 0;                              // Balls start on the ground
    vImpact[i] = vImpact0;                   // And "pop" up at vImpact0
    tCycle[i] = 0;
    COR[i] = 0.90 - float(i)/pow(MAX_BALLS,2); 
  }
}

void BouncingColoredBalls(int balls) {
  
  if(tLast[0]==0) {
    SetupBouncingBalls();
  }
  for (int i = 0 ; i < balls ; i++) {
    tCycle[i] =  millis() - tLast[i] ;     // Calculate the time since the last time the ball was on the ground

    // A little kinematics equation calculates positon as a function of time, acceleration (gravity) and intial velocity
    h[i] = 0.5 * GRAVITY * pow( tCycle[i]/1000 , 2.0 ) + vImpact[i] * tCycle[i]/1000;

    if ( h[i] < 0 ) {                      
      h[i] = 0;                            // If the ball crossed the threshold of the "ground," put it back on the ground
      vImpact[i] = COR[i] * vImpact[i] ;   // and recalculate its new upward velocity as it's old velocity * COR
      tLast[i] = millis();

      if ( vImpact[i] < 0.1 ) vImpact[i] = vImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
    }
    pos[i] = round( h[i] * (NUM_LEDS - 1) / h0);       // Map "h" to a "pos" integer index position on the LED strip
  }

  //Choose color of LEDs, then the "pos" LED on
  for (int i = 0 ; i < balls ; i++) 
    setPixel(pos[i], CHSV(uint8_t (i * 40) , 255, 255));
  
  showStrip();
  //Then off for the next loop around
  for (int i = 0 ; i < balls ; i++) {
    leds[pos[i]] = CRGB::Black;
  }
}
