/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NintendoSpy Firmware for Arduino
// v1.0.1
// Written by jaburns


// Used to test new controllers hooked up to UD-USB.
// Overrides MODE_SNES
//#define MODE_INPUT_TEST

// Actual functionality. 
#define MODE_SNES

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


#define PIND_READ( pin ) (PIND&(1<<(pin)))
#define PINC_READ( pin ) (PINC&(1<<(pin)))
#define PINB_READ( pin ) (PINB&(1<<(pin)))



#define PIND_WRITE_HIGH( pin )  (PORTD = PORTD | (1<<(pin)))
#define PIND_WRITE_LOW( pin )  (PORTD = PORTD & (~(1<<(pin))))
#define PINB_WRITE_HIGH( pin )  (PORTB = PORTB | (1<<(pin)))
#define PINB_WRITE_LOW( pin )  (PORTB = PORTB & (~(1<<(pin))))
#define PINC_WRITE_HIGH( pin )  (PORTC = PORTC | (1<<(pin)))
#define PINC_WRITE_LOW( pin )  (PORTC = PORTC & (~(1<<(pin))))

#define WAIT_FALLING_EDGE_PIND( pin ) while( !PIND_READ(pin) ); while( PIND_READ(pin) );
#define WAIT_FALLING_EDGE_PINB( pin ) while( !PINB_READ(pin) ); while( PINB_READ(pin) );
#define WAIT_FALLING_EDGE_PINC( pin ) while( !PINC_READ(pin) ); while( PINC_READ(pin) );

#define WAIT_RISING_EDGE_PINC( pin ) while( PINC_READ(pin) ); while( !PINC_READ(pin) );


#define MODEPIN_SNES 0
#define MODEPIN_N64  1
#define MODEPIN_GC   2

#define N64_PIN        2
#define N64_PREFIX     9
#define N64_BITCOUNT  32

#define SNES_LATCH      3
#define SNES_DATA       4
#define SNES_CLOCK      6
#define SNES_BITCOUNT  16
#define  NES_BITCOUNT   8

#define GC_PIN        5
#define GC_PREFIX    25
#define GC_BITCOUNT  64

#define ZERO  '\0'  // Use a byte value of 0x00 to represent a bit with value 0.
#define ONE    '1'  // Use an ASCII one to represent a bit with value 1.  This makes Arduino debugging easier.
#define SPLIT '\n'  // Use a new-line character to split up the controller state packets.




