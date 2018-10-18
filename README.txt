KADE 4 Tartarus

The intention of this library is to run a single Analog Joystick (with Button),
and be installed inside a Razer Tartarus V2 Keypad, to replace the tactile dpad

The idea is to give a full 3D joystick experiance for character movement in games,
while also replacing the bullshit tarticle buttons which have become faulty within
a month of purchasing...

The addition's I have made to the original code base:

- ADC Setup & Reading of Pins (used from LUFA libs)
- Digital Read methods (pulled from Arduino Core)
- Utility methods (map, millis)
- Joystick Calibration & Deadzone
- Mapping Joystick to XInput

This library has been forked from the KADE XInputProMicro repo, and I give full kudo's
to the original KADE development team:

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

 Jon Wilson    - jon@kadevice.com
 Bruno Freitas - bruno@kadevice.com
 Kevin Mackett - kevin@kadevice.com

 KADE software is licensed under GNU GPL v3.
 KADE hardware is licensed under Creative Commons BY-SA 3.0
 Refer to license.txt in this folder.

 We make our software and hardware open so that folks can contribute,
 innovate, improve the technology and make things better for the community.
 Derived products (software and hardware) must adhere to the same licenses
 and KADE should be appropriately attributed.

 Please take a moment and register for our community and support forums
 at: http://kadevice.com/forum

 Visit the Open Source Software / Open Hardware pages on our forum at
 http://kadevice.com/forum/viewforum.php?f=20