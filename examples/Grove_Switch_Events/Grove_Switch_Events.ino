/*
    Name: Grove_Switch_Events.ino
 	Grove 5-Way Tactile & Grove 6-Position DIP Switch demo.


    Author: turmary <turmary@126.com>
    Copyright (c) 2018 Seeed Corporation.

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
    LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Grove_Multi_Switch.h"

GroveMultiSwitch mswitch[1];
const char* grove_5way_tactile_keys[] = {
    "KEY A",
    "KEY B",
    "KEY C",
    "KEY D",
    "KEY E",
};
const char* grove_6pos_dip_switch_keys[] = {
    "POS 1",
    "POS 2",
    "POS 3",
    "POS 4",
    "POS 5",
    "POS 6",
};

const char** key_names;

int deviceDetect(void) {
    if (!mswitch->begin()) {
        Serial.println("***** Device probe failed *****");
        return -1;
    }

    Serial.println("***** Device probe OK *****");
    if (PID_VAL(mswitch->getDevID()) == PID_5_WAY_TACTILE_SWITCH) {
        Serial.println("Grove 5-Way Tactile Switch Inserted!");
        key_names = grove_5way_tactile_keys;
    } else if (PID_VAL(mswitch->getDevID()) == PID_6_POS_DIP_SWITCH) {
        Serial.println("Grove 6-Position DIP Switch Inserted!");
        key_names = grove_6pos_dip_switch_keys;
    }

    // enable event detection
    mswitch->setEventMode(true);

    // report device model
    Serial.print("A ");
    Serial.print(mswitch->getSwitchCount());
    Serial.print(" Button/Switch Device ");
    Serial.println(mswitch->getDevVer());
    return 0;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Grove Multi Switch");

    // Initial device probe
    if (deviceDetect() < 0) {
        Serial.println("Insert Grove 5-Way Tactile");
        Serial.println("or Grove 6-Position DIP Switch");
        for (;;);
    }

    return;
}

void loop() {
    GroveMultiSwitch::ButtonEvent_t* evt;

    delay(1);

    evt = mswitch->getEvent();
    if (!evt) {
        // dynamic device probe
        deviceDetect();
        delay(1000);
        return;
    }

    if (!(evt->event & GroveMultiSwitch::BTN_EV_HAS_EVENT)) {
        #if 0
        Serial.print("No event, errno = ");
        Serial.println(mswitch->errno);
        #endif
        return;
    }

    for (int i = 0; i < mswitch->getSwitchCount(); i++) {
        Serial.print(key_names[i]);
        Serial.print(": RAW - ");
        Serial.print((evt->button[i] & GroveMultiSwitch::BTN_EV_RAW_STATUS) ?
                     "HIGH " : "LOW ");
        if (PID_VAL(mswitch->getDevID()) == PID_5_WAY_TACTILE_SWITCH) {
            Serial.print((evt->button[i] & GroveMultiSwitch::BTN_EV_RAW_STATUS) ?
                         "RELEASED " : "PRESSED ");
        } else if (PID_VAL(mswitch->getDevID()) == PID_6_POS_DIP_SWITCH) {
            Serial.print((evt->button[i] & GroveMultiSwitch::BTN_EV_RAW_STATUS) ?
                         "OFF " : "ON ");
        }
        Serial.println("");
    }

    for (int i = 0; i < mswitch->getSwitchCount(); i++) {
        if (evt->button[i] & ~GroveMultiSwitch::BTN_EV_RAW_STATUS) {
            Serial.println("");
            Serial.print(key_names[i]);
            Serial.print(": EVENT - ");
        }
        if (evt->button[i] & GroveMultiSwitch::BTN_EV_SINGLE_CLICK) {
            Serial.print("SINGLE-CLICK ");
        }
        if (evt->button[i] & GroveMultiSwitch::BTN_EV_DOUBLE_CLICK) {
            Serial.print("DOUBLE-CLICK ");
        }
        if (evt->button[i] & GroveMultiSwitch::BTN_EV_LONG_PRESS) {
            Serial.print("LONG-PRESS ");
        }
        if (evt->button[i] & GroveMultiSwitch::BTN_EV_LEVEL_CHANGED) {
            Serial.print("LEVEL-CHANGED ");
        }
    }
    Serial.println("");
}
