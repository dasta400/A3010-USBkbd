/* ---------------------------LICENSE NOTICE-------------------------------- 
 *  MIT License
 *  
 *  Copyright (c) 2019 David Asta
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

 /*
  * Teensy++ 2.0 based keyboard controller for Acorn Archimedes A3010 keyboard to USB
  * 
  * CURRENT STATUS
  * ============================
  *     \ | prints instead # ~
  *     ` ~ prints instead ¬
  *     2 @ prints instead "
  *     3 # prints instead £
  *     ' " prints instead ' @
  *     
  *     Not working:
  *       It seems to work fine on Windows 10 and Linux Mint, but
  *           on RISC OS Pi keeps repeating the key after releasing it.
  *       Caps Lock LED not working well
  *       Multiple modifier keys (e.g. Ctrl + Shift)
  *       
  *     Not implemented:
  *       Scroll Lock LED
  *       Num Lock LED
  *       Pound key (I have to figure oput how to detect it)
  *       # keypad (I have to figure oput how to detect it, and hthen I'll map it to the normal hash)
  * ============================
  * 
  * The internal keyboard of the A3010 uses a membrane switch matrix 
  * scanned by an 87C51 8-bit microcontroller. To avoid ghosting, Acorn 
  * implemented the Ctrl, Alt, Shift and Home keys in an independent scan 
  * line (column 6).
  * 
  * The Uk keyboard has:
  *   103 keys
  *   3 status LEDs (Caps Lock, Scroll Lock, Num Lock)
  *   2 extra LEDs (Power On, Disk Activity)
  * 
  * A3010 connectors
  *   TOP connector (as ssen from top of the keyboard) contains the ROWS 
  *   and Pin 1 is located on the right
  *      1 NC             11 P1<3>
  *      2 NC             12 P0<5>
  *      3 NC             13 P1<2>
  *      4 NC             14 P1<1>
  *      5 P1<7>          15 P0<4>
  *      6 P1<6>          16 P0<3>
  *      7 P1<5>          17 P0<2>
  *      8 P1<4>          18 P0<1>
  *      9 P0<7>          19 P1<0>
  *     10 P0<6>          20 P0<0>
  *   BOTTOM connector (as ssen from top of the keyboard) contains the COLUMNS
  *   and Pin 1 is located on the left
  *      1 CapsLock LED   11 Y<8>
  *      2 Y<0>           12 Y<9>
  *      3 Y<1>           13 Y<10>
  *      4 Y<2>           14 Y<11>
  *      5 Y<3>           15 Y<12>
  *      6 Y<4>           16 +5Vcc
  *      7 Y<5>           17 ScrollLock LED
  *      8 Nkeycom        18 NumLock LED
  *      9 Y<6>           19 Disk activity
  *     10 Y<7>           20 Power On LED
  *                   Pin 20 is the only pin that is not connected to the 87C51
  *                   but instead is connected to somewhere in the A3010 board.
  *                   This Teensy++ 2.0 based keyboard controller doesn't know
  *                   anything about the disk activity, therefore this LED
  *                   can be used for another purpose.
  *     
  * As per A3010 schematic (strange values.. IMHO), for each LED:
  *     Capslock        = 220R  Green
  *     ScrollLock      = 270R  Green
  *     NumLock         = 270R  Green
  *     PowerOn         = 330R  Green
  *     Disk activity   = 330R  Orange
  *     
  *     UK Keyboard Matrix:
  *      Key               Row Column
  *     ------------------------------
  *      Esc                15      1
  *      F1                 10      2
  *      F2                 11      2
  *      F3                 11      3
  *      F4                 10      3
  *      F5                  7      7
  *      F6                  7     11
  *      F7                  5     13
  *      F8                  4     13
  *      F9                  7     13
  *      F10                 7     12
  *      F11                 5     12
  *      F12                 4     12
  *      ~ `                13      1
  *      1 !                 7      1
  *      2 @                 5      2
  *      3 #                 5      3
  *      4 $                10      4
  *      5 %                 7      4
  *      6 ^                 7      5
  *      7 &                10      5
  *      8 *                10      7
  *      9 (                10     11
  *      0 )                11     13
  *      - _                10     12
  *      = +                 7     10
  *      £                  10     10
  *      Backspace           5     10
  *      Tab                11      1
  *      Left Ctrl           9      6
  *      Left Shift          8      6
  *      Caps Lock          15      2
  *      Left Alt           14      6
  *      A                   4      1
  *      B                  15      4
  *      C                  13      3
  *      D                  12      3
  *      E                   4      3
  *      F                  11      4
  *      G                  12      4
  *      H                  12      5
  *      I                   5      7
  *      J                  11      5
  *      K                  12      7
  *      L                  11     11
  *      M                  13      5
  *      N                  15      5
  *      O                   5     11
  *      P                  13     13
  *      Q                   5      1
  *      R                   5      4
  *      S                  12      2
  *      T                   4      4
  *      U                   5      5
  *      V                  13      4
  *      W                   4      2
  *      X                  13      2
  *      Y                   4      5
  *      Z                  12      1
  *      [ {                12     13
  *      ] }                 4     11
  *      \ |                11      7
  *      ; :                15     13
  *      ' "                12     11
  *      , <                13      7
  *      . >                13     11
  *      / ?                15     11
  *      Return              4      7
  *      Right Shift         2      6
  *      Right Alt           0      6
  *      Right Ctrl/Action   1      6
  *      Print               3      6
  *      Scroll Lock         4      0
  *      Break/Pause         5      0
  *      Insert              4     10
  *      Home                6      6
  *      Page Up            10      0
  *      Delete             10      9
  *      Copy/End            5      9
  *      Page Down           4      9
  *      Cursor Up           4      8
  *      Cursor Left        15      7  
  *      Cursor Down        13      8
  *      Cursor Right       12      8
  *      Num Lock           11      0
  *      Numpad /           11     12
  *      Numpad *           11     10
  *      Numpad #           11      9
  *      Numpad -           12      9
  *      Numpad +           13      9
  *      Numpad Enter       15      9
  *      Numpad .           11      8
  *      Numpad 1           15      0
  *      Numpad 2           15     12
  *      Numpad 3           15     10
  *      Numpad 4           13      0
  *      Numpad 5           13     12
  *      Numpad 6           13     10
  *      Numpad 7           12      0
  *      Numpad 8           12     12
  *      Numapd 9           12     10
  *      Numpad 0           15      8
  */

