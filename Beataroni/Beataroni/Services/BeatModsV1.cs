using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Text.Json;
using Beataroni.Models.BeatMods;

namespace Beataroni.Services
{
  public class BeatModsV1
  {
    public static readonly Dictionary<string, string> DefaultFilters = new Dictionary<string, string>()
    {
      {"status","approved"},
    };

    /// API endpoints/urls
    /// Note: You MUST not have '//' in the resulting URLs, beatmods can't handle this
    private static readonly string APIDownload = "https://beatmods.com";
    private static readonly string APIVersion = "https://versions.beatmods.com/versions.json";
    private static readonly string APIMod = "https://beatmods.com/api/v1/mod";

    /// Fetch list of beatsaber/mod versions
    /// List will be returned in order sent from server - Should be newest -> oldest
    public static IList<string> FetchBSVersions()
    {
      var endpoint = $"{APIVersion}";
      
      try
      {
        var req = WebRequest.Create(endpoint);
        req.Method = "GET";

        var resp = req.GetResponse();

        var json = "";
        using (Stream s = resp.GetResponseStream())
        {
          var reader = new StreamReader(s, Encoding.UTF8);
          json = reader.ReadToEnd();
        }

        List<string> result = JsonSerializer.Deserialize<List<string>>(json);
        return result;
      } catch (WebException e)
      {
        Console.WriteLine($"Error fetching BS versions: {e.Message}");
        return null;
      }
    }

    /// Fetch list of mods for a set of filters
    /// Typical filters would be game version, installation type, mod status
    public static IList<Mod> FetchMods( Dictionary<string, string> filters )
    {
      var endpoint = $"{APIMod}";

      if( filters != null && filters.Count > 0 )
      {
        endpoint += "?";
        var first = true;
        foreach( var f in filters )
        {
          if( !first )
          {
            endpoint += "&";
          }
          endpoint += $"{f.Key}={f.Value}";
          first = false;
        }
      }

      try
      {
        var req = WebRequest.Create(endpoint);
        req.Method = "GET";

        var resp = req.GetResponse();

        var json = "";
        using (Stream s = resp.GetResponseStream())
        {
          var reader = new StreamReader(s, Encoding.UTF8);
          json = reader.ReadToEnd();
        }

        List<Mod> mods = JsonSerializer.Deserialize<List<Mod>>(json);

        // Expand any dependencies into .net object references
        mods.ForEach(x => x.ExpandDependencyRefs(mods));

        return mods;
      } catch (WebException e)
      {
        Console.WriteLine($"Error fetching BS Mods: {e.Message}");
        return null;
      }
    }
    
    /// Download a mod to a temporary file
    /// Returns either a path to the file, or null if download failed for some reason
    public static bool DownloadMod( Download dl, string destFile )
    {
      var endpoint = $"{APIDownload}";
      // Remember - Beatmods is really sensitive over '//'
      if (!dl.url.StartsWith('/'))
      {
        endpoint += "/";
      }
      endpoint += dl.url;
      try
      {
        var client = new WebClient();
        client.DownloadFile(endpoint, destFile);
      }
      catch (WebException e)
      {
        Console.WriteLine($"Error downloading file: {e.Message}");
        return false;
      }
      return true;
    }
  }
}
