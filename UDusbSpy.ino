// UDusbSpy - by Seth N. Hetu (sorlok_reaves)
// Based on NintendoSpy by Jeremy Burns (jaburns)
// Available under the terms of the MIT license; see the LICENSE file for details

// Used to test new controllers hooked up to UD-USB.
// Feedback will be provided through the console if enabled.
// You must disable this if you want "normal" operation.
//#define MODE_INPUT_TEST

// How many bits does the SNES expect to be shifted?
#define SNES_BITCOUNT  16

// Handy defines from NintendoSpy
#define ZERO  '\0'  // Use a byte value of 0x00 to represent a bit with value 0.
#define ONE    '1'  // Use an ASCII one to represent a bit with value 1.  This makes Arduino debugging easier.
#define SPLIT '\n'  // Use a new-line character to split up the controller state packets.


#define PIND_READ( pin ) (PIND&(1<<(pin)))
#define PINC_READ( pin ) (PINC&(1<<(pin)))
#define PINB_READ( pin ) (PINB&(1<<(pin)))

// Not sure why the first one doesn't work...
//#define PIND_WRITE( pin, val )  (PORTD = PORTD | ((val)<<(pin)))
#define PIND_WRITE( pin, val )  if (val) { (PORTD = PORTD | (1<<(pin))); } else { (PORTD = PORTD & (~(1<<(pin)))); }

#define WAIT_FALLING_EDGE_PIND( pin ) while( !PIND_READ(pin) ); while( PIND_READ(pin) );
#define WAIT_RISING_EDGE_PIND( pin ) while( PIND_READ(pin) ); while( !PIND_READ(pin) );

// Perform a pin read into a variable. Inverts, since SNES and UD-USB are both low-active.
#define READ_PIND_TO_VAR( var, pin ) var = !PIND_READ(pin);
#define READ_PINB_TO_VAR( var, pin ) var = !PINB_READ(pin);
#define READ_PINC_TO_VAR( var, pin ) var = !PINC_READ(pin);

// Print button press to serial and set a flag.
#define BTN_TEST_PRINT(cond, btn, flag) if (cond) { Serial.println(btn); flag=1; }

// Send a signal for a button read from a given pin
#define SEND_PIND_BTN( pin ) do{ *rawDataPtr = PIND_READ(pin); PIND_WRITE(4, *rawDataPtr); rawDataPtr++; } while(false)
#define SEND_PINB_BTN( pin ) do{ *rawDataPtr = PINB_READ(pin); PIND_WRITE(4, *rawDataPtr); rawDataPtr++; } while(false)
#define SEND_PINC_BTN( pin ) do{ *rawDataPtr = PINC_READ(pin); PIND_WRITE(4, *rawDataPtr); rawDataPtr++; } while(false)

// Send a signal for a button that doesn't exist (the SNES has 4, all set to HIGH)
#define SEND_UNUSED_BTN( val )   do{ *rawDataPtr++ = 0; PIND_WRITE(4, val); } while(false)

// Used as part of the input test
int tempVar = 0;

// Button presses on UD-USB
int btnStart = 0;
int btnSelect = 0;
int btnRight = 0;
int btnLeft = 0;
int btnDown = 0;
int btnUp = 0;
int btnP1 = 0;
int btnP2 = 0;
int btnP3 = 0;
int btnK1 = 0;
int btnK2 = 0;
int btnK3 = 0;
int btnHome = 0;
int btnP4 = 0;
int btnK4 = 0;

// Inputs on SNES
int snesLatch = 0;
int snesClock = 0;

// As we read from the UD-USB controller, we store the bits here in addition to sending them to the SNES
// Thus, NintendoSpy sees the same bits that we send to the console.
unsigned char rawData[ 128 ];



void setup()
{
   DDRD = B00010010; // Digital 0 to 7
   DDRB = B00000000; // Digital 8 to 13
   DDRC = B00000000; // Analog 0 to 5

   Serial.begin( 115200 );
}

