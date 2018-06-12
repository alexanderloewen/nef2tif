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

#include <Windows.h>
#include "Nkfl_Interface.h"
#include "mytiff.h"
#include <iostream>

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cerr << "usage: " << argv[0] << " <input NEF file> <output tif file>" << std::endl;
		return 0;
	}

	// vars
	unsigned long error;

	// open library
	NkflLibraryParam  libraryParam = { 0 };
	libraryParam.pNkflPtr = NULL;
	libraryParam.ulSize = sizeof(NkflLibraryParam);
	libraryParam.ulVersion = 0x01000000;
	libraryParam.ulVMMemorySize = 0;
	libraryParam.VMFileInfo[0] = (unsigned char)0;
	libraryParam.DefProfPath[0] = (unsigned char)0;

#ifdef _DEBUG
	std::cerr << "opening library ... " << std::endl;
#endif

	error = Nkfl_Entry(kNkfl_Cmd_OpenLibrary, &libraryParam);
	if ((error != kNkfl_Code_None) && (error < 0x100)) {
		std::cerr << "OpenLibrary failed with code " << error << std::endl;
		return 0;
	}

	// open session
	NkflSessionParam sessionParam = { 0 };

	sessionParam.ulSize = sizeof(sessionParam);
	sessionParam.ulType = kNkfl_Source_FileName;
	sessionParam.pFileInfo = argv[1];
	sessionParam.ulFileSize = 0;

#ifdef _DEBUG
	std::cerr << "opening session ... " << std::endl;
#endif

	error = Nkfl_Entry(kNkfl_Cmd_OpenSession, &sessionParam);
	if ((error != kNkfl_Code_None) && (error < 0x100)) {
		std::cerr << "OpenSession failed with code " << error << std::endl;
	}
	else {
		// set raw quality
		NkflRawDevelopmentParam	NkflRawDevelopment = { 0 };
		NkflRawDevelopment.ulSize = sizeof(NkflRawDevelopmentParam);
		NkflRawDevelopment.ulSessionID = sessionParam.ulSessionID;

		NkflRawDevelopment_RawQuality rawQuality = { 0 };
		rawQuality.ulSize = sizeof(NkflRawDevelopment_RawQuality);
		rawQuality.ulQuality = kNkfl_RawQuality_High;

		NkflRawDevelopment.ulRawDevelopment = kNkfl_RawDevelopment_RawQuality;
		NkflRawDevelopment.pData = &rawQuality;

		error =Nkfl_Entry(kNkfl_Cmd_RawDevelopment, &NkflRawDevelopment);
		if ((error != kNkfl_Code_None) && (error < 0x100)) {
			std::cerr << "RawDevelopment failed with code " << error << std::endl;
		}
		else {

			// get image info
			NkflImageInfoParam	imgInfoParam = { 0 };
			imgInfoParam.ulSize = sizeof(NkflImageInfoParam);
			imgInfoParam.ulSessionID = sessionParam.ulSessionID;

#ifdef _DEBUG
			std::cerr << "getting image info ... " << std::endl;
#endif

			error = Nkfl_Entry(kNkfl_Cmd_GetOriginalInfo, &imgInfoParam);
			if ((error != kNkfl_Code_None) && (error < 0x100)) {
				std::cerr << "GetImageInfo failed with code " << error << std::endl;
			}
			else {

#ifdef _DEBUG
				// output of image info
				std::cerr << "Resolution of the image to be obtained (ppi): ";
				std::cerr << imgInfoParam.dbResolution;
				std::cerr << std::endl;

				std::cerr << "Byte length of the image to be obtained: ";
				std::cerr << imgInfoParam.ulByteDepth;
				std::cerr << std::endl;

				std::cerr << "Color of the image to be obtained: ";
				std::cerr << imgInfoParam.ulColor;
				std::cerr << std::endl;

				std::cerr << "Height of the image to be obtained (in units of pixel): ";
				std::cerr << imgInfoParam.ulHeight;
				std::cerr << std::endl;

				std::cerr << "Image ID: ";
				std::cerr << imgInfoParam.ulImageID;
				std::cerr << std::endl;

				std::cerr << "Information of the image flip and rotation: ";
				std::cerr << imgInfoParam.ulOrientation;
				std::cerr << std::endl;

				std::cerr << "Width of the image to be obtained (in units of pixel): ";
				std::cerr << imgInfoParam.ulWidth;
				std::cerr << std::endl;
#endif

				// get image data
				unsigned long imageDataLength = imgInfoParam.ulWidth * imgInfoParam.ulHeight * 4 * imgInfoParam.ulByteDepth;
				char *imageData = new char[imageDataLength];
				RECT rectArea = { 0,0,(long)imgInfoParam.ulWidth, (long)imgInfoParam.ulHeight };

				NkflImageParam	imageParam = { 0 };

				imageParam.ulSize = sizeof(NkflImageParam);
				imageParam.ulSessionID = sessionParam.ulSessionID;
				imageParam.rectArea = rectArea;
				imageParam.ulDataSize = imageDataLength;
				imageParam.pData = imageData;
				imageParam.pFunc = NULL;
				imageParam.pProgressParam = NULL;

				std::cerr << "reading '" << argv[1] << "' ... ";
				std::cerr.flush();

				error = Nkfl_Entry(kNkfl_Cmd_GetImageData, &imageParam);
				if ((error != kNkfl_Code_None) && (error < 0x100)) {
					std::cerr << "GetImageData failed with code " << error << std::endl;
				}
				else {
					std::cerr << "done!" << std::endl;
					std::cerr << "bytes read: " << imageParam.ulDataSize << std::endl;

					// saving image data as tiff
					MyTiff::save(argv[2], imgInfoParam.ulWidth, imgInfoParam.ulHeight, imageData, 8 * imgInfoParam.ulByteDepth);
				}
				delete[] imageData;
			}
		}
		// close session
		error = Nkfl_Entry(kNkfl_Cmd_CloseSession, &sessionParam);
		if ((error != kNkfl_Code_None) && (error < 0x100)) {
			std::cerr << "CloseSession failed with code " << error << std::endl;
			return 0;
		}
	}

	// close library
	error = Nkfl_Entry(kNkfl_Cmd_CloseLibrary, NULL);
	if ((error != kNkfl_Code_None) && (error < 0x100)) {
		std::cerr << "CloseLibrary failed with code " << error << std::endl;
		return 0;
	}
	return 0;
}