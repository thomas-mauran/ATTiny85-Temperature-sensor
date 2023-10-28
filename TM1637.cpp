#include <Arduino.h>
#include <TinyBMP280.h>
#include <SoftwareSerial.h>
#include <TinyWireM.h>
#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/wdt.h> //Needed to enable/disable watch dog timer
#include "TM1637Display.h"

// SoftwareSerial mySerial(PB2, PB1);
tbmp::TinyBMP280 bmp;

bool displayToggle = false;
TM1637Display display(PB3, PB4); // CLC, DIO

void setup() {
	  // mySerial.begin(2400);

	  bmp.begin();
    display.setBrightness(1); // luminosité de 0 à 7
    display.clear();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN); //Power down everything, wake up from WDT
    sleep_enable();
}

//This runs each time the watch dog wakes us up from sleep
ISR(WDT_vect) {
  //watchdog_counter++;
  displayToggle=!displayToggle;
}

void loop() {
  setup_watchdog(9); //Setup watchdog to go off after 8sec
  sleep_mode(); //Go to sleep! Wake up 8 sec later

	int32_t t = bmp.readIntTemperature();

	// mySerial.print("T: "); mySerial.println(t);
  if (displayToggle)
  {
    display. showNumberDecEx(t, 0b01000000);
    delay(1000); // Adjust delay as needed

  }


//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
//From: http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void setup_watchdog(int timerPrescaler) {

  if (timerPrescaler > 9 ) timerPrescaler = 9; //Limit incoming amount to legal settings

  byte bb = timerPrescaler & 7; 
  if (timerPrescaler > 7) bb |= (1<<5); //Set the special 5th bit if necessary

  //This order of commands is important and cannot be combined
  MCUSR &= ~(1<<WDRF); //Clear the watch dog reset
  WDTCR |= (1<<WDCE) | (1<<WDE); //Set WD_change enable, set WD enable
  WDTCR = bb; //Set new watchdog timeout value
  WDTCR |= _BV(WDIE); //Set the interrupt enable, this will keep unit from resetting after each int
}