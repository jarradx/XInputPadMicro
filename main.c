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

void setup_pins(void);

int main(void) {
	// Set clock @ 16Mhz
	CPU_PRESCALE(0);

	// Disable JTAG
	bit_set(MCUCR, 1 << JTD);
	bit_set(MCUCR, 1 << JTD);

	// Setup pins
	setup_pins();

	// Init XBOX pad emulation
	xbox_init(true);

	// Pins polling and gamepad status updates
	for (;;) {
		xbox_reset_watchdog();

		// clear state of buttons we aren't currently using
		bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_UP);
		bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_DOWN);
		bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_LEFT);
		bit_clear(gamepad_state.digital_buttons_1, XBOX_DPAD_RIGHT);
		bit_clear(gamepad_state.digital_buttons_1, XBOX_START);
		bit_clear(gamepad_state.digital_buttons_1, XBOX_BACK);
		bit_clear(gamepad_state.digital_buttons_1, XBOX_RIGHT_STICK);
//		bit_clear(gamepad_state.digital_buttons_1, XBOX_LEFT_STICK);

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
//		uint16_t x = analogRead(A0);
//		uint16_t y = analogRead(A1);
		uint16_t x = ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_LEFT_ADJUSTED | ADC_CHANNEL0);
		uint16_t y = ADC_GetChannelReading(ADC_REFERENCE_AVCC | ADC_LEFT_ADJUSTED | ADC_CHANNEL1);

		// TODO: take into account deadspot, run pre-loop to calibrate when device is plugged in

		// map from 0 - 1023 from potentometer, to the uint16_t range XInput wants in return		
        gamepad_state.l_x = map(x, 0, 1023, -32768, 32767);
        gamepad_state.l_y = map(y, 0, 1023, -32768, 32767);

        // set state of left analog stick button
        digitalRead(10) != LOW ? bit_set(gamepad_state.digital_buttons_1, XBOX_LEFT_STICK)  : bit_clear(gamepad_state.digital_buttons_1, XBOX_LEFT_STICK);

		xbox_send_pad_state();
	}
}

void setup_pins(void) {
//  pinMode(A0, INPUT); // Analog Pin 0
//  pinMode(A1, INPUT); // Analog Pin 1
    ADC_Init(ADC_FREE_RUNNING | ADC_PRESCALE_32);

    ADC_SetupChannel(0);
    ADC_SetupChannel(1);

    pinMode(10, INPUT_PULLUP); // Digital Pin 10
}
