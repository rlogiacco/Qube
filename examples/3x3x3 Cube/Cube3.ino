#include <Elapsed.h>
#include <Effects.h>
#include <Qube.h>
#include <FormattingSerialDebug.h>

#define BUTTON_PIN 7
#define SIZE 3
const int pins[SIZE][SIZE] = { //
		{ A3, A1, A2 }, // row 1
		{ 13, 12, A0 }, // row 2
		{ 10, 9, 11 }   // row 3
};
const int layers[] = { 3, 2, 4 };

Qube qube(SIZE);

void setup() {
	SERIAL_DEBUG_SETUP(9600);
	for (int i = 0; i < SIZE; i++) {
		pinMode(layers[i], OUTPUT);
		for (int j = 0; j < SIZE; j++)
			pinMode(pins[i][j], OUTPUT);
	}

	pinMode(BUTTON_PIN, INPUT_PULLUP);

	noInterrupts();
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	// Compare value 16M / prescaler / frequency
	OCR1A = 10; // 6.25kHz
	// Turn on CTC mode:
	TCCR1B  |= (1 << WGM12);
	// Set CS12 bit for 256 prescaler:
	TCCR1B  |= (1 << CS12);
	// Enable timer compare interrupt:
	TIMSK1 |= (1 << OCIE1A);
	interrupts();
}

volatile uint8_t layer = 0;

ISR(TIMER1_COMPA_vect) {
	digitalWrite(layers[layer++], LOW);
	if (layer == qube.size)
		layer = 0;

	uint8_t position = 0;
	for (uint8_t y = 0; y < qube.size; y++) {
		for (uint8_t x = 0; x < qube.size; x++) {
			uint8_t byte = qube[layer][position / 8];
			uint8_t shift = 7 - position % 8;
			uint8_t bit = ((byte >> shift) & 0x1);
			position++;
			digitalWrite(pins[y][x], bit);
		}
	}
	digitalWrite(layers[layer], HIGH);
}

void loop() {
	int speed = 500;
	for (uint8_t i = 0; i < 3; i++) {
		qube.voxel(Coord(0, 0, 0), ON);
		delay(speed);
		qube.voxel(Coord(0, 0, 0), OFF);
		delay(speed);
	}
	for (uint8_t z = 0; z < qube.size; z++) {
		for (uint8_t y = 0; y < qube.size; y++) {
			for (uint8_t x = 0; x < qube.size; x++) {
				qube.voxel(Coord(x, y, z), ON);
				delay(speed);
			}
		}
	}
	for (uint8_t z = qube.size; z > 0; z--) {
		for (uint8_t y = qube.size; y > 0; y--) {
			for (uint8_t x = qube.size; x > 0; x--) {
				qube.voxel(Coord(x - 1, y - 1, z - 1), OFF);
				delay(speed);
			}
		}
	}
}
