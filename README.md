# Grove - Multi Switch  [![Build Status](https://travis-ci.com/Seeed-Studio/Grove_Multi_Switch.svg?branch=master)](https://travis-ci.com/Seeed-Studio/Grove_Multi_Switch)

The Grove - Multi Switch can be used to detect the switch position and event like single click/double click/long press, etc.

## Grove 5-Way Tactile Switch

![grove - 5-way tactile switch-x](https://user-images.githubusercontent.com/3901856/41648081-88928592-74aa-11e8-9be7-a9a22b65a70c.png)

## Grove 6-Pos DIP Switch

![grove - 6-pos dip switch-x](https://user-images.githubusercontent.com/3901856/41648114-9ee01e86-74aa-11e8-8f08-a5bca5f94f9a.png)

## Features

- raw status like level high or low
- event detection like single click/double click/long press/level change

## Usage Tips

This library has one classe - `GroveMultiSwitch`. The `GroveMultiSwitch` class does basic read/write with the switch device. Taking the device-i2c-address as an input paramenter.

At the beginning, we need to probe the device through `GroveMultiSwitch::begin()`, called initial setup.
And when the device removed and inserted agagin, we can also probe the cevice through `GroveMultiSwitch::begin()`, called dynamic setup.

The switch will auto detect all the events. All we can do is read the event through `GroveMultiSwitch::getEvent()` interface. It will return NULL when device unexist or the I2C inteface does't work.

When it has detected a event,a valid struct pointer `GroveMultiSwitch::ButtonEvent_t *` will returned.
Check the member `event` to see if there is a event.
Check the member `button` to see each switch event or status.


## Examples

### detect the device existence and list all events

File name: examples/Grove_Switch_Events/Grove_Switch_Events.ino

This example list the device information and events in the serial.
##### Grove 5-Way Tactile Switch
```
Grove Multi Switch
***** Device probe OK *****
Grove 5-Way Tactile Switch Inserted!
A 5 Button/Switch Device BN-5E-0.1
KEY A: RAW - HIGH RELEASED 
KEY B: RAW - HIGH RELEASED 
KEY C: RAW - HIGH RELEASED 
KEY D: RAW - HIGH RELEASED 
KEY E: RAW - HIGH RELEASED 

KEY D: EVENT - SINGLE-CLICK LEVEL-CHANGED 
KEY A: RAW - HIGH RELEASED 
KEY B: RAW - HIGH RELEASED 
KEY C: RAW - HIGH RELEASED 
KEY D: RAW - HIGH RELEASED 
KEY E: RAW - LOW PRESSED 

KEY C: EVENT - SINGLE-CLICK LEVEL-CHANGED 
KEY E: EVENT - LEVEL-CHANGED 
KEY A: RAW - HIGH RELEASED 
KEY B: RAW - HIGH RELEASED 
KEY C: RAW - HIGH RELEASED 
KEY D: RAW - HIGH RELEASED 
KEY E: RAW - LOW PRESSED 

KEY E: EVENT - SINGLE-CLICK LEVEL-CHANGED 
KEY A: RAW - HIGH RELEASED 
KEY B: RAW - HIGH RELEASED 
KEY C: RAW - HIGH RELEASED 
KEY D: RAW - HIGH RELEASED 
KEY E: RAW - LOW PRESSED 
KEY A: RAW - HIGH RELEASED 
KEY B: RAW - HIGH RELEASED 
KEY C: RAW - HIGH RELEASED 
KEY D: RAW - HIGH RELEASED 
KEY E: RAW - LOW PRESSED 

KEY E: EVENT - LONG-PRESS 
KEY A: RAW - HIGH RELEASED 
KEY B: RAW - HIGH RELEASED 
KEY C: RAW - HIGH RELEASED 
KEY D: RAW - HIGH RELEASED 
KEY E: RAW - HIGH RELEASED 

KEY E: EVENT - LEVEL-CHANGED 

```
##### Grove 6-Pos DIP Switch
```
***** Device probe failed *****
***** Device probe failed *****
***** Device probe OK *****
Grove 6-Position DIP Switch Inserted!
A 6 Button/Switch Device BN-6--0.1
POS 1: RAW - LOW ON 
POS 2: RAW - LOW ON 
POS 3: RAW - LOW ON 
POS 4: RAW - LOW ON 
POS 5: RAW - LOW ON 
POS 6: RAW - HIGH OFF 

POS 6: EVENT - LEVEL-CHANGED 
POS 1: RAW - LOW ON 
POS 2: RAW - LOW ON 
POS 3: RAW - LOW ON 
POS 4: RAW - LOW ON 
POS 5: RAW - HIGH OFF 
POS 6: RAW - HIGH OFF 

POS 5: EVENT - LEVEL-CHANGED 
POS 1: RAW - LOW ON 
POS 2: RAW - LOW ON 
POS 3: RAW - HIGH OFF 
POS 4: RAW - HIGH OFF 
POS 5: RAW - HIGH OFF 
POS 6: RAW - HIGH OFF 

POS 3: EVENT - LEVEL-CHANGED 
POS 4: EVENT - LEVEL-CHANGED 
POS 1: RAW - LOW ON 

```

TODO: fix this url when the wiki is available.

For more information, please refer to [wiki page](<wiki url>).

----

This software is written by Peter Yang for seeed studio and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>


[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/grove-human-presence-sensor)](https://github.com/igrigorik/ga-beacon)







