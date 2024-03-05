# Necromancer car firmware 

Firmware aka Arduino sketch for "Necromancer car"
project running on ESP8266

# Necromancer car
TL;DR: Necromancer car is project about taking out 
electronics of broken RC car replacing it with DIY one and 
control it with Steam Deck

I've had broken RC car and for some time wanted to take (already broken)
electronics out and replace it with my own.
When i saw Redditors speculating that Steam Deck wasn't used as RC
controlled idea expanded.

On the car side hardware consist of ESP8266 (powered by batteries, held in build into car chassis holder, rough boost converter set at 5V)  driving L9110S double H bridge module (powered by batteries directly) ESP8266 hosts it’s own access point (aka hot spot)  and listens for UDP packets that consist of 3 bytes:

 First byte holds throttle value 
 Second bytes wheel drive 
 Third holds flags (7 = most significant bit- 0 = least significant): 
    7 - is throttle negative ( 1 - yes, 0 - no)
    6 - is steering wheel negative ( 1- yes, 0 - no)

    1 - is head light on
    0 - is back light on