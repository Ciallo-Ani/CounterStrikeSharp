#include <cstring>
#include "log.h"
#include "core/gameconfig.h"


#ifdef _WIN32
#include <Psapi.h>
#include "utils/plat_win.cpp"
    #define ROOTBIN "/bin/win64/"
    #define GAMEBIN "/csgo/bin/win64/"
    #define MODULE_PREFIX ""
    #define MODULE_EXT ".dll"
#else
#include "utils/plat_unix.cpp"
    #define ROOTBIN "/bin/linuxsteamrt64/"
    #define GAMEBIN "/csgo/bin/linuxsteamrt64/"
    #define MODULE_PREFIX "lib"
    #define MODULE_EXT ".so"
#endif

namespace counterstrikesharp {
byte* ConvertToByteArray(const char* str, size_t* outLength)
{
    size_t len = strlen(str) / 4; // Every byte is represented as \xHH
    byte* result = (byte*)malloc(len);

    for (size_t i = 0, j = 0; i < len; ++i, j += 4) {
        sscanf(str + j, "\\x%2hhx", &result[i]);
    }

    *outLength = len;
    return result;
}

void* FindSignature(const char* moduleName, const char* signature)
{
    char szModule[MAX_PATH];
    V_snprintf(szModule, MAX_PATH, "%s%s%s%s%s", Plat_GetGameDirectory(), GAMEBIN, MODULE_PREFIX,
               moduleName, MODULE_EXT);

    auto module = dlmount(szModule);

    if (module == nullptr) {
        CSSHARP_CORE_ERROR("Could not find {}", szModule);
        return nullptr;
    }

    void* moduleBase;
    size_t moduleSize;

#ifdef _WIN32
    MODULEINFO m_hModuleInfo;
    GetModuleInformation(GetCurrentProcess(), module, &m_hModuleInfo, sizeof(m_hModuleInfo));

    moduleBase = (void*)m_hModuleInfo.lpBaseOfDll;
    moduleSize = m_hModuleInfo.SizeOfImage;
#else
    if (GetModuleInformation(module, &moduleBase, &moduleSize) != 0) {
        CSSHARP_CORE_INFO("GetModuleInformation == nullptr");
        return nullptr;
    }
#endif
    CSSHARP_CORE_INFO("Initialized module {} base: {} | size: {}\n", moduleName, (void*)moduleBase,
                      moduleSize);

    unsigned char* pMemory;
    void* returnAddr = nullptr;

    size_t iSigLength;
    byte* pData = CGameConfig::HexToByte(signature, iSigLength);

    pMemory = (byte*)moduleBase;

    for (size_t i = 0; i < moduleSize; i++) {
        size_t Matches = 0;
        while (*(pMemory + i + Matches) == pData[Matches] || pData[Matches] == '\x2A') {
            Matches++;
            if (Matches == iSigLength)
                returnAddr = (void*)(pMemory + i);
        }
    }

    if (returnAddr == nullptr) {
        CSSHARP_CORE_INFO("returnAddr == nullptr");
        return nullptr;
    }

    return returnAddr;
}
} // namespace counterstrikesharp