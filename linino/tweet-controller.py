#!/usr/bin/python
"""
 tweet-controller.py

 copyright Becky Stewart, Anti-Alias Labs
 becky@antialiaslabs.com

 For the Arduino Yun to be run on the Linino
 while bubbles_contoller runs on the microcontroller.

 Bubbles are triggered through following ways:
  1. A tweet has the hashtag #teambubble
  2. It has been 15 minutes since the last tweet
       (need to keep water moving)
"""

import sys  
sys.path.insert(0, '/usr/lib/python2.7/bridge/') 
import time
import thread
from credentials import *                                     
from bridgeclient import BridgeClient as bridgeclient
import twython    
# following import deals with namespace clashes 
from twython.streaming.api import (_transparent_params, 
                                    is_py3,
                                    requests
                                    )                               
import simplejson
                                              

class TeamBubbleStreamer(twython.TwythonStreamer):
    tweetCounter = 0
    value = bridgeclient() 
    def on_success(self, data):
        if 'text' in data:
            # print data['text'].encode('utf-8') 
            self.tweetCounter+=1
            self.tweetCounter = self.tweetCounter%1000
            self.value.put('tweets', "%03s"%self.tweetCounter)
            self.value.put('text', data['text'])
            
                          

    def on_error(self, status_code, data):
        print status_code

        # Want to stop trying to get data because of the error?
        # Uncomment the next line!
        self.value.put('text', '401')
        self.disconnect()

    # namespace conflicts with json modules between Bridge.py and 
    # Twython are resolved through this rather gross hack
    def _request(self, url, method='GET', params=None):
        """Internal stream request handling"""
        self.connected = True
        retry_counter = 0

        method = method.lower()
        func = getattr(self.client, method)
        params, _ = _transparent_params(params)

        def _send(retry_counter):
            requests_args = {}
            for k, v in self.client_args.items():
            # Maybe this should be set as a class variable and only done once?
                if k in ('timeout', 'allow_redirects', 'verify'):
                    requests_args[k] = v

            while self.connected:
                try:
                    if method == 'get':
                        requests_args['params'] = params
                    else:
                        requests_args['data'] = params

                    response = func(url, **requests_args)
                except requests.exceptions.Timeout:
                    self.on_timeout()
                else:
                    if response.status_code != 200:
                        self.on_error(response.status_code, response.content)

                    if self.retry_count and (self.retry_count - retry_counter) > 0:
                        time.sleep(self.retry_in)
                        retry_counter += 1
                        _send(retry_counter)

                    return response

        while self.connected:
            response = _send(retry_counter)

            for line in response.iter_lines(self.chunk_size):
                if not self.connected:
                    break
                if line:
                    try:
                        if is_py3:
                            line = line.decode('utf-8')
                        data = simplejson.loads(line)
                    except ValueError:  # pragma: no cover
                        self.on_error(response.status_code, 'Unable to decode response, not valid JSON.')
                    else:
                        if self.on_success(data):  # pragma: no cover
                            for message_type in self.handlers:
                                if message_type in data:
                                    handler = getattr(self, 'on_' + message_type, None)
                                    if handler and callable(handler) and not handler(data.get(message_type)):
                                        break

        response.close()



print 'Starting...'
# time.sleep(300)
print 'Starting Tweet Listener'

stream = TeamBubbleStreamer(APP_KEY, APP_SECRET, ACCESS_TOKEN, ACCESS_SECRET)
stream.statuses.filter(track='#teambubbles')

# print 'Trying again'
# time.sleep(120)
# streamSecondChance = TeamBubbleStreamer(APP_KEY, APP_SECRET, ACCESS_TOKEN, ACCESS_SECRET)
# streamSecondChance.statuses.filter(track='#teambubbles')
# stream.user()
