#pragma once

#include <cstring>
#include "log.h"
#include "core/gameconfig.h"


#ifdef _WIN32
#include <Psapi.h>
#include "utils/plat_win.cpp"
#else
#include "utils/plat_unix.cpp"
#endif
