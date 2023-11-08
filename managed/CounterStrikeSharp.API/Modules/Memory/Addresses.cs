using System.IO;
using CounterStrikeSharp.API.Core;

namespace CounterStrikeSharp.API.Modules.Memory;

public class Addresses
{
    public static string EnginePath = 
        Path.Join(Server.GameDirectory, Constants.ROOT_BINARY_PATH, Constants.MODULE_PREFIX + "engine2" + Constants.MODULE_EXT);
    public static string Tier0Path = 
        Path.Join(Server.GameDirectory, Constants.ROOT_BINARY_PATH, Constants.MODULE_PREFIX + "tier0" + Constants.MODULE_EXT);
    public static string ServerPath = 
        Path.Join(Server.GameDirectory, Constants.GAME_BINARY_PATH, Constants.MODULE_PREFIX + "server" + Constants.MODULE_EXT);
    public static string SchemaSystemPath =
        Path.Join(Server.GameDirectory, Constants.ROOT_BINARY_PATH, Constants.MODULE_PREFIX + "schemasystem" + Constants.MODULE_EXT);
    public static string VScriptPath = 
        Path.Join(Server.GameDirectory, Constants.ROOT_BINARY_PATH, Constants.MODULE_PREFIX + "vscript" + Constants.MODULE_EXT);
}