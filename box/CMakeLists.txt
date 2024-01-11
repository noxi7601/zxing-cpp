include(../zxing.cmake)

if (MSVC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

add_library (ZXingBox SHARED ZXingBox.cpp)

target_link_libraries (ZXingBox ZXing::ZXing)

if (MSVC)
    target_sources (ZXingBox PRIVATE ZXingBox.def)
    target_compile_options (ZXingBox PUBLIC /Zc:__cplusplus /utf-8)
endif ()
