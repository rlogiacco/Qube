#include <Effects.h>
#include <Qube.h>
#include <FormattingSerialDebug.h>

#define COMMON_CATHODE true
#define BUTTON_PIN 2
#define SIZE 3
const int pins[SIZE][SIZE] = { //
		{ 3, 4, 5 }, // row 1
		{ 6, 7, 8 }, // row 2
		{ 9, 10, 11 }   // row 3
};
const int layers[] = { A0, A1, A2 };

Qube qube(SIZE);

void setup() {
	SERIAL_DEBUG_SETUP(9600);
	for (int i = 0; i < SIZE; i++) {
		pinMode(layers[i], OUTPUT);
		for (int j = 0; j < SIZE; j++)
			pinMode(pins[i][j], OUTPUT);
	}

	pinMode(BUTTON_PIN, INPUT_PULLUP);

	// interrupt setup
	// frequency should be no less than 100Hz * cube size for flicker free results
	noInterrupts();
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	// Compare value 16M / prescaler / frequency
	OCR1A = 10; // 6.25kHz or 160us
	// OCR1A = 208 // this is minimum flicker free update frequency of 300Hz
	// OCR1A = 625 // try this to visualize flickering (frequency of 100Hz)
	// OCR1A = 3125 // try this to visualize multiplexing (frequency of 20Hz)


	// Turn on CTC mode:
	TCCR1B  |= (1 << WGM12);
	// Set CS12 bit for 256 prescaler:
	TCCR1B  |= (1 << CS12);
	// Enable timer compare interrupt:
	TIMSK1 |= (1 << OCIE1A);
	interrupts();
}

volatile uint8_t layer = 0;

// This needs to be re-implemented based on your specific
// cube implementation: the following example uses a simple
// implementation directly driven by the MCU and 3 npn transistors
ISR(TIMER1_COMPA_vect) {
	digitalWrite(layers[layer++], (COMMON_CATHODE ? LOW : HIGH));
	if (layer == qube.size)
		layer = 0;

	// if qube is disabled then we have nothing to do
	if (qube) {
		uint8_t position = 0;
		for (uint8_t y = 0; y < qube.size; y++) {
			for (uint8_t x = 0; x < qube.size; x++) {
				uint8_t byte = qube[layer][position / 8];
				uint8_t shift = 7 - position % 8;
				uint8_t bit = (((COMMON_CATHODE ? byte : ~byte) >> shift) & 0x1);
				position++;
				digitalWrite(pins[y][x], bit);
			}
		}
		digitalWrite(layers[layer], (COMMON_CATHODE ? HIGH : LOW));
	}
}

// This simple loop will test your wiring and bit to LED mapping
void loop() {
	int speed = 250;
	// Blink 3 times the first voxel
	for (uint8_t i = 0; i < 3; i++) {
		qube.voxel(Coord(0, 0, 0), ON);
		delay(speed);
		qube.voxel(Coord(0, 0, 0), OFF);
		delay(speed);
	}
	// Turn on each voxel in a specific order:
	// rows, columns, layers
	for (uint8_t z = 0; z < qube.size; z++) {
		for (uint8_t y = 0; y < qube.size; y++) {
			for (uint8_t x = 0; x < qube.size; x++) {
				qube.voxel(Coord(x, y, z), ON);
				delay(speed);
			}
		}
	}
	// Turn off each voxel in reverse order
	for (uint8_t z = qube.size; z > 0; z--) {
		for (uint8_t y = qube.size; y > 0; y--) {
			for (uint8_t x = qube.size; x > 0; x--) {
				qube.voxel(Coord(x - 1, y - 1, z - 1), OFF);
				delay(speed);
			}
		}
	}
}
