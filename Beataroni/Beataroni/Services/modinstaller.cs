using Beataroni.Models.BeatMods;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Runtime.InteropServices;

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

              // Ordinal match is safest, case-sensitive volumes can be mounted within volumes that
              // are case-insensitive.
              if (destinationPath.StartsWith(bsInstall, StringComparison.Ordinal))
              {
                if (entry.FullName.EndsWith('/'))
                {
                  // It's not well documented, but this is a directory
                  Directory.CreateDirectory(destinationPath);
                }
                else
                {
                  // Otherwise it must be a file
                  Directory.CreateDirectory(Path.GetDirectoryName(destinationPath));
                  entry.ExtractToFile(destinationPath, true);
                }
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

    public bool PatchBeatSaber(string bsInstall)
    {
      // To patch beat saber we just need to run IPA.exe in the
      // directory
      // This method requires that BSIPA be installed first
      var ipaExe = "IPA.exe";
      var ipaLinuxExe = "IPA-Minimal";
      var runningOnLinux = RuntimeInformation.IsOSPlatform(OSPlatform.Linux);
      if (runningOnLinux)
      {
        ipaExe = ipaLinuxExe;
        if (!PatchSteamProtonPrefix(bsInstall))
        {
          Console.WriteLine("PatchBeatSaber: Failed to patch steam's proton prefix");
          return false;
        }

        // Copy the Linux build of IPA into bsInstall
        // TODO: The packaging/build setup for this binary isn't great, consider
        // pulling it in directly as a library?
        try
        {
          var roniDir = Path.Combine(
            Path.GetDirectoryName(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName) 
            ?? throw new InvalidOperationException(), "Beataroni");
          File.Copy($"{roniDir}/{ipaLinuxExe}", $"{bsInstall}/{ipaLinuxExe}");
        }
        catch(Exception e)
        {
          Console.WriteLine($"PatchBeatSaber: Failed to install IPA-Minimal: {e.Message}");
          return false;
        }
      }

      try
      {
        Process proc = new Process();
        proc.StartInfo.UseShellExecute = false;
        proc.StartInfo.FileName = $"{bsInstall}/{ipaExe}";
        proc.StartInfo.WorkingDirectory = $"{bsInstall}";
        proc.StartInfo.Arguments = "-n"; // Don't wait for user input
        proc.StartInfo.CreateNoWindow = true;
        proc.Start();
        // Assuming proc will kill itself here, if not we'll hang, or need to use the Kill method
        proc.WaitForExit();
        if( proc.ExitCode != 0 )
        {
          Console.WriteLine($"PatchBeatSaber: IPA.exe returned non-zero:\n StdOut: {proc.StandardOutput} \n StdErr: {proc.StandardError}");
          return false;
        }
      }
      catch (Exception e)
      {
        Console.WriteLine(e.Message);
        return false;
      }
      return true;
    }

    public bool PatchSteamProtonPrefix(string bsInstall)
    {
      // Make a small modification to beat saber's proton prefix
      // in order to ensure winhttp.dll is loaded from IPA instead
      // of using the built-in wine variant


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
