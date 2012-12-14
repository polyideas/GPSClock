// GPS clock using test code from Adafruit's GPS and I2C 7-segment LED backpack.
// Original GPS code here: https://github.com/adafruit/Adafruit-GPS-Library
// Original LED Backpack code here: https://github.com/adafruit/Adafruit-LED-Backpack-Library
//
// Big thanks to Lady Ada from Adafruit and driverblock from the Adafruit forums for help getting this going.
//
// Tested and works great with:
// Adafruit GPS with the MTK33x9 chipset ---------> https://www.adafruit.com/products/746
// Arduino UNO -----------------------------------> https://www.adafruit.com/products/50
// 4-Digit, 7-Segment Display with I2C Backpack --> https://www.adafruit.com/products/879
// Plastic mounting plate for Ardunio ------------> https://www.adafruit.com/products/275
// Breadboarding wire ----------------------------> https://www.adafruit.com/products/153

// Pick one up today at the Adafruit electronics shop 
// and help support open source hardware & software! -ada
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_GPS.h>
#if ARDUINO >= 100
 #include <SoftwareSerial.h>
#else
  // Older Arduino IDE requires NewSoftSerial, download from:
  // http://arduiniana.org/libraries/newsoftserial/
 #include <NewSoftSerial.h>
 // DO NOT install NewSoftSerial if using Arduino 1.0 or later!
#endif

int fourdigitTime;
Adafruit_7segment matrix = Adafruit_7segment();
// Connect the GPS Power pin to 5V
// Connect the GPS Ground pin to ground
// Connect the LED Backpack Power pin to 5V
// Connect the LED Backpack Ground pin to ground
// If using software serial (sketch example default):
//   Connect the GPS TX (transmit) pin to Digital 8
//   Connect the GPS RX (receive) pin to Digital 7
//   Connect the LED Backpack C pin to Analog 5
//   Connect the LED Backpack D pin to Analog 4

// If using software serial, keep these lines enabled
// (you can change the pin numbers to match your wiring):
#if ARDUINO >= 100
  SoftwareSerial mySerial(8, 7);
#else
  NewSoftSerial mySerial(8, 7);
#endif
Adafruit_GPS GPS(&mySerial);
// If using hardware serial (e.g. Arduino Mega), comment
// out the above six lines and enable this line instead:
//Adafruit_GPS GPS(&Serial1);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  false

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

void setup()  
{
   matrix.begin(0x70); 
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);

  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);
}


// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();
void loop()                     // run over and over again
{
  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) UDR0 = c;
      // writing direct to UDR0 is much much faster than Serial.print 
      // but only one character can be written at a time. 
  }
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    GPS.parse(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

if ((GPS.month = 3) && (GPS.day >=


 if (GPS.hour <= 8) {
       fourdigitTime = ((GPS.hour + 4) *100)+ GPS.minute;
 } else if ((GPS.hour > 8) && (GPS.hour <= 20)) {
       fourdigitTime = ((GPS.hour - 8) *100)+ GPS.minute;   
 } else if (GPS.hour > 20) {
        fourdigitTime = ((GPS.hour - 20) *100)+ GPS.minute;   
 }
    matrix.print(fourdigitTime);
    matrix.drawColon(true);
    matrix.writeDisplay();
    Serial.print("fourdigitTime: "); Serial.println(fourdigitTime); 

  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer
    
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
  }
}
