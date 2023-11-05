using System.IO;
using CounterStrikeSharp.API.Core;

namespace CounterStrikeSharp.API.Modules.Memory;

public class Addresses
{
    public static string EnginePath = Path.Join(Server.GameDirectory, Constants.ROOT_BINARY_PATH, "engine2.dll");
    public static string Tier0Path = Path.Join(Server.GameDirectory, Constants.ROOT_BINARY_PATH, "tier0.dll");
    public static string ServerPath = Path.Join(Server.GameDirectory, Constants.GAME_BINARY_PATH, "server.dll");

    public static string SchemaSystemPath =
        Path.Join(Server.GameDirectory, Constants.ROOT_BINARY_PATH, "schemasystem.dll");

    public static string VScriptPath = Path.Join(Server.GameDirectory, Constants.ROOT_BINARY_PATH, "vscript.dll");
}