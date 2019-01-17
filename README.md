**Teensy++ 2.0 based keyboard controller for Acorn Archimedes A3010 keyboard to USB.**

The Teensy board is connected to an Acorn Archimedes A3010 membrane switch matrix and acts as microcontroller and USB HID, thus allowing the connections of this keyboard to any computer through USB.

## The Acorn Archimedes A3010
The A3010 keyboard has a key layout very similar to the industry standard PC keyboard. With 103 keys, 3 status LEDs (Caps Lock, Scroll Lock, Num Lock) and 2 extra LEDs (Power On, Disk Activity).

On the original A3010 board the matrix was driven by a 87C51 8-bit microcontroller, except the Disk Activity LED. As the Teensy does not know anything about the disk activity on a computer, this LED can be used for another purpose. For now, I am not using it.

## Materials used
- 1x Acorn Archimedes A3010 keyboard
- 1x Teensy++ 2.0
- 2x AMP 7-520355-0 connectors
- For the LEDs
  - 1x 220R carbon resistor 1/4 W
  - 2x 270R carbon resistors 1/4 W
  - 2x 330R carbon resistors 1/4 W
- 1x Mini-b to A-Type cable
- Soldering iron, solder, cable, pin headers and perfboard

## Current status of this project
- Wrong key mapping:
  - \ | prints instead # ~
  - ` ~ prints instead ¬
  - 2 @ prints instead "
  - 3 # prints instead £
  - ' " prints instead ' @


- Not working:
  - Caps Lock LED not working well. Sometimes stays on or off even the key has been pressed.


- Not implemented:
  - Multiple modifier keys (e.g. Ctrl + Shift). At the moment only one modifier key can be pressed in combination with another key.
  - Scroll Lock LED
  - Num Lock LED
  - £ key (I have to figure out how to detect it)
  - \# keypad (I have to figure out how to detect it, and then I'll map it to the normal hash)


  ## Tested systems
  - Reportedly works on:
    - Linux Mint 17.3
    - Windows 10 Home
    - Rasberry Pi 2B with Amibian


  - It does NOT work on:
    - On Rasberry Pi 2B with RISC OS 5.24 keeps repeating the key after releasing it. I have read on the RISC OS OPEN forums ([this topic](https://www.riscosopen.org/forum/forums/4/topics/2383)) that this seems to be some sort of common issue with some keyboards. If anybody has a solution, please share.
