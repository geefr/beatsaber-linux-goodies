using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json.Serialization;
using System.Text.Json;

namespace Beataroni.Services
{
  public class Settings
  {
    public string BSVersion {get;set;} = "";
    public string BSInstall {get;set;} = "";

    private static string SettingsFile = Environment.GetEnvironmentVariable(RuntimeInformation.IsOSPlatform(OSPlatform.Linux) ? "HOME" : "LocalAppData" ) + "/.config/beataroni/config.json";

    public Settings() {}

    public static Settings Load()
    {
      try
      {
        var confText = File.ReadAllText(SettingsFile, Encoding.UTF8);
        return JsonSerializer.Deserialize<Settings>(confText);
      } catch(Exception)
      {
        return new Settings();
      }
    }
    public bool Save()
    {
      try
      {
        var confDir = Directory.GetParent(SettingsFile);
        if( !confDir.Exists )
        {
          Directory.CreateDirectory(confDir.FullName);
        }
        var confText = JsonSerializer.Serialize<Settings>(this);
        File.WriteAllText(SettingsFile, confText, Encoding.UTF8);
        return true;
      } catch(Exception)
      {
        return false;
      }
    }
  }
}
