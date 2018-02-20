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
//#define PINC_WRITE( pin, val )  (PORTC = PORTC | ((val)<<(pin)))
#define PINC_WRITE( pin, val )  if (val) { (PORTC = PORTC | (1<<(pin))); } else { (PORTC = PORTC & (~(1<<(pin)))); }

#define WAIT_FALLING_EDGE_PINC( pin ) while( !PINC_READ(pin) ); while( PINC_READ(pin) );
#define WAIT_RISING_EDGE_PINC( pin ) while( PINC_READ(pin) ); while( !PINC_READ(pin) );

// Perform a pin read into a variable. Inverts, since SNES and UD-USB are both low-active.
#define READ_PIND_TO_VAR( var, pin ) var = !PIND_READ(pin);
#define READ_PINB_TO_VAR( var, pin ) var = !PINB_READ(pin);
#define READ_PINC_TO_VAR( var, pin ) var = !PINC_READ(pin);

// Print button press to serial and set a flag.
#define BTN_TEST_PRINT(cond, btn, flag) if (cond) { Serial.println(btn); flag=1; }

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
   DDRD = B00000010; // Digital 0 to 7
   DDRB = B00000000; // Digital 8 to 13
   DDRC = B00000001; // Analog 0 to 5

   Serial.begin( 115200 );
}


inline void sendRawData( unsigned char first, unsigned char count )
{
    for( unsigned char i = first ; i < first + count ; i++ ) {
        Serial.write( rawData[i] ? ONE : ZERO );
    }
    Serial.write( SPLIT );
}

inline void loop_INPUTTEST()
{
  // Read UD-USB's button states
  // First set
  READ_PIND_TO_VAR(btnStart,  2);
  READ_PIND_TO_VAR(btnSelect, 3);
  READ_PIND_TO_VAR(btnRight,  4);
  READ_PIND_TO_VAR(btnLeft,   5);
  READ_PIND_TO_VAR(btnDown,   6);
  READ_PIND_TO_VAR(btnUp,     7);

  // Second set
  READ_PINB_TO_VAR(btnP1, 0);
  READ_PINB_TO_VAR(btnP2, 1);
  READ_PINB_TO_VAR(btnP3, 2);
  READ_PINB_TO_VAR(btnK1, 3);
  READ_PINB_TO_VAR(btnK2, 4);
  READ_PINB_TO_VAR(btnK3, 5);

  // Third set
  READ_PINC_TO_VAR(btnHome, 3);
  READ_PINC_TO_VAR(btnP4,   4);
  READ_PINC_TO_VAR(btnK4,   5);

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
  BTN_TEST_PRINT(btnK3,     "K3",     tempVar);
  BTN_TEST_PRINT(btnK4,     "K4",     tempVar);
  BTN_TEST_PRINT(btnHome,   "Home",   tempVar);

  // Else, just show "...", so that we know time has passed in the console.
  BTN_TEST_PRINT(!tempVar, "...", tempVar);
}

inline void loop_SNES()
{
  unsigned char *rawDataPtr = rawData;
 
  noInterrupts();
  
  // Each cycle starts with latch
  WAIT_FALLING_EDGE_PINC( 1 );

  // Now there are 16 clock pulses
  // Controller writes data on the rising edge, and SNES samples on the falling
  // Buttons are low-active, which matches the UD-USB

  // Note: The very first button is driven at the falling edge of latch
  // Button: B
  *rawDataPtr = !PINB_READ(3);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Y
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINB_READ(0);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Select
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(3);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Start
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(2);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Up
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(7);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Down
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(6);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Left
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(5);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Right
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(4);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: A
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINB_READ(4);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: X
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINB_READ(1);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: L
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINC_READ(4);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: R
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINB_READ(2);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE(0, LOW);
  } else {
    PINC_WRITE(0, HIGH);
  }
  rawDataPtr++;

  // Not used (always high)
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr++ = 0;
  PINC_WRITE(0, HIGH);
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr++ = 0;
  PINC_WRITE(0, HIGH);
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr++ = 0;
  PINC_WRITE(0, HIGH);
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr++ = 0;
  PINC_WRITE(0, HIGH);

  // Done; drive serial pin low until next cycle.
  PINC_WRITE(0, LOW);

  interrupts();

  // Update NintendoSpy
  sendRawData( 0 , SNES_BITCOUNT );
}

void loop()
{
#ifdef MODE_INPUT_TEST
  loop_INPUTTEST();
#else
  loop_SNES();
#endif
}