#include <Keyboard.h>
/* In Arduino IDE must be selected:
 *      Tools > Board > Teensy++ 2.0
 *      Tools > USB Type > Keyboard + Mouse + Joystick
 */

#ifndef KEY_A
#error Something went wrong when including Keyboard.h
#endif

#define LED_BUILTIN 6     // LED on Teensy++ 2.0 will flash everytime time a key is pressed

// Missing key definitions
#define KEY_POUND   0x00
#define KEYPAD_HASH 0x00

//#define LED_BUILTIN 6     // LED on Teensy++ 2.0 flashes every key press
#define NUM_ROWS 16   // TOP Connector: pins 1, 2, 3 and 4 are Not connected (NC)
#define NUM_COLS 14   // BOTTOM Connector: pins 1, 17, 18, 19 and 20 are for LEDS, and pin 16 is +5Vcc
#define DEBOUNCE_VALUE 40

// Shift keys position in the matrix
#define LSHIFT_COL 6
#define LSHIFT_ROW 8
#define RSHIFT_COL 6
#define RSHIFT_ROW 2

// Ctrl keys position in the matrix
#define LCTRL_COL 6
#define LCTRL_ROW 9
#define RCTRL_COL 6
#define RCTRL_ROW 1

// Alt keys position in the matrix
#define LALT_COL 6
#define LALT_ROW 14
#define RALT_COL 6
#define RALT_ROW 0

// Caps Lock position in the matrix
#define CAPSLOCK_COL 2
#define CAPSLOCK_ROW 15

// LEDs
#define LED_CAPSLOCK 14    // C4
#define LED_SCROLLLOCK 15  // C5
#define LED_NUMLOCK 16     // C6
#define LED_POWERON 45     // F7
//#define LED_DISKACTIV      // Not used

// Pins for Teensy++ 2.0     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
// bottom cable -> columns (B7, D0, D1, D2, D3, D4, D5, D7, E0, E1, C0, C1, C2, C3)
int colPins[NUM_COLS] =    {27,  0,  1,  2,  3,  4,  5,  7,  8,  9, 10, 11, 12, 13};
// top cable ----> rows    (F6, F5, F4, F3, F2, F1, F0, E6, E7, B0, B1, B2, B3, B4, B5, B6)
int rowPins[NUM_ROWS] =    {44, 43, 42, 41, 40, 39, 38, 18, 19, 20, 21, 22, 23, 24, 25, 26};

