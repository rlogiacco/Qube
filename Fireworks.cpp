/*
 Fireworks.cpp - Qube library
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

#include "Fireworks.h"
#include <FormattingSerialDebug.h>

Fireworks::Fireworks(Qube& qube, uint16_t speed, uint16_t delay, uint8_t margin, uint8_t numParticles, uint8_t maxFireworks) :
		Effect(qube, speed), explode(explode) {
	this->delay = delay;
	this->explode = *(new Box(margin, margin, margin, qube.size - 1 - margin, qube.size - 1 - margin, qube.size - 1 - margin));
	this->numParticles = numParticles;
}
Fireworks::Fireworks(Qube& qube, uint16_t speed, uint16_t delay, Box& explode, uint8_t numParticles, uint8_t maxFireworks) :
		Effect(qube, speed), explode(explode) {
	this->delay = delay;
	this->numParticles = numParticles;
}

class Fireworks::Firework {
	private:
		enum Status {
			FIRED, EXPLODED, FINISHED
		};

		Coord& origin, explode;
		uint8_t speed;
		Vect* particles;
		uint8_t numParticles;
		Firework::Status status;
	public:
		Firework(Coord& origin, Coord& explode, uint8_t speed, uint8_t numParticles) :
				origin(origin), explode(explode) {
			this->speed = speed;
			this->status = FIRED;
			this->numParticles = numParticles;
			this->particles = new Vect[numParticles];
			for (int i = 0; i < numParticles - 1; i++) {
				particles[i].x = explode.x;
				particles[i].y = explode.y;
				particles[i].z = explode.z;
				// fixme set direction
			}
		}
		void update() {
			switch (status) {
				case FIRED:
					// move from origin to explode point
					// if position == explode point
					//    update status to EXPLODED
					break;
				case EXPLODED:
					for (int i = 0; i < numParticles - 1; i++) {
						// move particle along a parabola
					}
					// if all particles out of boundaires
					//    update status to FINISHED
					break;
				case FINISHED:
					// do nothing
					break;
			}
		}
};
void Fireworks::init() {

}
void Fireworks::update() {
	for (int i = 0; i < maxFireworks; i++) {
		fireworks[i].update();
	}
}
void Fireworks::fire() {
	// create new firework
}
