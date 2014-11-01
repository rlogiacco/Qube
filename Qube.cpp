/*
 * Qube.cpp
 *
 *  Created on: 26/ott/2014
 *      Author: rlogiacco
 */


#include "Qube.h"

Qube::Qube(uint8_t size) {
	this->size = size;
	this->cube = (volatile uint8_t**)malloc(size * sizeof(uint8_t*));
	for (int i = 0; i < size; i++) {
		this->cube[i] = (uint8_t*)malloc(_divrup((size * size), 8));
	}
	this->fill(EMPTY);
}

Qube::~Qube() {
	for (int i = 0; i < _divrup((size * size), 8); i++) {
		free((uint8_t*)(cube[i]));
	}
	free(cube);
};

void Qube::print() {
	for (uint8_t z = 0; z < size; z++) {
		Serial.print("layer ");Serial.println(z);
		uint8_t position = 0;
		for (uint8_t y = 0; y < size; y++) {
			for (uint8_t x = 0; x < size; x++) {
				uint8_t byte = cube[z][position / 8];
				uint8_t shift = 7 - position % 8;
				uint8_t bit = ((byte >> shift) & 0x1 );
				position++;
				Serial.print(bit);
			}
			Serial.println();
		}
	}
}

void Qube::fill(unsigned char pattern) {
	for (uint8_t z = 0; z < size; z++) {
		for (uint8_t y = 0; y < _divrup((size * size), 8); y++) {
			cube[z][y] = pattern;
		}
	}
}

void Qube::fill(Fill fill) {
	switch (fill) {
		case FULL:
			this->fill(0xFF);
			break;
		case EMPTY:
			this->fill(0x00);
			break;
	}
}

bool Qube::voxel(Coord coord) {
	if (coord.x < size && coord.y < size && coord.z < size) {
		uint8_t position = (coord.y * size) + coord.x;
		uint8_t shift = 7 - position % 8;
		return ((cube[coord.z][position / 8] >> shift) & 0x1 ) == 0x1;
	} else {
		return false;
	}
}

void Qube::voxel(Coord coord, Bit bit) {
	if (coord.x < size && coord.y < size && coord.z < size) {
		uint8_t position = (coord.y * size) + coord.x;
		switch (bit) {
			case ON:
				cube[coord.z][position / 8] |= (1 << (7 - position % 8));
				break;
			case OFF:
				cube[coord.z][position / 8] &= ~(1 << (7 - position % 8));
				break;
			case FLIP:
				cube[coord.z][position / 8] ^= (1 << (7 - position % 8));
				break;
		}
	}
}

void Qube::voxel(Coord coord, bool bit) {
	voxel(coord, bit ? ON : OFF);
}

void Qube::plane(Axis axis, uint8_t index, Fill fill) {
	uint8_t bits = size * size;
	switch (axis) {
		case X:
			switch (fill) {
				case FULL:
					break;
				case EMPTY:
					break;
				case WIREFRAME:
				case HOLLOW:
					break;
			}
			break;
		case Y:
			switch (fill) {
				case FULL:
					break;
				case EMPTY:
					break;
				case WIREFRAME:
				case HOLLOW:
					break;
			}
			break;
		case Z:
			switch (fill) {
				case FULL:
					for (uint8_t i = 0; i < bits / 8; i++) {
						cube[index][i] = 0xFF;
					}
					cube[index][bits / 8] |= (0xFF << (7 - bits % 8));
					break;
				case EMPTY:
					for (uint8_t i = 0; i < bits / 8; i++) {
						cube[index][i] = 0x00;
					}
					cube[index][bits / 8] &= ~(0xFF << (7 - bits % 8));
					break;
				case WIREFRAME:
				case HOLLOW:
					for (uint8_t position = 0; position < bits; position++) {
						if (position < size || position % size == 0 || position % size == size -1 || position >= (bits - size)) {
							cube[index][position / 8] |= (1 << (7 - position % 8));
						}
					}
					break;
			}
			break;
	}
}

void Qube::plane(Axis axis, uint8_t index, uint8_t fill) {
	uint8_t bits = size * size;
	switch(axis) {
		case X:
			break;
		case Y:
			break;
		case Z:
			for (uint8_t i = 0; i <= bits / 8; i++) {
				cube[index][i] = fill;
			}
			break;
	}
}


void Qube::shift(Axis axis, int8_t steps) {
	if (steps == 0)
		return;

	switch (axis) {
		case X:
			break;
		case Y:
			break;
		case Z:
			if (steps > 0) {
				for (uint8_t i = 0; i < steps; i++) {
					volatile uint8_t* tmp = cube[0];
					for (uint8_t l = 0; l < size - 1; l++) {
						cube[l] = cube[l + 1];
					}
					cube[size - 1] = tmp;
				}
			} else {
				for (uint8_t i = 0; i < -steps; i++) {
					volatile uint8_t* tmp = cube[size - 1];
					for (uint8_t l = size - 1; l > 0; l--) {
						cube[l] = cube[(l - 1)];
					}
					cube[0] = tmp;
				}
			}
			break;
	}
}
