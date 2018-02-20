// UDusbSpy - by Seth N. Hetu (sorlok_reaves)
// Based on NintendoSpy by Jeremy Burns (jaburns)
// Available under the terms of the MIT license; see the LICENSE file for details

// Used to test new controllers hooked up to UD-USB.
// Feedback will be provided through the console if enabled.
// You must disable this if you want "normal" operation.
//#define MODE_INPUT_TEST

// Read from a register (reg) and pin (pin) 
#define PIN_READ( reg, pin ) (reg&(1<<(pin)))

// Write low/high to a register and pin
#define PIN_WRITE_HIGH( reg, pin )  (reg = reg | (1<<(pin)))
#define PIN_WRITE_LOW(  reg, pin )  (reg = reg & (~(1<<(pin))))

// Wait for a falling/rising edge of a register and pin
#define WAIT_FALLING_EDGE( reg, pin ) while( !PIN_READ(reg, pin) ); while(  PIN_READ(reg, pin) );
#define WAIT_RISING_EDGE(  reg, pin ) while(  PIN_READ(reg, pin) ); while( !PIN_READ(reg, pin) );

// How many bits does the SNES expect to be shifted?
#define SNES_BITCOUNT  16

// Handy defines from NintendoSpy
#define ZERO  '\0'  // Use a byte value of 0x00 to represent a bit with value 0.
#define ONE    '1'  // Use an ASCII one to represent a bit with value 1.  This makes Arduino debugging easier.
#define SPLIT '\n'  // Use a new-line character to split up the controller state packets.

// Used as part of the input test
int tempVar = 0;

// Loop var
unsigned char i = 0;

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


inline void loop_INPUTTEST()
{
  // Read UD-USB's button states
  // First set
  btnStart = !PIN_READ(PIND, 2);
  btnSelect = !PIN_READ(PIND, 3);
  btnRight = !PIN_READ(PIND, 4);
  btnLeft = !PIN_READ(PIND, 5);
  btnDown = !PIN_READ(PIND, 6);
  btnUp = !PIN_READ(PIND, 7);

  // Second set
  btnP1 = !PIN_READ(PINB, 0);
  btnP2 = !PIN_READ(PINB, 1);
  btnP3 = !PIN_READ(PINB, 2);
  btnK1 = !PIN_READ(PINB, 3);
  btnK2 = !PIN_READ(PINB, 4);
  btnK3 = !PIN_READ(PINB, 5);

  // Third set
  btnHome = !PIN_READ(PINC, 3);
  btnP4 = !PIN_READ(PINC, 4);
  btnK4 = !PIN_READ(PINC, 5);

  // Output!
  tempVar = 0;
  if (btnStart) {
    Serial.println("Start");
    tempVar = 1;
  }
  if (btnSelect) {
    Serial.println("Select");
    tempVar = 1;
  }
  if (btnRight) {
    Serial.println("Right");
    tempVar = 1;
  }
  if (btnLeft) {
    Serial.println("Left");
    tempVar = 1;
  }
  if (btnDown) {
    Serial.println("Down");
    tempVar = 1;
  }
  if (btnUp) {
    Serial.println("Up");
    tempVar = 1;
  }
  if (btnP1) {
    Serial.println("P1");
    tempVar = 1;
  }
  if (btnP2) {
    Serial.println("P2");
    tempVar = 1;
  }
  if (btnP3) {
    Serial.println("P3");
    tempVar = 1;
  }
  if (btnK1) {
    Serial.println("K1");
    tempVar = 1;
  }
  if (btnK2) {
    Serial.println("K2");
    tempVar = 1;
  }
  if (btnK3) {
    Serial.println("K3");
    tempVar = 1;
  }
  if (btnHome) {
    Serial.println("Home");
    tempVar = 1;
  }
  if (btnP4) {
    Serial.println("P4");
    tempVar = 1;
  }
  if (btnK4) {
    Serial.println("K4");
    tempVar = 1;
  }
  if (!tempVar) {
    Serial.println("...");
  }
}

inline void loop_SNES()
{
  unsigned char *rawDataPtr = rawData;
 
  noInterrupts();
  
  // Each cycle starts with latch
  WAIT_FALLING_EDGE( PINC, 1 );

  // Now there are 16 clock pulses
  // Controller writes data on the rising edge, and SNES samples on the falling
  // Buttons are low-active, which matches the UD-USB

  // Note: The very first button is driven at the falling edge of latch
  // Button: B
  *rawDataPtr = !PIN_READ(PINB, 3);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Y
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PINB, 0);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Select
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PIND, 3);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Start
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PIND, 2);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Up
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PIND, 7);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Down
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PIND, 6);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Left
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PIND, 5);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Right
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PIND, 4);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: A
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PINB, 4);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: X
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PINB, 1);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: L
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PINC, 4);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: R
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr = !PIN_READ(PINB, 2);
  if (*rawDataPtr==HIGH) {
    PIN_WRITE_LOW(PINC, 0);
  } else {
    PIN_WRITE_HIGH(PINC, 0);
  }
  rawDataPtr++;

  // Not used (always high)
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr++ = 0;
  PIN_WRITE_HIGH(PINC, 0);
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr++ = 0;
  PIN_WRITE_HIGH(PINC, 0);
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr++ = 0;
  PIN_WRITE_HIGH(PINC, 0);
  WAIT_RISING_EDGE( PINC, 2 );
  *rawDataPtr++ = 0;
  PIN_WRITE_HIGH(PINC, 0);

  // Done; drive serial pin low until next cycle.
  PIN_WRITE_LOW(PINC, 0);

  interrupts();

  // Update NintendoSpy
  for(i=0; i<SNES_BITCOUNT; i++) {
    Serial.write(rawData[i]?ONE:ZERO);
  }
  Serial.write(SPLIT);
}

void loop()
{
#ifdef MODE_INPUT_TEST
  loop_INPUTTEST();
#else
  loop_SNES();
#endif
}

