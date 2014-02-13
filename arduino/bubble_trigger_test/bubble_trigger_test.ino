/* BubbleTriggerTest
*
*  copyright Becky Stewart, Anti-Alias Labs
*  becky@antialiaslabs.com
*
*  Tests that bubbling is occurring correctly by cycling 
*  through bubbling.
*/

// pins 
int greenStatusLED = 6;
int yellowStatusLED = 5;
int relay1 = 2;
int relay2 = 3;

// other variables
int bubbleTime = 150;
int bubbleThrottle = 5000;

void setup() {
  // set up pin modes
  pinMode( greenStatusLED, OUTPUT );
  pinMode( yellowStatusLED, OUTPUT );
  pinMode( relay1, OUTPUT );
  pinMode( relay2, OUTPUT );
  
  // start serial
  //Serial.begin( 9600 );
  //while( !Serial );
  
  // print welcome message at startup
  //Serial.println( "Testing the bubbler");

}

void loop() {
  
  // bubble a test series
  //Serial.println("...bubbles");
  bubble();
  
  delay( 500 );
  
  bubble(); 
  
  
  delay( 30000 );
}

void bubble() {
  // turn on status LED
  digitalWrite( greenStatusLED, HIGH );
  digitalWrite( yellowStatusLED, LOW );
  
  // trigger relay
  digitalWrite( relay1, HIGH );
  digitalWrite( relay2, HIGH );
  
  // wait to create bubbles
  delay( bubbleTime );
  
  // stop relay
  digitalWrite( relay1, LOW );
  digitalWrite( relay2, LOW );
  
  // update status LEDs
  digitalWrite( greenStatusLED, LOW);
  digitalWrite( yellowStatusLED, HIGH);
  
  // wait to throttle bubbling
  delay( bubbleThrottle );
}
