/*
 * Cube.h
 *
 *  Created on: 06/ago/2014
 *      Author: ii40253
 */

#ifndef CUBE_H_
#define CUBE_H_

#include <Arduino.h>

typedef void (*displayFn)(void);

enum State {
	ON, OFF, FLIP
};

enum Axis {
	X, Y, Z
};

class Cube {
public:
	Cube(uint8_t size, uint8_t fps = 24);

	bool get(uint8_t x, uint8_t y, uint8_t z);
	void set(uint8_t x, uint8_t y, uint8_t z, bool active = true);
	void flip(uint8_t x, uint8_t y, uint8_t z);

	void set(uint8_t index, Axis axis, uint16_t bitmask);
	void set(uint8_t** voxels);

	void display(byte* frames);
	void display(Stream* stream);
	void display(displayFn function);
private:
	uint8_t size;
	uint8_t fps;
};

#endif /* CUBE_H_ */
