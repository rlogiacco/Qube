/*
 Video.cpp - Qube library
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

#include "Video.h"

Video::Video(Qube& qube, uint16_t speed, byte* frames) :
		Effect(qube, speed) {
}
Video::Video(Qube& qube, uint16_t speed, Stream* stream) :
		Effect(qube, speed) {
}
void Video::init() {
}
void Video::update() {
}
bool Video::completed() {
}
