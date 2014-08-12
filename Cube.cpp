/*
 * Cube.cpp
 *
 *  Created on: 06/ago/2014
 *      Author: ii40253
 */

#include "Cube.h"

Cube::Cube(uint8_t size, uint8_t fps) {
	this->size = size;
	this->fps = fps;
}



void Cube::display(byte* frames) {
	// iterate over layers
	for (int layer = 0; layer < size; layer++) {
		// extract layer bits
		uint32_t bitmask = frames[0];
	}
}

void Cube::display(Stream* stream) {

}

void Cube::display(displayFn function) {

}

