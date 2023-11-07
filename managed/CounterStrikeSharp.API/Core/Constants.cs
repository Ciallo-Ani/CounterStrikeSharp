using System.Runtime.InteropServices;

namespace CounterStrikeSharp.API.Core;

public static class Constants
{
    public static string ROOT_BINARY_PATH { get; }
    public static string GAME_BINARY_PATH { get; }

    static Constants()
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            ROOT_BINARY_PATH = "/bin/linuxsteamrt64/";
            GAME_BINARY_PATH = "/csgo/bin/linuxsteamrt64/";
        }
        else
        {
            ROOT_BINARY_PATH = "/bin/win64/";
            GAME_BINARY_PATH = "/csgo/bin/win64/";
        }
    }
}