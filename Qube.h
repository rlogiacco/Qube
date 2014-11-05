/*
 Qube.h - Qube library
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

#ifndef QUBE_H_
#define QUBE_H_

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

enum Axis {
	X, Y, Z
};
enum Fill {
	EMPTY, FULL, WIREFRAME, HOLLOW
};
enum Bit {
	OFF, ON, FLIP
};
enum Overflow {
	WRAP, CLEAR
};

struct Coord {
		uint8_t x;
		uint8_t y;
		uint8_t z;
		Coord(uint8_t x, uint8_t y, uint8_t z) :
				x(x), y(y), z(z) {
		}
};

// divide and round up
#define _divrup(x,y) (x / y + (((x < 0) ^ (y > 0)) && (x % y)))

class Qube {
	private:
		volatile uint8_t** cube;
		volatile bool enabled;

		template<typename T, size_t n> char (& _size(const T (&)[n]) )[n];

	public:

		/**
		 * Constructor accepts size and an optional status, defaulting to enabled
		 */
		Qube(uint8_t size, bool enabled = true);

		volatile uint8_t size;

		/**
		 * Returns the status of a specific voxel
		 */
		bool voxel(Coord coord);

		/**
		 * Sets the status of a specific voxel
		 */
		void voxel(Coord coord, Bit bit);
		void voxel(Coord coord, bool on);

		/**
		 * Sets the fill pattern for an entire plane/layer
		 */
		void plane(Axis axis, uint8_t index, Fill fill);
		void plane(Axis axis, uint8_t index, uint8_t fill);

		/**
		 * Draws a line from start coordinates to end
		 */
		void line(Coord start, Coord end);

		/**
		 * Draws a box from start coordinates with specified lenght, width and height
		 */
		void box(Coord start, uint8_t l, uint8_t w, uint8_t h, Fill fill);

		/**
		 * Fills the entire cube with the supplied pattern
		 */
		void fill(uint8_t pattern);
		void fill(Fill fill);

		/**
		 * Shifts all contents by one or more steps along an axis.
		 * Negative steps value invert shifting direction and an optional overflow method can be specified
		 */
		void shift(Axis axis, int8_t steps, Overflow overflow = WRAP);

		/**
		 * Prints on serial a string representation of the contents
		 */
		void print();

		/**
		 * Runs a test procedure: blink three times voxel at (0,0,0), then turn on each voxel
		 * by columns, rows and layers and then turns them all off in reverse order.
		 */
		void test(uint16_t speed = 250);

		/**
		 * Use array notation for direct layer access
		 */
		volatile uint8_t* operator[] (uint8_t layer) { return cube[layer]; }

		/**
		 * Cast to bool returns the status: true for enabled, false for disabled
		 */
		operator volatile bool() { return enabled; }

		/**
		 * Assign a bool to set the status: true for enabled, false for disabled
		 */
		void operator= (bool enable) { enabled = enable; }
		~Qube();
	};

#endif /* QUBE_H_ */
