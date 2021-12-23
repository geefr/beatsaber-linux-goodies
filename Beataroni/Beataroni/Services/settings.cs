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
    // TODO: Most of these are blank - will be initialised to sensible defaults,
    // or need to be setup each time installer is launched. Will need persistent settings
    // storage for this.
    public string BSVersion {get;set;} = "";
    public string BSInstall {get;set;} = "";

    private static string SettingsFile = Environment.GetEnvironmentVariable(RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? "LocalAppData" : "Home") + "/.config/beataroni/config.json";

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
