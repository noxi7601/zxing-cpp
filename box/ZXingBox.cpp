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

using namespace ZXing;

extern "C" {

int __stdcall QRCodeBuild(int width, int height, int margin, int eccLevel, const char* input) {
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

	return 0;
}

}
