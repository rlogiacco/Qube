// Do not remove the include below
#include "qube.h"

#include <LedControl.h>
#include <SerialDebug.h>

#define LED_DRV_DIN 12
#define LED_DRV_CLK 10
#define LED_DRV_LAD 11

#define DELAY 500

LedControl driver = LedControl(LED_DRV_DIN, LED_DRV_CLK, LED_DRV_LAD, 1);

void setup() {
	SERIAL_DEBUG_SETUP(9600);

	pinMode(13, OUTPUT);

	//we have already set the number of devices when we created the LedControl
	int devices = driver.getDeviceCount();
	//we have to init all devices in a loop
	for (int address = 0; address < devices; address++) {
		/*The MAX72XX is in power-saving mode on startup*/
		driver.shutdown(address, false);
		/* Set the brightness to a medium values */
		driver.setIntensity(address, 15);
		/* and clear the display */
		driver.clearDisplay(address);
	}
}

int row = 0;
int col = 0;
int address = 0;
int intensity = 8;

void loop() {
	//read the number cascaded devices
	int devices = driver.getDeviceCount();

	//we have to init all devices in a loop

	//*
	char c = Serial.read();
	if (c > 0) {
		driver.setLed(address, row, col, false);
	}
	if (c == '+') {
		col++;
	} else if (c == '-') {
		col--;
	} else if (c == 'n') {
		row++;
	} else if (c == 'p') {
		row--;
	} else if (c == 'a') {
		intensity++;
		driver.setIntensity(address, intensity);
	} else if (c == 'z') {
		intensity--;
		driver.setIntensity(address, intensity);
	}
	if (c > 0) {
		driver.setLed(address, row, col, true);
		DEBUG(address, row, col);
	}
	/*
	digitalWrite(13, HIGH);
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 8; col++) {
			for (int address = 0; address < devices; address++) {
				driver.setLed(address, row, col, true);
				DEBUG(address, row, col, "ON");
				delay(DELAY);
			}
		}
	}
	digitalWrite(13, LOW);
	for (int row = 0; row < 2; row++) {
		for (int col = 0; col < 8; col++) {
			for (int address = 0; address < devices; address++) {
				driver.setLed(address, row, col, false);
				DEBUG(address, row, col, "OFF");
				delay(DELAY);
			}
		}
	}
	*/
}
