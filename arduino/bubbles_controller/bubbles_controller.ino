/* bubbles_controller
*
*  copyright Becky Stewart, Anti-Alias Labs
*  becky@antialiaslabs.com
*
*  For the Arduino Yun to be run on the microcontoller
*  while tweet-contoller.py runs on the Linino.
*
*  Bubbles are triggered through following ways:
*  1. A tweet has the hashtag #teambubble
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

char tweetValue[2];    // incoming storage of tweet
int lastTweetCount = 0; // keep track of number of tweets
unsigned long lastBubble; 

//---------------------------------------------
void setup() {
  Bridge.begin();    // initialise Bridge

  // set up pin modes
  pinMode( greenStatusLED, OUTPUT );
  pinMode( yellowStatusLED, OUTPUT );
  pinMode( relay1, OUTPUT );
  pinMode( relay2, OUTPUT );

  // initialise the timer for the last bubble
  lastBubble = millis();
}

//---------------------------------------------
void loop() {
  // Read command output
  Bridge.get("tweets", tweetValue, 2);
  int tweets = atoi(tweetValue);


  delay(5000);  // wait 5 seconds before you do it again

  // startup bubble
  bubble();
}

//---------------------------------------------
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
  
  // initialise the timer for the last bubble
  lastBubble = millis();
}
