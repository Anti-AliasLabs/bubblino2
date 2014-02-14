/* bubbles_controller
*
*  copyright Becky Stewart, Anti-Alias Labs
*  becky@antialiaslabs.com
*
*  For the Arduino Yun to be run on the microcontoller
*  while tweet-contoller.py runs on the Linino.
*
*  Bubbles are triggered through following ways:
*  1. A tweet has the hashtag #teambubbles
*  2. It has been 15 minutes since the last tweet
*       (need to keep water moving)
*/

#include <Bridge.h>

// pins
int greenStatusLED = 6;
int yellowStatusLED = 5;
int relay1 = 2;
int relay2 = 3;

// other variables
int bubbleTime = 150;
int bubbleThrottle = 5000;

char tweetValue[4];    // incoming storage of tweet
int lastTweetCount = 0; // keep track of number of tweets
unsigned long lastBubble;
char tweetText[141];

boolean debugMode = true;

//---------------------------------------------
void setup() {
  Bridge.begin();    // initialise Bridge

  if(debugMode) {
  // we don't want Serial when deployed
  // it will make the microcontroller hang
  // waiting for a response
  Serial.begin(9600);
  while ( !Serial );
  }

  // set up pin modes
  pinMode( greenStatusLED, OUTPUT );
  pinMode( yellowStatusLED, OUTPUT );
  pinMode( relay1, OUTPUT );
  pinMode( relay2, OUTPUT );

  // startup bubble
  // also initialises bubble timer
  bubble();
}

//---------------------------------------------
void loop() {
  // Read command output
  Bridge.get("tweets", tweetValue, 4);
  Bridge.get("text", tweetText, 141);
  int tweets = atoi(tweetValue);
  if(debugMode) {
    Serial.println( tweets );
    Serial.println( tweetText );
  }
  
  unsigned long currTime = millis();
  
  // if new tweets or has been 15 minutes
  if ( tweets != lastTweetCount ||
        currTime - lastBubble > (1000*60*15) ) {
    bubble();
    lastTweetCount = tweets;
  }
  delay(1000);  // wait 1 second before you do it again

}

//---------------------------------------------
void bubble() {
  if(debugMode){
    Serial.println("Bubbles!");
  }
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

  // reset the timer for the last bubble
  lastBubble = millis();
}
