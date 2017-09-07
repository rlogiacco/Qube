/*
 Fireworks.h - Qube library
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

#ifndef QUBE_FIREWORKS_EFFECT_H_
#define QUBE_FIREWORKS_EFFECT_H_

#include "Effects.h"

class Fireworks: public Effect {
	private:
		class Firework;
		Elapsed delay;
		Box& explode;
		Fireworks::Firework* fireworks;
		uint8_t numParticles;
		uint8_t maxFireworks;
	public:
		Fireworks(Qube& qube, uint16_t speed, uint16_t delay, uint8_t margin = 1, uint8_t numParticles = 5, uint8_t maxFireworks = 3);
		Fireworks(Qube& qube, uint16_t speed, uint16_t delay, Box& explode, uint8_t numParticles = 5, uint8_t maxFireworks = 3);
		void init();
		void fire();
		void update();
};

#endif /* QUBE_FIREWORK_EFFECT_H_ */
