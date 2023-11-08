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

namespace counterstrikesharp {
static void* FindSignature(const char* modulePath, const char* signature)
{
    auto module = dlmount(modulePath);

    if (module == nullptr) {
        CSSHARP_CORE_ERROR("Could not find {}", modulePath);
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
        CSSHARP_CORE_INFO("Failed to find signature \'{}\' FROM \'{}\'", signature, modulePath);
        return nullptr;
    }

    return returnAddr;
}
} // namespace counterstrikesharp