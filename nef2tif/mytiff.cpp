#include "mytiff.h"
#include <tiffio.h>
#include <iostream>

int MyTiff::save(const char *filename, unsigned long _width, unsigned long _height, const char *buffer, unsigned long _bitpersample)
{
	TIFF *image;

	std::cerr << "saving tiff '" << filename << "' (" << _width << "x" << _height << ", " << _bitpersample << ") ...";
	std::cerr.flush();

	// Open the TIFF file
	if ((image = TIFFOpen(filename, "w")) == NULL) {
		std::cerr << " failed!\nTIFFOpen failed" << std::endl;
		return -1;
	}

	// We need to set some values for basic tags before we can add any data
	TIFFSetField(image, TIFFTAG_IMAGEWIDTH, _width);
	TIFFSetField(image, TIFFTAG_IMAGELENGTH, _height);
	TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, _bitpersample);
	TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, _height);

	TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
	TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	TIFFSetField(image, TIFFTAG_XRESOLUTION, 72.0);
	TIFFSetField(image, TIFFTAG_YRESOLUTION, 72.0);
	TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
	TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

	// Write the information to the file
	TIFFWriteEncodedStrip(image, (tstrip_t)0 /* raw strip number */, (tdata_t)buffer, _width*_height * 3 * (_bitpersample / 8));

	// Close the file
	TIFFClose(image);
	std::cerr << " done!" << std::endl;
	return 0;
}
