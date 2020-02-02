/*
    Name: Grove_Multi_Switch.cpp
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
#include "Grove_Multi_Switch.h"
#include "Arduino.h"

#define I2C_CMD_GET_DEV_ID		0x00	// gets device ID information
#define I2C_CMD_GET_DEV_EVENT		0x01	// gets device event status
#define I2C_CMD_EVENT_DET_MODE		0x02	// enable button event detect mode
#define I2C_CMD_BLOCK_DET_MODE		0x03	// enable button block detect mode
#define I2C_CMD_AUTO_SLEEP_ON		0xb2	// enable device auto sleep mode
#define I2C_CMD_AUTO_SLEEP_OFF		0xb3	// disable device auto sleep mode (default mode)
#define I2C_CMD_SET_ADDR		0xc0	// sets device i2c address
#define I2C_CMD_RST_ADDR		0xc1	// resets device i2c address
#define I2C_CMD_TEST_TX_RX_ON		0xe0	// enable TX RX pin test mode
#define I2C_CMD_TEST_TX_RX_OFF		0xe1	// disable TX RX pin test mode
#define I2C_CMD_TEST_GET_VER		0xe2	// use to get software version
#define I2C_CMD_GET_DEVICE_UID		0xf1	// use to get chip id

int GroveMultiSwitch::readDev(uint8_t* data, int len) {
    int r = 0;

    // Ask for `len` byte
    m_i2c->requestFrom((int)m_devAddr, len);

    while (m_i2c->available() && len-- > 0) {
        *data++ = m_i2c->read();
        r++;
    }

    // slave may send more than requested
    while (m_i2c->available()) {
        m_i2c->read();
        r++;
    }
    return r;
}

int GroveMultiSwitch::writeDev(const uint8_t* data, int len) {
    int r, olen = len;

    m_i2c->beginTransmission(m_devAddr);
    while (len-- > 0) {
        m_i2c->write(*data++);
    }
    r = m_i2c->endTransmission();

    if (r > 0) {
        // transmission error
        return -r;
    }
    return olen;
}

int GroveMultiSwitch::readReg(uint8_t reg, uint8_t* data, int len) {
    int r;

    if ((r = writeDev(&reg, 1)) <= 0) {
        return -1;
    }

    r = readDev(data, len);
    return r;
}

uint32_t GroveMultiSwitch::probeDevID(void) {
    uint32_t id;
    uint8_t dummy;
    int tries;

    for (tries = 4; tries > 0; tries--) {
        if ((errno = readReg(I2C_CMD_GET_DEV_ID, (uint8_t*)&id, sizeof id)) <= 0) {
            id = 0;
        }

        if (VID_VAL(id) == VID_MULTI_SWITCH) {
            return (m_devID = id);
        }

        // I2C data buffer shift to right align.
        readDev(&dummy, 1);
    }

    #if 0
    // debug only
    Serial.print("id = ");
    Serial.println(id);
    #endif
    return (m_devID = id);
}

const char* GroveMultiSwitch::getDevVer(void) {
    if (!m_devID) {
        return NULL;
    }

    if ((errno = readReg(I2C_CMD_TEST_GET_VER, (uint8_t*)versions,
                         sizeof versions)) <= 0) {
        return NULL;
    }

    version = ((unsigned)versions[6] - '0') * 10 +
              ((unsigned)versions[8] - '0');
    return versions;
}

void GroveMultiSwitch::setDevAddr(uint8_t addr) {
    uint8_t data[2] = { 0, };

    if (!m_devID) {
        return;
    }

    data[0] = I2C_CMD_SET_ADDR;
    data[1] = addr;

    errno = writeDev(data, sizeof data);
    return;
}

int GroveMultiSwitch::getSwitchCount(void) {
    if (VID_VAL(m_devID) != VID_MULTI_SWITCH) {
        return 0;
    }
    if (PID_VAL(m_devID) == PID_5_WAY_TACTILE_SWITCH) {
        return 5;
    } else if (PID_VAL(m_devID) == PID_6_POS_DIP_SWITCH) {
        return 6;
    }
    return 0;
}

GroveMultiSwitch::ButtonEvent_t* GroveMultiSwitch::getEvent(void) {
    static ButtonEvent_t event, levent;
    static int initial = 0;
    int len = sizeof(uint32_t) + m_btnCnt;

    if (!m_devID) {
        return NULL;
    }

    if ((errno = readReg(I2C_CMD_GET_DEV_EVENT, (uint8_t*)&event, len)) <= 0) {
        return NULL;
    }

    if (version > 1) {	/* Real version 0.1 */
        return &event;
    }
    // Fix: v0.1 will miss event BTN_EV_LEVEL_CHANGED
    //      if this API called frequently.

    if (!initial) {
        levent = event;
        initial = 1;
    }

    for (int i = 0; i < BUTTON_MAX; i++) {
        event.button[i] &= ~BTN_EV_LEVEL_CHANGED;
        if ((event.button[i] ^ levent.button[i]) & BTN_EV_RAW_STATUS) {
            event.button[i] |= BTN_EV_LEVEL_CHANGED;
            event.event     |= BTN_EV_HAS_EVENT;
        }
    }
    levent = event;
    return &event;
}

bool GroveMultiSwitch::setEventMode(bool enable) {
    uint8_t data[] = { 0, };

    if (!m_devID) {
        return false;
    }

    data[0] = enable ? I2C_CMD_EVENT_DET_MODE : I2C_CMD_BLOCK_DET_MODE;

    if ((errno = writeDev(data, sizeof data)) > 0) {
        return true;
    }
    return false;
}
