include("makefiles/shared.cmake")

add_definitions(-DCOMPILER_MSVC -DCOMPILER_MSVC64 -DWIN32 -D_WIN32 -D_WINDOWS)
add_definitions(-D_ALLOW_KEYWORD_MACROS -D__STDC_LIMIT_MACROS -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -DSPDLOG_COMPILED_LIB)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

SET(
        COUNTER_STRIKE_SHARP_LINK_LIBRARIES
        ${SOURCESDK_LIB}/public/win64/tier0.lib
        ${SOURCESDK_LIB}/public/win64/tier1.lib
        ${SOURCESDK_LIB}/public/win64/interfaces.lib
        ${SOURCESDK_LIB}/public/win64/mathlib.lib
        dynload_s
        dyncall_s
        spdlog
        distorm
        funchook-static
)
