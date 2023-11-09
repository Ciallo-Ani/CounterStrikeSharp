/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023 Source2ZE
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "dbg.h"
#include "interface.h"
#include "strtools.h"
#include "metamod_oslink.h"
#include "log.h"
#include "core/utils.h"
#include "core/gameconfig.h"

#ifdef _WIN32
    #include <Psapi.h>
#endif
namespace counterstrikesharp {
namespace modules {

class CModule {
public:
    CModule(const char *path, const char *module)
        : m_pszModule(module),
          m_pszPath(path) {
        char szModule[MAX_PATH];

        V_snprintf(szModule, MAX_PATH, "%s%s%s%s%s", Plat_GetGameDirectory(), path, MODULE_PREFIX,
                   m_pszModule, MODULE_EXT);

        m_hModule = dlmount(szModule);

        if (!m_hModule) Error("Could not find %s\n", szModule);

#ifdef _WIN32
        MODULEINFO m_hModuleInfo;
        GetModuleInformation(GetCurrentProcess(), m_hModule, &m_hModuleInfo, sizeof(m_hModuleInfo));

        m_base = (void *)m_hModuleInfo.lpBaseOfDll;
        m_size = m_hModuleInfo.SizeOfImage;
#else
        if (int e = GetModuleInformation(m_hModule, &m_base, &m_size))
            Error("Failed to get module info for %s, error %d\n", szModule, e);
#endif
    }

    void *FindSignature(const char *signature) {
        if (strlen(signature) == 0) {
            return nullptr;
        }

        size_t iSigLength;
        byte* pData = CGameConfig::HexToByte(signature, iSigLength);

        return this->FindSignature(pData);
    }

    void *FindSignature(const byte *pData) {
        unsigned char *pMemory;
        void *return_addr = nullptr;

        size_t iSigLength = V_strlen((const char *)pData);

        pMemory = (byte *)m_base;

        for (size_t i = 0; i < m_size; i++) {
            size_t Matches = 0;
            while (*(pMemory + i + Matches) == pData[Matches] || pData[Matches] == '\x2A') {
                Matches++;
                if (Matches == iSigLength) {
                    return_addr = (void*)(pMemory + i);
                }
            }
        }

        return return_addr;
    }

    void *FindSignature(const byte* pData, size_t iSigLength)
    {
        unsigned char* pMemory;
        void* return_addr = nullptr;

        pMemory = (byte*)m_base;

        for (size_t i = 0; i < m_size; i++) {
            size_t Matches = 0;
            while (*(pMemory + i + Matches) == pData[Matches] || pData[Matches] == '\x2A') {
                Matches++;
                if (Matches == iSigLength) {
                    return_addr = (void*)(pMemory + i);
                }
            }
        }

        return return_addr;
    }

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

    void *FindInterface(const char *name) {
        CreateInterfaceFn fn = (CreateInterfaceFn)dlsym(m_hModule, "CreateInterface");

        if (!fn) Error("Could not find CreateInterface in %s\n", m_pszModule);

        void *pInterface = fn(name, nullptr);

        if (!pInterface) Error("Could not find %s in %s\n", name, m_pszModule);

        return pInterface;
    }

    const char *m_pszModule;
    const char *m_pszPath;
    HINSTANCE m_hModule;
    void *m_base;
    size_t m_size;
};
}  // namespace modules
}  // namespace counterstrikesharp