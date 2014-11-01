/*
 * Qube.h
 *
 *  Created on: 26/ott/2014
 *      Author: rlogiacco
 */

#ifndef QUBE_H_
#define QUBE_H_

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

enum Axis { X, Y, Z };
enum Fill { EMPTY, FULL, WIREFRAME, HOLLOW};
enum Bit { OFF, ON, FLIP };
struct Coord {
		uint8_t x;
		uint8_t y;
		uint8_t z;
		Coord(uint8_t x, uint8_t y, uint8_t z) : x(x), y(y), z(z) { }
};

// divide and round up
#define _divrup(x,y) (x / y + (((x < 0) ^ (y > 0)) && (x % y)))

class Qube {
	private:
		volatile uint8_t** cube;
		volatile uint8_t current_layer;

		template<typename T, size_t n> char (& _size(const T (&)[n]) )[n];
	public:
		Qube(uint8_t size);
		volatile uint8_t size;
		bool voxel(Coord coord);
		void voxel(Coord coord, Bit bit);
		void voxel(Coord coord, bool on);
		void plane(Axis axis, uint8_t index, Fill fill);
		void plane(Axis axis, uint8_t index, uint8_t fill);
		void line(Coord start, Coord end);
		void box(uint8_t x, uint8_t y, uint8_t z, uint8_t l, uint8_t h, uint8_t w, Fill fill);
		void fill(uint8_t pattern);
		void fill(Fill fill);
		void shift(Axis axis, int8_t steps);
		void print();
		volatile uint8_t* operator[] (uint8_t level) { return cube[level]; }
		~Qube();
};



#endif /* QUBE_H_ */
