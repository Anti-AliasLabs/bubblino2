#include <Process.h>

Process twitter;     // process to read tweets

void setup() {
  Bridge.begin();    // initialise Bridge
  Serial.begin(9600);   // initialise serial

  while (!Serial);  // wait for Serial Monitor to open
  Serial.println( "Welcome to the BubblinO2" );

  // run the Python script on the Linino
  twitter.runShellCommend( "python /usr/bubblino2/linino/tweet-controller.py" );
}

void loop() {
  // Read command output
  while (twitter.available()) {
    int result = twitter.parseInt();			// look for an integer
    Serial.println(result);			// print the number as well
  }
  delay(5000);  // wait 5 seconds before you do it again

}
