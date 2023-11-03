include("makefiles/shared.cmake")

#SET(_ITERATOR_DEBUG_LEVEL 2)
#add_definitions(-D_LINUX -DPOSIX -DLINUX -DGNUC -DCOMPILER_GCC -DPLATFORM_64BITS)
add_definitions(-DCOMPILER_MSVC64 -DWIN32 -D_WIN32 -D_WINDOWS -D_ALLOW_KEYWORD_MACROS -D__STDC_LIMIT_MACROS -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -DSPDLOG_COMPILED_LIB)
# Set(CMAKE_CXX_FLAGS_RELEASE "/D_NDEBUG /MD /wd4005 /MP")

#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp")
#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dstrnicmp=strncasecmp -D_snprintf=snprintf")
#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp")

# Others
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse -msse -fno-strict-aliasing")
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fno-threadsafe-statics -v -fvisibility=default")

SET(
        COUNTER_STRIKE_SHARP_LINK_LIBRARIES
        ${SOURCESDK_LIB}/public/win64/tier0.lib
        ${SOURCESDK_LIB}/public/win64/tier1.lib
        ${SOURCESDK_LIB}/public/win64/interfaces.lib
        ${SOURCESDK_LIB}/public/win64/mathlib.lib
        spdlog
        dynload_s
        dyncall_s
        distorm
        funchook-static
)
