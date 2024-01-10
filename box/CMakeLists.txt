include(../zxing.cmake)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library (ZXingBox ZXingBox.cpp ZXingBox.def)

target_compile_options (ZXingBox PUBLIC /Zc:__cplusplus /utf-8)

target_link_libraries (ZXingBox ZXing::ZXing)
