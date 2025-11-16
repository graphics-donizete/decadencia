add_library(libskia INTERFACE)

target_include_directories(
    libskia
    INTERFACE
    ${CMAKE_SOURCE_DIR}/external/
    ${CMAKE_SOURCE_DIR}/external/skia
    ${CMAKE_SOURCE_DIR}/external/skia/third_party/externals/angle2/include
)

target_link_libraries(
    libskia
    INTERFACE
    ${CMAKE_SOURCE_DIR}/external/skia/out/Static/libskia.a
)

if(${APPLE})
    target_link_libraries(
        libskia
        INTERFACE
        "-framework CoreServices"
        "-framework CoreFoundation"
        "-framework CoreGraphics"
        "-framework CoreText"
    )
endif()

if(${LINUX})
    find_package(OpenGL REQUIRED)
    target_link_libraries(
        libskia
        INTERFACE
        ${OPENGL_LIBRARIES}
    )
endif()