/*
    Name: Grove_Multi_Switch.h
 	Grove 5-Way Tactile & Grove 6-Position DIP Switch library.


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
#ifndef __GROVE_MULTI_SWITCH__
#define __GROVE_MULTI_SWITCH__

#include <stdint.h>
#include <Wire.h>

class GroveMultiSwitch {
  public:
    enum button_event_t {
        BTN_EV_NO_EVENT     = 0,
        BTN_EV_HAS_EVENT    = 0x80000000UL,

#define RAW_DIGITAL_BTN_PRESSED		0
#define RAW_DIP_SWITCH_ON		0
        BTN_EV_RAW_STATUS   = 1UL << 0,
        BTN_EV_SINGLE_CLICK = 1UL << 1,
        BTN_EV_DOUBLE_CLICK = 1UL << 2,
        BTN_EV_LONG_PRESS   = 1UL << 3,
        BTN_EV_LEVEL_CHANGED = 1UL << 4,
    };

    struct ButtonEvent_t {
        uint32_t event;			// BTN_EV_NO_EVENT/BTN_EV_HAS_EVENT
#define BUTTON_MAX		6
        uint8_t button[BUTTON_MAX];	// one button each element
    };

    /**
        Create GroveMultiSwitch object.

        @param addr - The i2c address of device.
        @return - none
    */
#define _MULTI_SWITCH_DEF_I2C_ADDR	0x03	// The device i2c address in default
    GroveMultiSwitch(uint8_t addr = _MULTI_SWITCH_DEF_I2C_ADDR) {
        m_devAddr = addr;
        m_i2c = &Wire;
        version = 0;
    }

    /**
        Initialize the switch interface

        @return  bool - true for success or false for fail
    */
    bool begin(void) {
        m_i2c->begin();
        probeDevID();
        // versions needed
        getDevVer();
        m_btnCnt = getSwitchCount();
        return (m_btnCnt > 0);
    }

    /**
        @param none
        @return - Vendor ID(high 16 bits) & Product ID(low 16 bits)
                 0 if device unexist.
    */
    uint32_t getDevID(void) {
        return m_devID;
    }
#define VID_VAL(x)			((x) >> 16)
#define PID_VAL(x)			((x) & 0xFFFFUL)
#define VID_MULTI_SWITCH 		0x2886	// Vender ID of the device
#define PID_5_WAY_TACTILE_SWITCH	0x0002	// Grove 5-Way Tactile
#define PID_6_POS_DIP_SWITCH		0x0003	// Grove 6-Position DIP Switch

    /**
        Get device model and version
        @return - NULL if device unexist.
    */
    const char* getDevVer(void);

    /**
        Change i2c address of device.

        @param addr - The new i2c address of device.
        @return - none
    */
    void setDevAddr(uint8_t addr);

    /**
        Get button/Switch count
    */
    int getSwitchCount(void);

    /**
         Disable or enable event detecting mode.
        Tactile switch events: single/double click, long press
        DIP switch events    : level changed

         Raw switch status could be checked by getEvent()
        no matter what mode has been setup.
        Tactile switch status: press/non-press
        DIP switch status    : switch on/off

        @param enable - true for enable, else for disable.
        @return  bool - true for success or false for fail
    */
    bool setEventMode(bool enable);

    /**
        Get button/switch event,
        refer to setEventMode() to get all available events.

        @return ButtonEvent_t* -
               NULL if error, such as device unexist.
               .event = BTN_EV_NO_EVENT if no event
               .event = BTN_EV_HAS_EVENT if event occured,
                        and .button to check event of each button/switch.
    */
    ButtonEvent_t* getEvent(void);

  private:
    int readDev(uint8_t* data, int len);
    int writeDev(const uint8_t* data, int len);
    int readReg(uint8_t reg, uint8_t* data, int len);

    /**
        probe device & it's VID & PID

    */
    uint32_t probeDevID(void);

#define _MULTI_SWITCH_VERSIONS_SZ			10
    char versions[_MULTI_SWITCH_VERSIONS_SZ];
    int version;

    TwoWire* m_i2c;
    uint8_t m_devAddr;
    uint32_t m_btnCnt;
    uint32_t m_devID;
  public:
    // debug purpose
    int errno;
};

#endif //__GROVE_MULTI_SWITCH__