inline void loop_INPUTTEST()
{
  // Read UD-USB's button states
  // First set
  READ_PIND_TO_VAR(btnStart,  5);
  READ_PIND_TO_VAR(btnSelect, 6);
  READ_PIND_TO_VAR(btnRight,  7);

  // Second set
  READ_PINB_TO_VAR(btnLeft,   3);
  READ_PINB_TO_VAR(btnDown,   4);
  READ_PINB_TO_VAR(btnUp,     5);

  // Third set
  READ_PINC_TO_VAR(btnP1, 0);
  READ_PINC_TO_VAR(btnP2, 1);
  READ_PINC_TO_VAR(btnP3, 2);
  READ_PINC_TO_VAR(btnP4, 3);
  READ_PINC_TO_VAR(btnK1, 4);
  READ_PINC_TO_VAR(btnK2, 5);

  // Output!
  tempVar = 0;
  BTN_TEST_PRINT(btnStart,  "Start",  tempVar);
  BTN_TEST_PRINT(btnSelect, "Select", tempVar);
  BTN_TEST_PRINT(btnRight,  "Right",  tempVar);
  BTN_TEST_PRINT(btnLeft,   "Left",   tempVar);
  BTN_TEST_PRINT(btnDown,   "Down",   tempVar);
  BTN_TEST_PRINT(btnUp,     "Up",     tempVar);
  BTN_TEST_PRINT(btnP1,     "P1",     tempVar);
  BTN_TEST_PRINT(btnP2,     "P2",     tempVar);
  BTN_TEST_PRINT(btnP3,     "P3",     tempVar);
  BTN_TEST_PRINT(btnP4,     "P4",     tempVar);
  BTN_TEST_PRINT(btnK1,     "K1",     tempVar);
  BTN_TEST_PRINT(btnK2,     "K2",     tempVar);

  // Else, just show "...", so that we know time has passed in the console.
  BTN_TEST_PRINT(!tempVar, "...", tempVar);
}

inline void loop_SNES()
{
  unsigned char *rawDataPtr = rawData;
 
  noInterrupts();
  
  // Each cycle starts with latch
  WAIT_FALLING_EDGE_PIND( 3 );

  // Now there are 16 clock pulses
  // Controller writes data on the rising edge, and SNES samples on the falling
  // Buttons are low-active, which matches the UD-USB
  // Note: I've intentionally left this loop unrolled.

  // The very first button is driven at the falling edge of latch
  // Button: B
  SEND_PINC_BTN(4);

  // Next buttons wait for rising edge of clock.
  // Button: Y
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PINC_BTN(0);

  // Button: Select
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PIND_BTN(6);

  // Button: Start
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PIND_BTN(5);

  // Button: Up
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PINB_BTN(5);

  // Button: Down
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PINB_BTN(4);

  // Button: Left
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PINB_BTN(3);

  // Button: Right
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PIND_BTN(7);

  // Button: A
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PINC_BTN(5);

  // Button: X
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PINC_BTN(1);

  // Button: L
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PINC_BTN(3);

  // Button: R
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_PINC_BTN(2);

  // Not used (always high)
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_UNUSED_BTN( HIGH );
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_UNUSED_BTN( HIGH );
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_UNUSED_BTN( HIGH );
  WAIT_RISING_EDGE_PIND( 2 );
  SEND_UNUSED_BTN( HIGH );

  // Done; drive serial pin low until next cycle.
  PIND_WRITE(4, LOW);

  interrupts();

  // Update NintendoSpy
  for(unsigned char i=0; i<SNES_BITCOUNT; i++) {
    Serial.write( rawData[i] ? ZERO : ONE );
  }
  Serial.write( SPLIT );
}

void loop()
{
#ifdef MODE_INPUT_TEST
  loop_INPUTTEST();
#else
  loop_SNES();
#endif
}