unsigned int keyMap[NUM_ROWS][NUM_COLS] = {
  {0, 0, 0, 0, 0, 0, KEY_RIGHT_ALT, 0, 0, 0, 0, 0, 0, 0}, // Row 0      pin 5
  {0, 0, 0, 0, 0, 0, KEY_RIGHT_CTRL, 0, 0, 0, 0, 0, 0, 0}, // Row 1      pin 6
  {0, 0, 0, 0, 0, 0, KEY_RIGHT_SHIFT, 0, 0, 0, 0, 0, 0, 0}, // Row 2      pin 7
  {0, 0, 0, 0, 0, 0, KEY_PRINTSCREEN, 0, 0, 0, 0, 0, 0, 0}, // Row 3      pin 8
  {KEY_SCROLL_LOCK, KEY_A, KEY_W, KEY_E, KEY_T, KEY_Y, 0, KEY_RETURN, KEY_UP, KEY_PAGE_DOWN, KEY_INSERT, KEY_RIGHT_BRACE, KEY_F12, KEY_F8}, // Row 4      pin 9
  {KEY_PAUSE, KEY_Q, KEY_2, KEY_3, KEY_R, KEY_U, 0, KEY_I, 0, KEY_END, KEY_BACKSPACE, KEY_O, KEY_F11, KEY_F9}, // Row 5      pin 10
  {0, 0, 0, 0, 0, 0, KEY_HOME, 0, 0, 0, 0, 0, 0, 0}, // Row 6      pin 11
  {0, KEY_1, 0, 0, KEY_5, KEY_6, 0, KEY_F5, 0, 0, KEY_EQUAL, KEY_F6, KEY_F10, KEY_9}, // Row 7      pin 12
  {0, 0, 0, 0, 0, 0, KEY_LEFT_SHIFT, 0, 0, 0, 0, 0, 0, 0}, // Row 8      pin 13
  {0, 0, 0, 0, 0, 0, KEY_LEFT_CTRL, 0, 0, 0, 0, 0, 0, 0}, // Row 9      pin 14
  {KEY_PAGE_UP, 0, KEY_F1, KEY_F4, KEY_4, KEY_7, 0, KEY_8, 0, KEY_DELETE, KEY_POUND, KEY_9, KEY_MINUS, 0}, // Row 10     pin 15
  {KEY_NUM_LOCK, KEY_TAB, KEY_F2, KEY_F3, KEY_F, KEY_J, 0, KEY_BACKSLASH, KEYPAD_PERIOD, KEYPAD_HASH, KEYPAD_ASTERIX, KEY_L, KEYPAD_SLASH, KEY_0}, // Row 11     pin 16
  {KEYPAD_7, KEY_Z, KEY_S, KEY_D, KEY_G, KEY_H, 0, KEY_K, KEY_RIGHT, KEYPAD_MINUS, KEYPAD_9, KEY_QUOTE, KEYPAD_8, KEY_LEFT_BRACE}, // Row 12     pin 17
  {KEYPAD_4, KEY_TILDE, KEY_X, KEY_C, KEY_V, KEY_M, 0, KEY_COMMA, KEY_DOWN, KEYPAD_PLUS, KEYPAD_6, KEY_PERIOD, KEYPAD_5, KEY_P}, // Row 13     pin 18
  {0, 0, 0, 0, 0, 0, KEY_LEFT_ALT, 0, 0, 0, 0, 0, 0, 0}, // Row 14     pin 19
  {KEYPAD_1, KEY_ESC, KEY_CAPS_LOCK, KEY_SPACE, KEY_B, KEY_N, 0, KEY_LEFT, KEYPAD_0, KEYPAD_ENTER, KEYPAD_3, KEY_SLASH, KEYPAD_2, KEY_SEMICOLON} // Row 15     pin 20
};

int debouncerCount[NUM_ROWS][NUM_COLS];
bool capslock_on;



void setup() {
  // turn off Arduino on-board LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // set all LED pins as outputs and turn them OFF
  pinMode(LED_CAPSLOCK,   OUTPUT); digitalWrite(LED_CAPSLOCK,   HIGH);
  pinMode(LED_SCROLLLOCK, OUTPUT); digitalWrite(LED_SCROLLLOCK, HIGH);
  pinMode(LED_NUMLOCK,    OUTPUT); digitalWrite(LED_NUMLOCK,    HIGH);
  pinMode(LED_POWERON,    OUTPUT); digitalWrite(LED_POWERON,    HIGH);
  //pinMode(LED_DISKACTIV,  OUTPUT); digitalWrite(LED_DISKACTIV,  HIGH);  // Not used
  pinMode(LED_BUILTIN,    OUTPUT); digitalWrite(LED_BUILTIN,    LOW);


  // set all ROWS pins as input pull-up
  for(int r=0; r<NUM_ROWS; r++){
    pinMode(rowPins[r], INPUT_PULLUP);
  }

  // set all COLS pins as inputs
  for(int c=0; c<NUM_COLS; c++){
    pinMode(colPins[c], INPUT);

    // clear debouncer counts
    for(int r=0; r<NUM_ROWS; r++){
      debouncerCount[r][c] = 0;
    }
  }

  capslock_on = false;

  // Switch ON the Power On LED
  digitalWrite(LED_POWERON, LOW);
}

