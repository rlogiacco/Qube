#include <Elapsed.h>
#include <Effects.h>
#include <Qube.h>
#include <FormattingSerialDebug.h>

#define BUTTON_PIN 2
#define SIZE 3
const int pins[SIZE][SIZE] = { //
		{ 3, 4, 5 }, // row 1
		{ 6, 7, 8 }, // row 2
		{ 9, 10, 11 }   // row 3
};
const int layers[] = { A2, A1, A0 };

Qube qube(SIZE);

#define EFFECTS_SIZE 8
Effect** effects = new Effect*[EFFECTS_SIZE];
uint8_t currentEffect = EFFECTS_SIZE - 1;
Effect* effect;

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

	qube.test(200);

	effects[0] = new Rain(qube, 5, 100);
	effects[1] = new RandomFill(qube, 5);
	effects[2] = new RandomBlinker(qube, 2, 20);
	effects[3] = new PlaneBounce(qube, 5, Z);
	effects[4] = new Windmill(qube, 5, Z);
	effects[5] = new Blink(qube, 3);

	// last effects are carousels: do not include those into carousel effects or it will get recursive
	effects[6] = new Carousel(qube, 120, effects, EFFECTS_SIZE - 2);
	effects[7] = new RandomCarousel(qube, 120, effects, EFFECTS_SIZE - 2);

	effect = effects[currentEffect];
	effect->init();
}

volatile uint8_t layer = 0;

ISR(TIMER1_COMPA_vect) {
	digitalWrite(layers[layer++], HIGH);
	if (layer == qube.size)
		layer = 0;
	if (qube) {
		uint8_t position = 0;
		for (uint8_t y = 0; y < qube.size; y++) {
			for (uint8_t x = 0; x < qube.size; x++) {
				uint8_t byte = qube[layer][position / 8];
				uint8_t shift = 7 - position % 8;
				uint8_t bit = ((byte >> shift) & 0x1);
				position++;
				digitalWrite(pins[y][x], ~bit);
			}
		}
		digitalWrite(layers[layer], LOW);
	}
}

#define DEBOUNCE 15
#define DMASK ((1ULL<<DEBOUNCE)-1)
#define DF (1ULL<<(DEBOUNCE-1))
#define DR (DMASK-DF)

// macro for detection of falling edge and debouncing
#define DFE(signal, state) ((state=((state<<1)|(signal&1))&DMASK)==DF)

Elapsed elapsed = 0;
void loop() {
	if(DFE(digitalRead(BUTTON_PIN), pinState) || elapsed >= 300000) {
		elapsed = 0;
		currentEffect = ++currentEffect % EFFECTS_SIZE;
		DEBUG("current effect is %u", currentEffect);
		effect = effects[currentEffect];
		effect->init();
	}
	effect->update();
}
