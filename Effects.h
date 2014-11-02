/*
 Effects.h - Qube library
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

#ifndef QUBE_EFFECTS_H_
#define QUBE_EFFECTS_H_

#include "Qube.h"
#include "Elapsed.h"

void test(Qube& qube, uint16_t wait = 500);

class Effect {
	protected:
		Qube& qube;
		uint16_t speed;
		Elapsed elapsed;
	public:
		Effect(Qube& qube, uint16_t speed);
		virtual void init() = 0;
		virtual void update() = 0;
		virtual ~Effect() { };

		// Sets or clear all voxels in random order
		void random_filler(Qube& qube, uint16_t speed, bool state);

		// Sets a plane and sends it back and forth once
		void plane_boing(Qube& qube, uint16_t speed, Axis axis);

		// Have a maximum amount of drops flowing from top toward the bottom
		void raindrop(Qube& qube, uint16_t speed, uint16_t drops = 1);

		void windmill(Qube& qube, uint16_t speed, uint16_t rotations = 1);

		void blink(Qube& qube, uint16_t speed, uint16_t iterations);

		void test(Qube& qube, uint16_t speed = 500);

		void stepway(Qube& qube, uint16_t speed, uint16_t iterations);

};

class RandomFill : public Effect {
	private:
		bool state;
		uint16_t counter;
	public:
		RandomFill(Qube& qube, uint16_t speed, bool reverse = false);
		void init();
		void update();
};

class PlaneBounce : public Effect {
	private:
		Axis axis;
		uint16_t counter;
	public:
		PlaneBounce(Qube& qube, uint16_t speed, Axis axis);
		void init();
		void update();
};

class Rain : public Effect {
	private:
		uint16_t drops;
	public:
		Rain(Qube& qube, uint16_t speed, uint16_t drops) : Effect(qube, speed) { this->drops = drops; }
		void init();
		void update();
};

class Windmill : public Effect {
	private:
		Axis axis;
		uint16_t counter;
	public:
		Windmill(Qube& qube, uint16_t speed, Axis axis);
		void init();
		void update();
};

class Blink : public Effect {
	private:
		bool state;
	public:
		Blink(Qube& qube, uint16_t speed);
		void init();
		void update();
};

#endif /* QUBE_EFFECTS_H_ */
