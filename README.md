bubblino2
=========

Reads tweets and triggers an bubbles in a water tank using an Arduino Yun.


Linino
------
Runs Python 2.7 with the following requirements:

* oauthlib==0.6.1
* requests==2.1.0
* requests-oauthlib==0.4.0
* twython==3.1.2
* wsgiref==0.1.2

`tweet-controller.py` is launched at boot via `/etc/init.d/tweetinit`. `tweet-controller.py` listens to the filtered Twitter stream that containes '#teambubbles'. The Bridge is used to communicate with the microcontroller.

Place your Twitter credentials in `credentials.py`.

Due to still not understood reasons, the python script can only be launched manually via ssh or automatically via init.d. The script won't successfully authenticate to Twitter if launched via `rc.local`. It seems that the ethernet/Wifi interface won't be fully launched in time.

Additionally, some other interaction with the network needs to happen before launching the Python script. Here, Twitter is first pinged before starting Python.

The script `tweetinit` should be placed in `/etc/init.d/` and then run `/etc/init.d/tweetinit enable` to generate a symlink to `/etc/rc.d/`.


AtMega
------
The microncontroller listens for an updated number of tweets received on the Bridge and then handles triggering the relay for the bubble machine.

If no tweets are received in 15 minutes, the machine is bubbled just to keep the water moving.

The variable `debugMode` can be set to output to `serial`. It will stall the microcontroller from progressing until an external serial connection is made (not with the Linino), so don't leave on in deployed code.
