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

#pragma pack(push, 1)

struct CodeData {
    int size;
    int width;
    int height;
    uint8_t data[1];
};

#pragma pack(pop)

CodeData* __stdcall CodeBuild(const char* name, int width, int height, int margin, int eccLevel, const char* input) {
    auto format = BarcodeFormatFromString(name);
    if (format != BarcodeFormat::None) {
        auto writer = MultiFormatWriter(format).setMargin(margin).setEncoding(CharacterSet::UTF8).setEccLevel(eccLevel);

        BitMatrix matrix = writer.encode(input, width, height);
        auto bitmap = ToMatrix<uint8_t>(matrix);
        if (bitmap.size() > 0) {
            int size = (sizeof(CodeData) - 1) + bitmap.size();
            CodeData* result = reinterpret_cast<CodeData*>(new uint8_t[size]);
            if (result != NULL) {
                result->size = size;
                result->width = bitmap.width();
                result->height = bitmap.height();
                memcpy(result->data, bitmap.data(), bitmap.size());
            }

            return result;
        }
    }

    return NULL;
}

void __stdcall CodeFree(const CodeData* output) {
    if (output != NULL) {
        delete[] output;
    }
}

} // extern "C"
