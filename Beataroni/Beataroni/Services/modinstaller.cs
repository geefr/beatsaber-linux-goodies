using Beataroni.Models.BeatMods;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;

namespace Beataroni.Services
{
  public class ModInstaller
  {
    private List<string> BSPathsToFix = new List<string>
    {
      "Beat Saber_Data",
      "DLC",
      "MonoBleedingEdge",
      "Plugins",
      "UserData",
      "IPA",
      "Libs",
      "CustomAvatars",
      "CustomSabers",
      "CustomNotes",
      "CustomCampaigns",
      "CustomPlatforms",
      "Playlists",
    };

    /**
     * Ensure the case of path is correct, so that files are placed
     * in the correct folders under BS install
     */
    private string FixPath(string path)
    {
      var foundPath = BSPathsToFix.Find(x =>
      {
        if (path.Length < x.Length) return false;
        var testPath = path.Remove(x.Length);
        return string.Equals(testPath, x, StringComparison.OrdinalIgnoreCase);
      });
      if (string.IsNullOrEmpty(foundPath)) return path;
      return $"{foundPath}{path.Substring(foundPath.Length)}";
    }

    public bool InstallMod(Mod m, string bsInstall)
    {
      foreach (var dl in m.downloads)
      {
        if (!(dl.type.Equals("steam") || dl.type.Equals("universal")))
        {
          // TODO: At the moment Beataroni only supports steam installs - As Oculus doesn't work on Linux
        }

        // Download the file
        var tmpFile = Path.GetTempFileName();
        if (!BeatModsV1.DownloadMod(dl, tmpFile))
        {
          return false;
        }

        // Extract the archive
        bsInstall = Path.GetFullPath(bsInstall);

        // Ensures that the last character on the extraction path
        // is the directory separator char.
        // Without this, a malicious zip file could try to traverse outside of the expected
        // extraction path.
        if (!bsInstall.EndsWith(Path.DirectorySeparatorChar.ToString(), StringComparison.Ordinal))
          bsInstall += Path.DirectorySeparatorChar;

        try
        {
          using (ZipArchive archive = ZipFile.OpenRead(tmpFile))
          {
            foreach (ZipArchiveEntry entry in archive.Entries)
            {
              // Ported from QBeat/util.cpp
              foreach (var correctPath in BSPathsToFix)
              {
                var path = entry.FullName;
                if (path.StartsWith(correctPath, StringComparison.OrdinalIgnoreCase))
                {
                  path = path.Remove(0, correctPath.Length);
                  path = $"{correctPath}{path}";
                }
              }

              // Gets the full path to ensure that relative segments are removed.
              string destinationPath = Path.GetFullPath(Path.Combine(bsInstall, entry.FullName));

              if (entry.FullName.EndsWith('/'))
              {
                // It's not well documented, but this is a directory
                Directory.CreateDirectory(destinationPath);
              }
              else
              {
                // Otherwise it must be a file
                // Ordinal match is safest, case-sensitive volumes can be mounted within volumes that
                // are case-insensitive.
                if (destinationPath.StartsWith(bsInstall, StringComparison.Ordinal)) entry.ExtractToFile(destinationPath);
              }
            }
          }

          File.Delete(tmpFile);
        }
        catch( Exception e )
        {
          Console.WriteLine($"Error Extracting Mod: {dl.url}, {e.Message}");
          return false;
        }
      }
      return true;
    }

    public bool PatchBeatSaber(Mod m)
    {
      // TODO
      return false;
    }

    public bool UninstallMod(Mod m)
    {
      // TODO
      return false;
    }

    public bool ValidateMod(Mod m)
    {
      // TODO
      return false;
    }

    public bool IsModInstalled(Mod m)
    {
      // TODO - Return true if mod at least partially installed
      return false;
    }
  }
}
