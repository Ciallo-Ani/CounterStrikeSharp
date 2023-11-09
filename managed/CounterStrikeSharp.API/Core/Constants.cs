using System.Runtime.InteropServices;

namespace CounterStrikeSharp.API.Core;

public static class Constants
{
    public static string ROOT_BINARY_PATH   { get; }
    public static string GAME_BINARY_PATH   { get; }
    public static string MODULE_PREFIX      { get; }
    public static string MODULE_EXT         { get; }

    static Constants()
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            ROOT_BINARY_PATH = "/bin/linuxsteamrt64/";
            GAME_BINARY_PATH = "/csgo/bin/linuxsteamrt64/";
            MODULE_PREFIX    = "lib";
            MODULE_EXT       = ".so";
        }
        else
        {
            ROOT_BINARY_PATH = "/bin/win64/";
            GAME_BINARY_PATH = "/csgo/bin/win64/";
            MODULE_PREFIX    = "";
            MODULE_EXT       = ".dll";
        }
    }
}