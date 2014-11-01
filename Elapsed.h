/* Elapsed time types - for easy-to-use measurements of elapsed time
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2011 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef QUBE_ELAPSED_H_
#define QUBE_ELAPSED_H_
#ifdef __cplusplus

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Elapsed {
	private:
		unsigned long ms;
	public:
		Elapsed(void) 					{ ms = millis(); }
		Elapsed(unsigned long val) 		{ ms = millis() - val; }
		Elapsed(const Elapsed &orig) 	{ ms = orig.ms; }

		operator unsigned long () const 				{ return millis() - ms; }
		Elapsed& operator = (const Elapsed &rhs) 		{ ms = rhs.ms; return *this; }
		Elapsed& operator = (unsigned long val) 		{ ms = millis() - val; return *this; }
		Elapsed& operator -= (unsigned long val)      	{ ms += val ; return *this; }
		Elapsed& operator += (unsigned long val)      	{ ms -= val ; return *this; }
		Elapsed  operator - (int val) const           	{ Elapsed r(*this); r.ms += val; return r; }
		Elapsed  operator - (unsigned int val) const  	{ Elapsed r(*this); r.ms += val; return r; }
		Elapsed  operator - (long val) const          	{ Elapsed r(*this); r.ms += val; return r; }
		Elapsed  operator - (unsigned long val) const 	{ Elapsed r(*this); r.ms += val; return r; }
		Elapsed  operator + (int val) const           	{ Elapsed r(*this); r.ms -= val; return r; }
		Elapsed  operator + (unsigned int val) const  	{ Elapsed r(*this); r.ms -= val; return r; }
		Elapsed  operator + (long val) const          	{ Elapsed r(*this); r.ms -= val; return r; }
		Elapsed  operator + (unsigned long val) const 	{ Elapsed r(*this); r.ms -= val; return r; }
};

#endif // __cplusplus
#endif // QUBE_ELAPSED_H_
