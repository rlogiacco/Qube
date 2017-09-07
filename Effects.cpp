/*
 Effects.cpp - Qube library
 Copyright (c) 2014 Roberto Lo Giacco.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as 
 published by the Free Software Foundation, either version 3 of the 
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Effects.h"
#include <FormattingSerialDebug.h>

Effect::Effect(Qube& qube, uint16_t speed) :
		qube(qube) {
	this->speed = 1000 / speed;
}

void Rain::init() {
	DEBUG("rain effect activated");
	qube.fill(EMPTY);
	qube = true;
}
void Rain::update() {
	if (elapsed > speed) {
		qube.shift(Z, 1);
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

RandomFill::RandomFill(Qube& qube, uint16_t speed, bool reverse) :
		Effect(qube, speed) {
	this->state = !reverse;
	this->counter = 0;
}
void RandomFill::init() {
	DEBUG("random fill effect activated");
	this->counter = qube.size * qube.size * qube.size;
	qube.fill(state ? EMPTY : FULL);
	qube = true;
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

PlaneBounce::PlaneBounce(Qube& qube, uint16_t speed, Axis axis) :
		Effect(qube, speed) {
	this->axis = axis;
	this->counter = 0;
}
void PlaneBounce::init() {
	DEBUG("plane bouncer effect activated");
	qube.fill(EMPTY);
	this->counter = 0;
	qube.plane(axis, 0, FULL);
	qube = true;
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

Windmill::Windmill(Qube& qube, uint16_t speed, Axis axis) :
		Effect(qube, speed) {
	this->axis = axis;
	this->counter = 0;
}
void Windmill::init() {
	DEBUG("windmill effect activated");
	qube.fill(EMPTY);
	this->counter = (qube.size) - 1 * 2;
	qube = true;
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

Blink::Blink(Qube& qube, uint16_t speed) :
		Effect(qube, speed) {
	this->state = false;
}
void Blink::init() {
	DEBUG("blink effect activated");
	qube.fill(FULL);
}
void Blink::update() {
	if (elapsed > speed) {
		qube = state;
		state = !state;
		elapsed = 0;
	}
}

RandomBlinker::RandomBlinker(Qube& qube, uint16_t speed, uint8_t frequency) :
		Effect(qube, speed) {
	this->frequency = 1000 / frequency;
}
void RandomBlinker::init() {
	DEBUG("random blinker effect activated");
	qube.fill(EMPTY);
	qube = true;
}
void RandomBlinker::update() {
	if ((elapsed / frequency) % 2) {
		qube = !qube;
		if (elapsed > speed) {
			Coord coord(rand() % qube.size, rand() % qube.size, rand() % qube.size);
			qube.voxel(coord, FLIP);
			qube.shift(Z, -1);
			elapsed = 0;
		}
	}
}

Carousel::Carousel(Qube& qube, uint16_t duration, Effect** effects, uint8_t size) :
		Effect(qube, duration) {
	this->effects = effects;
	this->size = size;
	this->current = 0;
	this->speed = duration;
}
void Carousel::init() {
	current = 0;
	DEBUG("carousel effect activated at index %u", current);
	effects[0]->init();
}
void Carousel::update() {
	if (elapsed / 1000 == speed) {
		if (++current == size) {
			current = 0;
		}
		DEBUG("moving to effect index %u", current);
		effects[current]->init();
		elapsed = 0;
	}
	effects[current]->update();
}

void RandomCarousel::init() {
	current = rand() % size;
	DEBUG("random carousel effect activated at index %u", current);
	effects[current]->init();
}
void RandomCarousel::update() {
	if (elapsed / 1000 == speed) {
		current = rand() % size;
		DEBUG("moving to effect index %u", current);
		effects[current]->init();
		elapsed = 0;
	}
	effects[current]->update();
}
