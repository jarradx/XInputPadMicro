/*
     .k8GOGGNqkSFS5XkqXPSkSkkqXXFS5kSkSS15U22F2515U2uujuu1U1u2U1U2uUuFS.
   :0qE     JS5uuJuuFFX51jU2SSk12jU2SSXF5uuu15SFS5k12ujj21S5kFS5S12jJYu11
  5XS:        1UYYLu.   vUUX    U22r     SUF         SUF           ;YYLuU5
 1F5i  NNSkS7  2uLJui   51u     S5.      .PX         .XX           LJvLLu1.
 kUk  0iLk5FFu vuYY2:   5F    Xkk7        78    E0    i0    GEXPXk2uLLvLLuk
X25, 8O   2kX0  5YJUi   M    555    PkXk   i    q1FU   7    ONNkP12YLvLvLYS
S25  8888  888  5uY5         FuS    PS50   .    FuUU   7          uJvLvLLJ2i
kUF             SJjU.      P02UF    P25k   .    Su2Y   v          2LLvLvLL17
S21  XJj88  0u  1uY2.        X2k           .    k11E   v    7;ii:JuJvLvLvJ2:
2257 jqv   Pqq  1LJur         PP.          7    EX:    q    OqqXP51JYvLvYYS.
 X2F  kXkXSXk  kJYLU:   O     ,Z    0PXZ   i    ii    q0    i:::,,.jLLvLLuF'
 ik1k  ;qkPj  .uJvYu:   UN      :   XU2F   :         S5S           iJLLvjUF8
  :PSq       72uLLLui   uSi    .;   2uY1   r.       72j1           LYYLYJSU88
    XqE2   rP12juJuu1FX55U5FqXXSXkXF1juUkkPSXSPXPXPF1Jju5FkFSFXFSF5uujUu5j28V
      .uGOZESS5S5SFkkPkPkXkPXqXPXqXXFkSkkPXPXPkqSkSS1521252121U2u2u12Suv7

*
* Arduino Micro (Leonardo) XInput Pad Emulator firmware
*
* Copyright (c) 2017
* Bruno Freitas - bruno@brunofreitas.com
* Jon Wilson    - degenatrons@gmail.com
* Kevin Mackett - kevin@sharpfork.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "XInputPad.h"
#include "util.h"
#include <LUFA/Drivers/Peripheral/ADC.h>

void setup(void);
void loop(void);

int main(void) {
	// Set clock @ 16Mhz
	CPU_PRESCALE(0);

	// Disable JTAG
	bit_set(MCUCR, 1 << JTD);
	bit_set(MCUCR, 1 << JTD);

	// setup
	setup();

	for (;;) {
		// loop
		loop();
	}
}

int cUpperX = 0;
int cLowerX = 1023;
int cUpperY = 0;
int cLowerY = 1023;
int cCenter = 511; // center point
int cDeadzone = 0;

void calibrate(void) {
    // record upper and lower "centered" values for the first 5 seconds of being plugged in
    while (millis() < 5000) {
        uint16_t x = ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_RIGHT_ADJUSTED | ADC_CHANNEL6);
        uint16_t y = ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_RIGHT_ADJUSTED | ADC_CHANNEL7);

        if (x > cUpperX) cUpperX = x;
        if (x < cLowerX) cLowerX = x;
        if (y > cUpperY) cUpperY = y;
        if (y < cLowerY) cLowerY = y;
    }

    int distances[4] = {
        abs(cUpperX - cCenter),
        abs(cLowerX - cCenter),
        abs(cUpperY - cCenter),
        abs(cLowerY - cCenter)
    };


    for (int i=0; i<4; i++){
       if (distances[i] > cDeadzone) cDeadzone = distances[i];
    }
}

void setup(void) {
	// setup ADC
    ADC_Init(ADC_FREE_RUNNING | ADC_PRESCALE_128);

    // Setup ADC Channels (by index)
    ADC_SetupChannel(6);
    ADC_SetupChannel(7);

    // Pull digital pin HIGH that need it (Pin 10 is Joystick Button, which pulls LOW on press)
    pinMode(10, INPUT_PULLUP); // Digital Pin 10

    // Init XBOX pad emulation
	xbox_init(true);

	// Calibrate Deadzone
    calibrate();
}

void loop(void) {
	xbox_reset_watchdog();

	// clear state of buttons we aren't currently using
	bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_UP);
	bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_DOWN);
	bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_LEFT);
	bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_RIGHT);
	bit_clear(gamepad_state.digital_buttons_1, XBOX_START);
	bit_clear(gamepad_state.digital_buttons_1, XBOX_BACK);
	bit_clear(gamepad_state.digital_buttons_1, XBOX_RIGHT_STICK);
	// bit_clear(gamepad_state.digital_buttons_1, XBOX_LEFT_STICK);

	bit_clear(gamepad_state.digital_buttons_2, XBOX_A);
	bit_clear(gamepad_state.digital_buttons_2, XBOX_B);
	bit_clear(gamepad_state.digital_buttons_2, XBOX_X);
	bit_clear(gamepad_state.digital_buttons_2, XBOX_Y);
	bit_clear(gamepad_state.digital_buttons_2, XBOX_LB);
	bit_clear(gamepad_state.digital_buttons_2, XBOX_RB);
	bit_clear(gamepad_state.digital_buttons_2, XBOX_HOME);

	gamepad_state.r_x = 0;
	gamepad_state.r_y = 0;

	gamepad_state.lt = 0;
	gamepad_state.rt = 0;

	// set state of left analog stick
	uint16_t x = ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_RIGHT_ADJUSTED | ADC_CHANNEL6); // Channel 7 is my X...? to investigate
	uint16_t y = ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_RIGHT_ADJUSTED | ADC_CHANNEL7); // Channel 6 is my Y...? to investigate

	// map from 0 - 1023 from potentometer, to -32768 - 32767 signed int range XInput wants in return
	// take into account deadzone from calibration
	int dx, dy;
	if (x <= cCenter) {
	    // map from 0 - (511 - deadzone), to the -32768 - -1;
	    dx = map(x, 0, cCenter - cDeadzone, -32768, -1);
	} else {
	    // map from 0 - (512 + deadzone), to the 0 - 32767;
	    dx = map(x, cCenter + 1 + cDeadzone, 1023, 0, 32767);
	}

	if (y <= cCenter) {
	    // map from 0 - (511 - deadzone), to the 32767 - 0;
	    dy = map(y, 0, cCenter - cDeadzone, 32767, 0);
	} else {
	    // map from 0 - (512 + deadzone), to the -1 - -32768;
	    dy = map(y, cCenter + 1 + cDeadzone, 1023, -1, -32768);
	}

  	// set the left x, y analog stick values
    gamepad_state.l_x = dx;
    gamepad_state.l_y = dy;

    // set state of left analog stick button
    digitalRead(10) != LOW ? bit_set(gamepad_state.digital_buttons_1, XBOX_LEFT_STICK)  : bit_clear(gamepad_state.digital_buttons_1, XBOX_LEFT_STICK);

	xbox_send_pad_state();
}