// Declare some space to store the bits we read from a controller.
unsigned char rawData[ 128 ];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// General initialization, just sets all pins to input and starts serial communication.
void setup()
{
/*    PORTD = 0x00;
    DDRD  = 0x00;
    PORTC = 0xFF; // Set the pull-ups on the port we use to check operation mode.
    DDRC  = 0x00;*/

   DDRD = B00000010; // Digital 0 to 7
   DDRB = B00000000; // Digital 8 to 13
   DDRC = B00000001; // Analog 0 to 5


   
/*   pinMode(2, OUTPUT);  // We send latch to controller
   pinMode(3, OUTPUT);  // We send clock to controller
   pinMode(4, INPUT);  // We read data from controller
   pinMode(5, INPUT);  // We read latch from SNES console
   pinMode(6, INPUT);  // We read clock from SNES console
   pinMode(7, OUTPUT);  // We send data to SNES console
   */


    Serial.begin( 115200 );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sends a packet of controller data over the Arduino serial interface.
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
  btnStart = PIND_READ(2);
  btnSelect = PIND_READ(3);
  btnRight = PIND_READ(4);
  btnLeft = PIND_READ(5);
  btnDown = PIND_READ(6);
  btnUp = PIND_READ(7);

  // Second set
  btnP1 = PINB_READ(0);
  btnP2 = PINB_READ(1);
  btnP3 = PINB_READ(2);
  btnK1 = PINB_READ(3);
  btnK2 = PINB_READ(4);
  btnK3 = PINB_READ(5);

  // Third set
  btnHome = PINC_READ(3);
  btnP4 = PINC_READ(4);
  btnK4 = PINC_READ(5);

  // SNES stuff
  snesLatch = PINC_READ(1);
  snesClock = PINC_READ(2);

  // Output!
  tempVar = 0;
  if (btnStart == LOW) {
    Serial.println("Start");
    tempVar = 1;
  }
  if (btnSelect == LOW) {
    Serial.println("Select");
    tempVar = 1;
  }
  if (btnRight == LOW) {
    Serial.println("Right");
    tempVar = 1;
  }
  if (btnLeft == LOW) {
    Serial.println("Left");
    tempVar = 1;
  }
  if (btnDown == LOW) {
    Serial.println("Down");
    tempVar = 1;
  }
  if (btnUp == LOW) {
    Serial.println("Up");
    tempVar = 1;
  }
  if (btnP1 == LOW) {
    Serial.println("P1");
    tempVar = 1;
  }
  if (btnP2 == LOW) {
    Serial.println("P2");
    tempVar = 1;
  }
  if (btnP3 == LOW) {
    Serial.println("P3");
    tempVar = 1;
  }
  if (btnK1 == LOW) {
    Serial.println("K1");
    tempVar = 1;
  }
  if (btnK2 == LOW) {
    Serial.println("K2");
    tempVar = 1;
  }
  if (btnK3 == LOW) {
    Serial.println("K3");
    tempVar = 1;
  }
  if (btnHome == LOW) {
    Serial.println("Home");
    tempVar = 1;
  }
  if (btnP4 == LOW) {
    Serial.println("P4");
    tempVar = 1;
  }
  if (btnK4 == LOW) {
    Serial.println("K4");
    tempVar = 1;
  }
  if (tempVar == 0) {
    Serial.println("...");
  }
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
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Y
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINB_READ(0);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Select
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(3);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Start
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(2);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Up
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(7);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Down
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(6);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Left
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(5);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: Right
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PIND_READ(4);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: A
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINB_READ(4);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: X
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINB_READ(1);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: L
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINC_READ(4);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Next buttons wait for rising edge of clock.
  // Button: R
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr = !PINB_READ(2);
  if (*rawDataPtr==HIGH) {
    PINC_WRITE_LOW(0);
  } else {
    PINC_WRITE_HIGH(0);
  }
  rawDataPtr++;

  // Not used (always high)
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr++ = 0;
  PINC_WRITE_HIGH(0);
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr++ = 0;
  PINC_WRITE_HIGH(0);
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr++ = 0;
  PINC_WRITE_HIGH(0);
  WAIT_RISING_EDGE_PINC( 2 );
  *rawDataPtr++ = 0;
  PINC_WRITE_HIGH(0);

  // Done; drive serial pin low until next cycle.
  PINC_WRITE_LOW(0);

  interrupts();

  // Update NintendoSpy
  sendRawData( 0 , SNES_BITCOUNT );
  
  /*
  // Forward latch to controller.
  latchVal = PIN_READ(5);
  if (latchVal) {
    PIN_WRITE_HIGH(2);
  } else {
    PIN_WRITE_LOW(2);
  }

  // Forward clock to controller.
  clockVal = PIN_READ(6);
  if (clockVal) {
    PIN_WRITE_HIGH(3);
  } else {
    PIN_WRITE_LOW(3);
  }

  
  // TEMP: forward data to SNES
  dataVal = PIN_READ(4);
  if (dataVal) {
    PIN_WRITE_HIGH(7);
  } else {
    PIN_WRITE_LOW(7);
  }

*/
  // TODO: Here's where we need to react to the latch+clock, update button states, and then do latching on output
  
  

// TEMP
/*tempVar = digitalRead(2);
Serial.println(tempVar);
digitalWrite(3, HIGH);
*/
    


  /*
    noInterrupts();
    read_shiftRegister< SNES_LATCH , SNES_DATA , SNES_CLOCK >( SNES_BITCOUNT );
    interrupts();
    sendRawData( 0 , SNES_BITCOUNT );
    */
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino sketch main loop definition.
void loop()
{
#ifdef MODE_INPUT_TEST
  loop_INPUTTEST();
#else
  #ifdef MODE_SNES
      loop_SNES();
  #endif
#endif
}
