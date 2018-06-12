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
