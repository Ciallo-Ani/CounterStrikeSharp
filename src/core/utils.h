#pragma once

#include <public/eiface.h>
#include <string>

#include "core/globals.h"

#ifdef _WIN32
    #define ROOTBIN "/bin/win64/"
    #define GAMEBIN "/csgo/bin/win64/"
    #define MODULE_PREFIX ""
    #define MODULE_EXT ".dll"
#else
    #define ROOTBIN "/bin/linuxsteamrt64/"
    #define GAMEBIN "/csgo/bin/linuxsteamrt64/"
    #define MODULE_PREFIX "lib"
    #define MODULE_EXT ".so"
#endif

namespace counterstrikesharp {
namespace utils {

static std::string gameDirectory;

inline std::string GameDirectory() {
    if (gameDirectory.empty()) {
        CBufferStringGrowable<255> gamePath;
        globals::engine->GetGameDir(gamePath);
        gameDirectory = std::string(gamePath.Get());
    }

    return gameDirectory;
}

inline std::string PluginDirectory() { return GameDirectory() + "/addons/counterstrikesharp"; }
inline std::string GamedataDirectory() { return GameDirectory() + "/addons/counterstrikesharp/gamedata"; }
inline std::string ConfigDirectory() { return GameDirectory() + "/addons/config"; }
}  // namespace utils
}  // namespace counterstrikesharp
