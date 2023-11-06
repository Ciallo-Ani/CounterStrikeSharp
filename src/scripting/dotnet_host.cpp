#include "scripting/dotnet_host.h"

#include <dotnet/coreclr_delegates.h>
#include <dotnet/hostfxr.h>

#include <codecvt>
#include <locale>

#ifdef WIN32
    #include <Windows.h>
    #include <direct.h>

    #define STR(s) L##s
    #define CH(c) L##c
    #define DIR_SEPARATOR L'\\'

#else
    #include <dlfcn.h>
#endif

#include <cassert>
#include <iostream>
#include <sstream>
#include <metamod_oslink.h>

#include "core/log.h"
#include "core/utils.h"

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

namespace {
hostfxr_initialize_for_runtime_config_fn init_fptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr;
hostfxr_close_fn close_fptr;
hostfxr_handle cxt;

bool load_hostfxr();
load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t *assembly);
}  // namespace

namespace {

// <SnippetLoadHostFxr>
// Using the nethost library, discover the location of hostfxr and get exports
bool load_hostfxr() {
    std::string baseDir = counterstrikesharp::utils::PluginDirectory();

#ifdef _WIN32
    std::string buffer = std::string(baseDir + "/dotnet/host/fxr/7.0.11/hostfxr.dll");
#else
    std::string buffer = std::string(baseDir + "/dotnet/host/fxr/7.0.11/libhostfxr.so");
#endif

    CSSHARP_CORE_INFO("Loading hostfxr from {0}", buffer.c_str());

    // Load hostfxr and get desired exports
    auto lib = dlmount(buffer.c_str());

    CSSHARP_CORE_INFO("lib -> {}", (void*)lib);

    if (lib == nullptr) {
        CSSHARP_CORE_ERROR("Failed to load {}", buffer);
        return false;
    }
    
    init_fptr = (hostfxr_initialize_for_runtime_config_fn)dlsym(lib, "hostfxr_initialize_for_runtime_config");
    get_delegate_fptr = (hostfxr_get_runtime_delegate_fn)dlsym(lib, "hostfxr_get_runtime_delegate");
    close_fptr = (hostfxr_close_fn)dlsym(lib, "hostfxr_close");

    return (init_fptr && get_delegate_fptr && close_fptr);
}
// </SnippetLoadHostFxr>

// <SnippetInitialize>
// Load and initialize .NET Core and get desired function pointer for scenario
load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t *config_path) {
    // Load .NET Core
    int rc = init_fptr(config_path, nullptr, &cxt);
    if (rc != 0 || cxt == nullptr) {
        std::ostringstream oss;
        oss << std::hex << std::showbase;
        CSSHARP_CORE_ERROR("Init failed: {}. Return Code: {}", oss.str(), rc);
        close_fptr(cxt);
        return nullptr;
    }

    // Get the load assembly function pointer
    void* load_assembly_and_get_function_pointer = nullptr;
    rc = get_delegate_fptr(cxt, hdt_load_assembly_and_get_function_pointer, &load_assembly_and_get_function_pointer);
    if (rc != 0 || load_assembly_and_get_function_pointer == nullptr) {
        std::ostringstream oss;
        oss << std::hex << std::showbase;
        CSSHARP_CORE_ERROR("Get delegate failed: {}. Return Code: {}", oss.str(), rc);
        return nullptr;
    }

    // close_fptr(cxt);
    return (load_assembly_and_get_function_pointer_fn)load_assembly_and_get_function_pointer;
}

}  // namespace

CDotNetManager::CDotNetManager() {}

CDotNetManager::~CDotNetManager() {}

bool CDotNetManager::Initialize() {
    std::string baseDir = counterstrikesharp::utils::PluginDirectory();

    if (!load_hostfxr()) {
        CSSHARP_CORE_ERROR("Failed to initialize .NET");
        return false;
    }

    CSSHARP_CORE_INFO(".NET Runtime Initialised.");

    std::string wideStr = std::string(baseDir + "/api/CounterStrikeSharp.API.runtimeconfig.json");

    CSSHARP_CORE_INFO("wideStr -> {}", wideStr);

    load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer = nullptr;
    load_assembly_and_get_function_pointer = get_dotnet_load_assembly(converter.from_bytes(wideStr).c_str());
    if (load_assembly_and_get_function_pointer == nullptr) {
        CSSHARP_CORE_ERROR("Failure: get_dotnet_load_assembly()");
        return false;
    }

    auto dotnetlib_path = converter.from_bytes(std::string(baseDir + "/api/CounterStrikeSharp.API.dll"));
    auto dotnet_type = converter.from_bytes("CounterStrikeSharp.API.Core.Helpers, CounterStrikeSharp.API");

    CSSHARP_CORE_INFO("dotnetlib_path -> {}", converter.to_bytes(dotnetlib_path));
    CSSHARP_CORE_INFO("dotnet_type -> {}", converter.to_bytes(dotnet_type));
    // Namespace, assembly name

    // FIXME
    typedef int(CORECLR_DELEGATE_CALLTYPE * custom_entry_point_fn)();
    custom_entry_point_fn entry_point = nullptr;

    int rc = load_assembly_and_get_function_pointer(
        dotnetlib_path.c_str(), 
        dotnet_type.c_str(), 
        converter.from_bytes("LoadAllPlugins").c_str(),
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr, 
        (void **)&entry_point);

    assert(rc == 0 && entry_point != nullptr &&
           "Failure: load_assembly_and_get_function_pointer()");

    const bool success = entry_point();
    if (!success) {
        CSSHARP_CORE_ERROR("not success! Failed to initialize .NET");
        return false;
    }

    CSSHARP_CORE_INFO("CounterStrikeSharp.API Loaded Successfully.");
    return true;
}

void CDotNetManager::UnloadPlugin(PluginContext *context) {}

void CDotNetManager::Shutdown() {
    // CoreCLR does not currently supporting unloading... :(
}

PluginContext *CDotNetManager::FindContext(std::string path) { return nullptr; }
