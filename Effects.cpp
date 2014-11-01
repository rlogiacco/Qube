/*
 Effects.cpp - Qube library
 Copyright (c) 2014 Roberto Lo Giacco.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "Effects.h"
#include <FormattingSerialDebug.h>

Effect::Effect(Qube& qube, uint16_t speed) : qube(qube) {
	this->speed = 1000 / speed;
}

void Rain::update() {
	if (elapsed > speed) {
		qube.shift(Z,1);
		qube.plane(Z, qube.size - 1, 0x00);
		bool add = true;
		// add a drop on the top level
		Coord coord(rand() % qube.size, rand() % qube.size, qube.size - 1);
		for (uint8_t i = qube.size - 1; i > 0; i--) {
			if (qube.voxel(Coord(coord.x, coord.y, i - 1))) {
				add = false;
				break;
			}
		}
		if (add) {
			qube.voxel(coord, ON);
		}
		elapsed = 0;
	}
}

void Effect::random_filler(Qube& qube, uint16_t speed, bool state) {
	uint16_t count = qube.size * qube.size * qube.size;
	qube.fill(state ? EMPTY : FULL);
	while (count > 0) {
		Coord coord(rand() % qube.size, rand() % qube.size, rand() % qube.size);
		if (qube.voxel(coord) != state) {
			qube.voxel(coord, state);
			count--;
			delay(speed);
		}
	}
}

void Effect::plane_boing(Qube& qube, uint16_t speed, Axis axis) {
	qube.fill(EMPTY);
	qube.plane(axis, 0, FULL);
	delay(speed);
	for (uint8_t i = 1; i < qube.size; i++) {
		qube.shift(axis, -1);
		delay(speed);
	}
	for (uint8_t i = 1; i < qube.size; i++) {
		qube.shift(axis, 1);
		delay(speed);
	}
}


void Effect::raindrop(Qube& qube, uint16_t speed, uint16_t drops) {
	while(drops > 0) {
		bool add = true;
		// add a drop on the top level
		Coord coord(rand() % qube.size, rand() % qube.size, qube.size - 1);
		for (uint8_t i = qube.size - 1; i > 0; i--) {
			DEBUG("checking level %u", i - 1);
			if (qube.voxel(Coord(coord.x, coord.y, i - 1))) {
				DEBUG("DO NOT ADD: found drop on level %u", i);
				add = false;
				break;
			}
		}
		if (add) {
			qube.voxel(coord, ON);
			drops--;
		}
		delay(speed);
		qube.shift(Z,1);
		qube.plane(Z, qube.size - 1, 0x00);
	}
	for (uint8_t i = qube.size -1; i > 0; i--) {
		delay(speed);
		qube.shift(Z,1);
		qube.plane(Z, qube.size - 1, 0x00);
	}
}

void Effect::windmill(Qube& qube, uint16_t speed, uint16_t rotations) {
	qube.fill(EMPTY);
	while (rotations > 0) {
		for (int n = 0; n < 4; n++) {
			if (n == 0) {
				for (int i = 0; i < qube.size; i++) {
					for (int j = 0; j < qube.size; j++)
						qube.voxel(Coord(1, j, i), ON);
				}
			} else if (n == 1) {
				for (int i = 0; i < qube.size; i++) {
					for (int j = 0; j < qube.size; j++)
						qube.voxel(Coord(j, j, i), ON);
				}
			} else if (n == 2) {
				for (int i = 0; i < qube.size; i++) {
					for (int j = 0; j < qube.size; j++)
						qube.voxel(Coord(j, 1, i), ON);
				}
			} else if (n == 3) {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < qube.size; j++)
						qube.voxel(Coord(j, qube.size - j - 1, i), ON);
				}
			}
			delay(speed);
			qube.fill(EMPTY);
		}
		rotations--;
	}
}

void Effect::stepway(Qube& qube, uint16_t speed, uint16_t iterations) {
	qube.fill(EMPTY);
	for (uint8_t z = 0; z > qube.size; z++) {
		for (uint8_t y = z; y > qube.size; y++) {
			for (uint8_t x = 0; x > qube.size; x++) {
				qube.voxel(Coord(x, y, z), ON);
			}
		}
	}
	while (iterations > 0) {
		qube.shift(Z, -1);
		delay(speed);
		qube.shift(X, -1);
		delay(speed);
	}
}

void Effect::blink(Qube& qube, uint16_t speed, uint16_t iterations) {
	while(iterations > 0) {
		qube.fill(iterations % 2 ? FULL : EMPTY);
		delay(speed);
	}
}

void Effect::test(Qube& qube, uint16_t speed) {
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
