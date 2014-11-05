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

void Rain::init() {
	qube.fill(EMPTY);
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

RandomFill::RandomFill(Qube& qube, uint16_t speed, bool reverse) : Effect(qube, speed) {
	this->state = !reverse;
}
void RandomFill::init() {
	this->counter = qube.size * qube.size * qube.size;
	qube.fill(state ? EMPTY : FULL);
}
void RandomFill::update() {
	if (elapsed > speed) {
		if (counter > 0) {
			Coord coord(rand() % qube.size, rand() % qube.size, rand() % qube.size);
			if (qube.voxel(coord) != state) {
				qube.voxel(coord, state);
				counter--;
				elapsed = 0;
			}
		} else {
			counter = qube.size * qube.size * qube.size;
			state = !state;
		}
	}
}


PlaneBounce::PlaneBounce(Qube& qube, uint16_t speed, Axis axis) : Effect(qube, speed) {
	this->axis = axis;
}
void PlaneBounce::init() {
	qube.fill(EMPTY);
	this->counter = 0;
	qube.plane(axis, 0, FULL);
}
void PlaneBounce::update() {
	if (elapsed > speed) {
		if (counter == 0) {
			counter = (qube.size - 1) * 2;
		}
		qube.shift(axis, (counter >= qube.size) ? -1 : 1);
		counter--;
		elapsed = 0;
	}
}

Windmill::Windmill(Qube& qube, uint16_t speed, Axis axis) : Effect(qube, speed) {
	this->axis = axis;
}
void Windmill::init() {
	qube.fill(EMPTY);
	this->counter = (qube.size) - 1 * 2;
}
void Windmill::update() {
	if (elapsed > speed) {
		if (counter == 0) {
			counter = (qube.size - 1) * 2;
		}
		qube.fill(EMPTY);
		if (counter == 4) {
			for (int i = 0; i < qube.size; i++) {
				for (int j = 0; j < qube.size; j++)
					qube.voxel(Coord(1, j, i), ON);
			}
		} else if (counter == 3) {
			for (int i = 0; i < qube.size; i++) {
				for (int j = 0; j < qube.size; j++)
					qube.voxel(Coord(j, j, i), ON);
			}
		} else if (counter == 2) {
			for (int i = 0; i < qube.size; i++) {
				for (int j = 0; j < qube.size; j++)
					qube.voxel(Coord(j, 1, i), ON);
			}
		} else if (counter == 1) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < qube.size; j++)
					qube.voxel(Coord(j, qube.size - j - 1, i), ON);
			}
		}
		counter--;
		elapsed = 0;
	}
}

Blink::Blink(Qube& qube, uint16_t speed) : Effect(qube, speed) {
	this->state = false;
}
void Blink::init() { }
void Blink::update() {
	if (elapsed > speed) {
		qube.fill(state ? FULL : EMPTY);
		state = !state;
		elapsed = 0;
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

void test(Qube& qube, uint16_t speed) {
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
