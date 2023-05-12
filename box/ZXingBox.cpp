#include "BarcodeFormat.h"
#include "BitMatrix.h"
#include "BitMatrixIO.h"
#include "CharacterSet.h"
#include "MultiFormatWriter.h"
#include "ZXVersion.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <streambuf>

using namespace ZXing;

extern "C" {

uint8_t* __stdcall QRCodeBuild(int width, int height, int margin, int eccLevel, const char* input) {
	auto writer = MultiFormatWriter(BarcodeFormat::QRCode).setMargin(margin).setEncoding(CharacterSet::UTF8).setEccLevel(eccLevel);

	BitMatrix matrix = writer.encode(input, width, height);
	auto bitmap = ToMatrix<uint8_t>(matrix);
	{
		std::ofstream out("result.dat");
		if (out.is_open()) {
			int width_ = bitmap.width();
			int height_ = bitmap.height();
			out.write(reinterpret_cast<const char*>(&width_), sizeof(width_));
			out.write(reinterpret_cast<const char*>(&height_), sizeof(height_));
			out.write(reinterpret_cast<const char*>(bitmap.data()), bitmap.size());
			out.close();
		}
	}
	if (bitmap.size() > 0) {
		int width_ = bitmap.width();
		int height_ = bitmap.height();
		int size = sizeof(size) + sizeof(width_) + sizeof(height_) + bitmap.size();
		uint8_t* result = new uint8_t[size];
		if (result != NULL) {
			memcpy(&result[0], &size, sizeof(size));
			memcpy(&result[sizeof(size)], &width_, sizeof(width_));
			memcpy(&result[sizeof(size) + sizeof(width_)], &height_, sizeof(height_));
			memcpy(&result[sizeof(size) + sizeof(width_) + sizeof(height_)], bitmap.data(), bitmap.size());
		}

		return result;
	} else {
		return NULL;
	}
}

void __stdcall QRCodeFree(const char* output) {
	if (output != NULL) {
		delete[] output;
	}
}

}
