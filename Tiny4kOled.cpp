#include <Arduino.h>
#include <TinyBMP280.h>
#include <SoftwareSerial.h>
#include <TinyWireM.h>
#include <Tiny4kOLED.h>

SoftwareSerial mySerial(PB3, PB1);
tbmp::TinyBMP280 bmp;

  int32_t intValue;
  int32_t floatValue;

void setup() {
	  mySerial.begin(2400);

	  bmp.begin();
    oled.begin();
    oled.setFont(FONT6X8P);
    // Clear the memory before turning on the display
    oled.clear();

    // Turn on the display
    oled.on();

    // Switch the half of RAM that we are writing to, to be the half that is non currently displayed
    oled.switchRenderFrame();

}

void loop() {

	int32_t t = bmp.readIntTemperature();

	mySerial.print("T: "); mySerial.println(t);

  updateDisplay(t);
	delay(1000);
}


void updateDisplay(int32_t temperature) {
  // Clear the half of memory not currently being displayed.
  oled.clear();

  // Position the text cursor
  // In order to keep the library size small, text can only be positioned
  // with the top of the font aligned with one of the four 8 bit high RAM pages.
  // The Y value therefore can only have the value 0, 1, 2, or 3.
  // usage: oled.setCursor(X IN PIXELS, Y IN ROWS OF 8 PIXELS STARTING WITH 0);
  oled.setCursor(0, 1);


  intValue = temperature / 100;      // tens now = 2
  floatValue = temperature % 100;      // ones now = 6
  mySerial.println(intValue);
  mySerial.println(floatValue);

  oled.print("Temperature: ");
  oled.print(intValue);
  oled.print(".");
  oled.print(floatValue);
  oled.print("°C");


  // Swap which half of RAM is being written to, and which half is being displayed.
  // This is equivalent to calling both switchRenderFrame and switchDisplayFrame.
  oled.switchFrame();
}
