/*
 Video.h - Qube library
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

#ifndef QUBE_VIDEO_H_
#define QUBE_VIDEO_H_

#include "Effects.h"

class Video : public Effect {
	private:

	public:
		Video(Qube& qube, uint16_t speed, byte* frames);
		Video(Qube& qube, uint16_t speed, Stream* stream);
		void init();
		void update();
		bool completed();
};

#endif /* QUBE_VIDEO_H_ */
