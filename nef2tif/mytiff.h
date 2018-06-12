/*
* nef2tif _ raw image format NEF (Nikon Electronic Format) to TIFF converter
* Copyright (C) 2018  Alexander Löwen <alexander.loewen@web.de>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef MYTIFF_H
#define MYTIFF_H

class MyTiff
{
public:
	static int save(const char* filename,
		unsigned long _width,
		unsigned long _height,
		const char *buffer,
		unsigned long _bitpersample);
};

#endif // MYTIFF_H