void loop() {
  readMatrixKBD();

  if(capslock_on){
    digitalWrite(LED_CAPSLOCK, LOW);
  }else{
    digitalWrite(LED_CAPSLOCK, HIGH);
  }
}

void check_modifer_key(int r, int c, int mod){
  // is the modifier key pressed?
  pinMode(colPins[c], OUTPUT);
  digitalWrite(colPins[c], LOW);
  pinMode(rowPins[r], INPUT_PULLUP);
  
  if(digitalRead(rowPins[r]) == LOW){
    Keyboard.set_modifier(mod);
  }

  pinMode(rowPins[r], INPUT_PULLUP);
  pinMode(colPins[c], INPUT);
}

void check_capslock(){
  pinMode(colPins[CAPSLOCK_COL], OUTPUT);
  digitalWrite(colPins[CAPSLOCK_COL], LOW);
  pinMode(rowPins[CAPSLOCK_ROW], INPUT_PULLUP);
  
  if(digitalRead(rowPins[CAPSLOCK_ROW]) == LOW){
    if(capslock_on){
      capslock_on = false;
    }else{
      capslock_on = true;
    }
  }

  pinMode(rowPins[CAPSLOCK_ROW], INPUT_PULLUP);
  pinMode(colPins[CAPSLOCK_COL], INPUT);
}

void readMatrixKBD(){
  int repeatCount = 0;
  check_modifer_key(LSHIFT_ROW, LSHIFT_COL, MODIFIERKEY_SHIFT); // is the Left Shift pressed?
  check_modifer_key(RSHIFT_ROW, RSHIFT_COL, MODIFIERKEY_SHIFT); // is the Right Shift pressed?
  check_modifer_key(LCTRL_ROW, LCTRL_COL, MODIFIERKEY_CTRL); // is the Left Ctrl pressed?
  check_modifer_key(RCTRL_ROW, RCTRL_COL, MODIFIERKEY_CTRL); // is the Right Ctrl pressed?
  check_modifer_key(LALT_ROW, LALT_COL, MODIFIERKEY_ALT); // is the Left Alt pressed?
  check_modifer_key(RALT_ROW, RALT_COL, MODIFIERKEY_ALT); // is the Right Alt pressed?

  check_capslock(); // Not reliable. Sometimes stays on when it shouldn't

  // ToDo - Multiple modifier keys (e.g. Ctrl+Shift) can be pressed at same time
  //        but this implementation it isn't allowed because check_modifer_key()
  //        set modifier individually, but it should do something like for 
  //        example: Keyboard.set_modifier(MODIFIERKEY_CTRL | MODIFIERKEY_ALT);

  // for each column
  for(int c=0; c<NUM_COLS; c++){
    // enable column, by setting it as OUTPUT and LOW
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], LOW);
      
    // for each row
    for(int r=0; r<NUM_ROWS; r++){
      pinMode(rowPins[r], INPUT_PULLUP);

      // if there is LOW on a row read, then a key was pressed
      if(digitalRead(rowPins[r]) == LOW){
        // apply a key debouncer to avoid the mechanical bouncing effect of keyboards
        debouncerCount[r][c]++;
        repeatCount = debouncerCount[r][c];

        if(repeatCount == 1){
          digitalWrite(LED_BUILTIN, HIGH);
          Keyboard.press(keyMap[r][c]);
          Keyboard.release(keyMap[r][c]);
        }else if(repeatCount > DEBOUNCE_VALUE){
          repeatCount -= DEBOUNCE_VALUE;

          if(repeatCount % (DEBOUNCE_VALUE * 2) == 0){
            digitalWrite(LED_BUILTIN, HIGH);
            Keyboard.press(keyMap[r][c]);
            Keyboard.release(keyMap[r][c]);
          }
        }
      }else{
        // clear debouncer count for ROW, COLUMN
        debouncerCount[r][c] = 0;
      }

      // disable the row, by setting it as INPUT_PULLUP
      pinMode(rowPins[r], INPUT_PULLUP);
      digitalWrite(LED_BUILTIN, LOW);
    }

    // disable the column, by setting it as INPUT
    pinMode(colPins[c], INPUT);
  }

  // Turn off Modifier keys (Ctrl, Shift, Alt, Windows Key
  Keyboard.set_modifier(0);
}
